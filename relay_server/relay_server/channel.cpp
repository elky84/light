#include "stdafx.h"
#include "channel.h"

#include "client_peer.h"
#include "packet_generated.h"

#include "PacketStream.h"
#include "CommandLineArguments.h"

#include <array>

channel::channel(channel::index index)
    : m_index(index)
{

}

channel::~channel()
{
}

void channel::on_update()
{

}

bool channel::on_join(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    LOG_INFO(L"[%d]", peer->get_idx());

    light::FlatBuffersPtr<PACKET::REQ_JOIN> packet(stream->body_size(), stream->body());
    if (packet.parsing() == false)
    {
        LOG_ERROR(L"receieved invalid packet. [%d] [%d]", peer->get_idx(), stream->Header()->command());
        return false;
    }

    if (NULL == packet->USER_NAME())
    {
        LOG_ERROR(L"receieved empty user_name. [%d] [%d]", peer->get_idx(), stream->Header()->command());
        return false;
    }

    if (m_peers.find(packet->USER_INDEX()) != m_peers.end())
    {
        LOG_ERROR(L"[%d] already joined.", peer->get_idx());
        return false;
    }

    if ( false == light::CommandLineArguments::has(L"auth_pass"))
    {
        auto reserve = m_reserved.find(packet->USER_INDEX());
        if (m_reserved.end() == reserve)
        {
            LOG_ERROR(L"not find in reserved information.peer[%d] req[%d]", peer->get_idx(), packet->USER_INDEX());
            return false;
        }

        std::string hash = packet->HASH() == NULL ? "" : packet->HASH()->str();

        if (hash != reserve->second)
        {
            LOG_ERROR(L"different hash. peer[%d] req[%d, %S] own[%d, %S] ", peer->get_idx(), packet->USER_INDEX(), hash.c_str(), reserve->first, reserve->second.c_str());
            return false;
        }
    }

    peer->user_index(packet->USER_INDEX());

    const std::wstring user_name = CHAR_TO_WCHAR(packet->USER_NAME()->c_str());
    peer->user_name(user_name);

    peer->set_channel(this);

    {
        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::JOIN, m_index);
        flatbuffers::FlatBufferBuilder builder;

        std::vector<flatbuffers::Offset<PACKET::PLAYER_DATA>> player_data_buffers;
        for each(auto it in m_peers) // 기존 유저 정보를 새 peer에게 전달하고
        {
            auto offset = PACKET::CreatePLAYER_DATA(builder, it.second->user_index(), builder.CreateString(WSTRING_TO_STRING(it.second->user_name())));
            player_data_buffers.push_back(offset);
        }

        auto player_datas_offset = builder.CreateVector(player_data_buffers);

        auto player_data_offset = PACKET::CreatePLAYER_DATA(builder, packet->USER_INDEX(), builder.CreateString(packet->USER_NAME()));

        auto object = PACKET::CreateRES_JOIN(builder, player_data_offset, player_datas_offset);
        builder.Finish(object);
        res_stream.write((light::net::PACKET_SIZE)builder.GetSize(), builder.GetBufferPointer());

        peer->send(res_stream.Header()->get_size(), res_stream.Header());
    }

    {
        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::JOINED, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto player_data_offset = PACKET::CreatePLAYER_DATA(builder, packet->USER_INDEX(), builder.CreateString(packet->USER_NAME()));
        auto object = PACKET::CreateJOINED(builder, player_data_offset);
        light::FlatBuffersHelper::write(res_stream, builder, object);

        broadcast(res_stream);
    }

    m_peers.insert({ peer->user_index(), peer }); // 패킷을 모두 쏜 후, 나도 피어 그룹에 포함시킨다.
    return true;
}

bool channel::on_leave(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    UNREFERENCED_PARAMETER(stream);
    LOG_INFO(L"[%d] [%d]", peer->get_idx(), peer->user_index());

    {
        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::LEAVE, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateRES_LEAVE(builder, peer->user_index());

        light::FlatBuffersHelper::write(res_stream, builder, object);

        broadcast(res_stream);
    }

    m_packets.erase(peer->user_index());
    m_peers.erase(peer->user_index());
    return true;
}

