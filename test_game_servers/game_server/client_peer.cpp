#include "stdafx.h"
#include "client_peer.h"

#include "channel_manager.h"

#include <boost/bind.hpp>

#include "packet_generated.h"

client_peer::client_peer(const int& idx, channel_manager* channel_manager)
	: PeerEx(idx)
    , m_channel_manager(channel_manager)
    , m_channel_index(0)
    , m_user_index(0)
{
    m_custom_dispatcher.register_func(PACKET_NUMBER::C2S::ENTER, boost::bind(&client_peer::on_enter, this, _1));
    m_custom_dispatcher.register_func(PACKET_NUMBER::C2S::LEAVE, boost::bind(&client_peer::on_leave, this, _1));
    m_custom_dispatcher.register_func(PACKET_NUMBER::C2S::BROADCAST, boost::bind(&client_peer::on_broadcast, this, _1));

}

client_peer::~client_peer()
{
}

void client_peer::on_connect()
{
	LOG_INFO(L"[%d]", get_idx());
}

void client_peer::on_disconnect()
{
	LOG_INFO(L"[%d]", get_idx());

    if (m_channel_index != 0)
    {
        light::FlatBuffersPtr<PACKET::LEAVE> ptr;
        m_channel_manager->leave(this, m_channel_index, ptr);
        m_channel_index = 0;
    }

	reuse();
}

void client_peer::on_update()
{
}


bool client_peer::on_broadcast(light::net::PacketHeader* header)
{
    light::FlatBuffersPtr<PACKET::BROADCAST> packet(header->get_size() - header->header_size(), header->data());
    if (false == packet.parsing())
    {
        LOG_ERROR(L"invalid packet received. [%d] [%d, %d]", get_idx(), header->get_size(), header->header_size());
        return false;
    }

    m_channel_manager->strand_post_job(boost::bind(&channel_manager::broadcast, m_channel_manager.get(), this, header->index(), packet), m_channel_manager->channel_manager_strand_position());
    return true;
}

bool client_peer::on_enter(light::net::PacketHeader* header)
{
    light::FlatBuffersPtr<PACKET::ENTER> packet(header->get_size() - header->header_size(), header->data());
    if ( false == packet.parsing())
    {
        LOG_ERROR(L"invalid packet received. [%d] [%d, %d]", get_idx(), header->get_size(), header->header_size());
        return false;
    }

    if (NULL == packet->USER_NAME())
    {
        return false;
    }

    const char* user_name = packet->USER_NAME()->c_str();
    m_user_name = CHAR_TO_WCHAR(user_name);
    m_user_index = packet->USER_INDEX();

    m_channel_manager->strand_post_job(boost::bind(&channel_manager::enter, m_channel_manager.get(), this, header->index(), packet), m_channel_manager->channel_manager_strand_position());
    m_channel_index = header->index();
    return true;
}

bool client_peer::on_leave(light::net::PacketHeader* header)
{
    light::FlatBuffersPtr<PACKET::LEAVE> packet(header->get_size() - header->header_size(), header->data());
    if (false == packet.parsing())
    {
        LOG_ERROR(L"invalid packet received. [%d] [%d, %d]", get_idx(), header->get_size(), header->header_size());
        return false;
    }
    m_channel_manager->strand_post_job(boost::bind(&channel_manager::leave, m_channel_manager.get(), this, header->index(), packet), m_channel_manager->channel_manager_strand_position());
    m_channel_index = 0;
    return true;
}

void client_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(len);

    light::net::PacketHeader* header = (light::net::PacketHeader*)ptr;
    if (false == m_custom_dispatcher.call(header->command(), header))
    {
        send(header->get_size(), header); // 디스패칭 안되어 있으면 에코~
    }
}