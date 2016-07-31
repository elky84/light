#include "stdafx.h"
#include "client_peer.h"

#include "PacketStream.h"

#include "channel_manager.h"

#include <boost/bind.hpp>

#include "packet_generated.h"

client_peer::client_peer(const int& idx, channel_manager* channel_manager)
	: PeerEx(idx)
    , m_channel_manager(channel_manager)
    , m_user_index(0)
{

}

client_peer::~client_peer()
{
}

void client_peer::on_update()
{

}

void client_peer::on_connect()
{
	LOG_INFO(L"[%d]", get_idx());
}

void client_peer::on_disconnect()
{
	LOG_INFO(L"[%d]", get_idx());

    if (m_channel.get() != NULL)
    {
        std::shared_ptr<light::net::PacketStream> stream(new light::net::PacketStream(PACKET_NUMBER::C2S::LEAVE, m_channel->get_index()));
        m_channel_manager->on_leave(this, stream);

        m_channel.reset();

        m_ready = false;
    }

	reuse();
}

void client_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
    std::shared_ptr<light::net::PacketStream> stream(new light::net::PacketStream(len, ptr));
    m_channel_manager->strand_post_job(boost::bind(&channel_manager::on_packet, m_channel_manager.get(), this, stream), m_channel_manager->channel_manager_strand_position());

}