bool channel::on_send(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    if (m_peers.find(peer->user_index()) == m_peers.end())
    {
        LOG_ERROR(L"not exist peer [%d] [%d]", peer->get_idx(), peer->user_index());
        return false;
    }

    light::FlatBuffersPtr<PACKET::REQ_SEND> packet(stream->body_size(), stream->body());
    if (packet.parsing() == false)
    {
        LOG_ERROR(L"receieved invalid packet. [%d]", stream->Header()->command());
        return false;
    }

    if (m_packets.find(peer->user_index()) != m_packets.end())
    {
        LOG_ERROR(L"already received packet. [%d]", stream->Header()->command());
        return false;
    }

    m_packets[peer->user_index()] = packet;

    if (m_packets.size() == m_peers.size() )
    {
        flatbuffers::FlatBufferBuilder builder;
        std::vector<flatbuffers::Offset<flatbuffers::String>> strings;
        for each(auto it in m_packets) // 기존 유저 정보를 새 peer에게 전달하고
        {
            strings.push_back(builder.CreateString(it.second->MESSAGE()));
        }

        auto messages_offset = builder.CreateVector(strings);
        
        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::SEND, m_index);
        auto object = PACKET::CreateRES_SEND(builder, messages_offset);

        light::FlatBuffersHelper::write(res_stream, builder, object);

        broadcast(res_stream);
        m_packets.clear();
    }
    return true;
}


bool channel::on_spawn(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    if (m_peers.find(peer->user_index()) == m_peers.end())
    {
        LOG_ERROR(L"not exist peer [%d] [%d]", peer->get_idx(), peer->user_index());
        return false;
    }

    light::FlatBuffersPtr<PACKET::REQ_SPAWN> packet(stream->body_size(), stream->body());
    if (packet.parsing() == false)
    {
        LOG_ERROR(L"receieved invalid packet. [%d] [%d] [%d]", peer->get_idx(), peer->user_index(), stream->Header()->command());
        return false;
    }

    {
        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::SPAWN, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateRES_SPAWN(builder, packet->MONSTER_ID());

        light::FlatBuffersHelper::write(res_stream, builder, object);

        broadcast(res_stream);
    }

    UNREFERENCED_PARAMETER(stream);
    return true;
}


bool channel::on_ready(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    if (m_peers.find(peer->user_index()) == m_peers.end())
    {
        LOG_ERROR(L"not exist peer [%d] [%d]", peer->get_idx(), peer->user_index());
        return false;
    }

    {
        peer->ready(true);

        bool all_ready = true;
        for each(auto it in m_peers) // 기존 유저 정보를 새 peer에게 전달하고
        {
            if ( false == it.second->ready())
            {
                all_ready = false;
                break;
            }
        }

        light::net::PacketStream res_stream(PACKET_NUMBER::S2C::READY, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateRES_READY(builder, all_ready);
        light::FlatBuffersHelper::write(res_stream, builder, object);

        broadcast(res_stream);
    }

    UNREFERENCED_PARAMETER(stream);
    return true;
}

bool channel::broadcast(light::net::PacketStream& stream)
{
    for each(auto it in m_peers)
    {
        it.second->send(stream.Header()->get_size(), stream.Header());
    }

    LOG_TRACE(L"[%d]", stream.Header()->command());
    return true;
}

bool channel::reservation(int id, const std::string& hash)
{
    auto it = m_reserved.find(id);
    if (m_reserved.end() != it)
    {
        LOG_INFO(L"req[%d, %S] own[%d, %S]", id, hash.c_str(), it->first, it->second.c_str());
        it->second = hash;
        return true;
    }

    m_reserved[id] = hash;
    LOG_INFO(L"req[%d, %S]", id, hash.c_str());
    return true;
}