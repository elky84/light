#include "stdafx.h"
#include "channel_manager.h"

#include "PacketStream.h"
#include "async_worker.h"

#include "client_peer.h"
#include "channel.h"

#include "json_parser.h"
#include <boost/foreach.hpp>

channel_manager::channel_manager()
{
    m_dispatcher.register_func(PACKET_NUMBER::C2S::JOIN, boost::bind(&channel_manager::on_join, this, _1, _2));
    m_dispatcher.register_func(PACKET_NUMBER::C2S::LEAVE, boost::bind(&channel_manager::on_leave, this, _1, _2));
    m_dispatcher.register_func(PACKET_NUMBER::C2S::SEND, boost::bind(&channel_manager::on_send, this, _1, _2));
    m_dispatcher.register_func(PACKET_NUMBER::C2S::SPAWN, boost::bind(&channel_manager::on_spawn, this, _1, _2));
    m_dispatcher.register_func(PACKET_NUMBER::C2S::READY, boost::bind(&channel_manager::on_ready, this, _1, _2));
}

channel_manager::~channel_manager()
{
}

void channel_manager::start()
{
    m_async_worker.reset(new light::async_worker());
    m_async_worker->start();
}

void channel_manager::on_update()
{

}

bool channel_manager::on_join(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    int index = stream->Header()->index();
    if (m_channels[index].get() == NULL)
    {
        LOG_INFO(L"create channel. [%d] [%d]", index, peer->get_idx());
        m_channels[index].reset(new channel(index));
    }

    strand_post_job(boost::bind(&channel::on_join, m_channels[index].get(), peer, stream), channel_strand_position(index));
    return true;
}

bool channel_manager::on_leave(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    int index = stream->Header()->index();
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return false;
    }

    strand_post_job(boost::bind(&channel::on_leave, m_channels[index].get(), peer, stream), channel_strand_position(index));
    return true;
}

bool channel_manager::on_send(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    int index = stream->Header()->index();
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return false;
    }

    strand_post_job(boost::bind(&channel::on_send, m_channels[index].get(), peer, stream), channel_strand_position(index));
    return true;
}

bool channel_manager::on_spawn(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    int index = stream->Header()->index();
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return false;
    }

    strand_post_job(boost::bind(&channel::on_spawn, m_channels[index].get(), peer, stream), channel_strand_position(index));
    return true;
}

bool channel_manager::on_ready(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    int index = stream->Header()->index();
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return false;
    }

    strand_post_job(boost::bind(&channel::on_ready, m_channels[index].get(), peer, stream), channel_strand_position(index));
    return true;
}

void channel_manager::on_redis_event(const std::string& msg)
{
    try
    {
        light::json::parser parser;
        parser.set(msg);

        int index = parser.get<int>("roomnum");
        if (m_channels[index].get() == NULL)
        {
            LOG_TRACE(L"create channel. [%d] [%S]", index, msg.c_str());
            m_channels[index].reset(new channel(index));
        }

        boost::property_tree::ptree& ptree = parser.get_child("members");
        BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, ptree)
        {
            int id = vt.second.get<int>("id");
            std::string hash = vt.second.get<std::string>("hash");

            LOG_TRACE(L"id[%d] hash[%S]", id, hash.c_str());

            strand_post_job(boost::bind(&channel::reservation, m_channels[index].get(), id, hash), channel_strand_position(index));
        }

        LOG_INFO(L"success. channel_no[%d]", index);
    }
    catch (std::exception const& e)
    {
        LOG_ERROR(L"%S. msg[%S]", e.what(), msg.c_str());
    }
}

void channel_manager::on_packet(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream)
{
    m_dispatcher.call(stream->Header()->command(), peer, stream);
}