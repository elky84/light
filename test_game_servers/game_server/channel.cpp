#include "stdafx.h"
#include "channel.h"

#include "client_peer.h"

#include "PacketStream.h"

#include "packet_generated.h"

#include "CommandLineArguments.h"

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

bool channel::enter(client_peer* peer, light::FlatBuffersPtr<PACKET::ENTER> packet)
{
    LOG_INFO(L"[%d]", peer->get_idx());

    if (m_peers.find(peer) != m_peers.end())
    {
        LOG_ERROR(L"[%d] already joined.", peer->get_idx());
        return false;
    }

    if (NULL == packet->USER_NAME())
    {
        return false;
    }

    if (light::CommandLineArguments::has(L"auth_pass") == false)
    {
        auto it = m_reserved.find(packet->USER_INDEX());
        if (m_reserved.end() == it)
        {
            LOG_ERROR(L"peer[%d] req[%d]", peer->get_idx(), packet->USER_INDEX());
            return false;
        }

        std::string hash = packet->HASH() == NULL ? "" : packet->HASH()->str();

        if (hash != it->second)
        {
            LOG_ERROR(L"peer[%d] req[%d, %S] own[%d, %S] ", peer->get_idx(), packet->USER_INDEX(), hash.c_str(), it->first, it->second.c_str());
            return false;
        }
    }

    for each(auto it in m_peers) // 기존 유저 정보를 새 peer에게 전달하고
    {
        light::net::PacketStream stream(PACKET_NUMBER::S2C::ENTER, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateENTER(builder, builder.CreateString(WCHAR_TO_CHAR(it->user_name().c_str())), builder.CreateString(""), it->user_index());
        builder.Finish(object);

        stream.write((light::net::PACKET_SIZE)builder.GetSize(), builder.GetBufferPointer());

        peer->send(stream.Header()->get_size(), stream.Header());
    }

    m_peers.insert(peer); // 새 peer를 등록 시킨 후, 새 유저 정보를 모두에게 broadcast한다.
    {
        light::net::PacketStream stream(PACKET_NUMBER::S2C::ENTER, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateENTER(builder, builder.CreateString(packet->USER_NAME()), builder.CreateString(""), packet->USER_INDEX());
        light::FlatBuffersHelper::write(stream, builder, object);

        broadcast(stream);
    }

    return true;
}

bool channel::leave(client_peer* peer, light::FlatBuffersPtr<PACKET::LEAVE> packet)
{
    LOG_INFO(L"[%d]", peer->get_idx());

    {
        light::net::PacketStream stream(PACKET_NUMBER::S2C::LEAVE, m_index);

        flatbuffers::FlatBufferBuilder builder;
        auto object = PACKET::CreateLEAVE(builder, light::FlatBuffersHelper::to_string(builder, peer->user_name()), peer->user_index());

        light::FlatBuffersHelper::write(stream, builder, object);

        broadcast(stream);
    }
    m_peers.erase(peer);

    return true;
}

bool channel::broadcast(client_peer* peer, light::FlatBuffersPtr<PACKET::BROADCAST> packet)
{
    if (m_peers.find(peer) == m_peers.end())
    {
        LOG_ERROR(L"not exist peer [%d]", peer->get_idx());
        return false;
    }


    light::net::PacketStream stream(PACKET_NUMBER::S2C::BROADCAST, m_index);
    stream.write(packet.buffer()->get_size(), packet.buffer()->get_buffer());

    broadcast(stream);
    return true;
}

bool channel::broadcast(light::net::PacketStream& stream)
{
    for each(auto it in m_peers)
    {
        it->send(stream.Header()->get_size(), stream.Header());
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