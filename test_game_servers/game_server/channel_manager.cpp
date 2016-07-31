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

bool channel_manager::enter(client_peer* peer, channel::index index, light::FlatBuffersPtr<PACKET::ENTER> packet)
{
    if (m_channels[index].get() == NULL)
    {
        LOG_INFO(L"create channel. [%d] [%d]", index, peer->get_idx());
        m_channels[index].reset(new channel(index));
    }

    strand_post_job(boost::bind(&channel::enter, m_channels[index].get(), peer, packet), channel_strand_position(index));
    return true;
}

bool channel_manager::leave(client_peer* peer, channel::index index, light::FlatBuffersPtr<PACKET::LEAVE> packet)
{
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return false;
    }

    strand_post_job(boost::bind(&channel::leave, m_channels[index].get(), peer, packet), channel_strand_position(index));
    return true;
}

void channel_manager::broadcast(client_peer* peer, channel::index index, light::FlatBuffersPtr<PACKET::BROADCAST> packet)
{
    if (m_channels[index].get() == NULL)
    {
        LOG_ERROR(L"failed. channel is not exists. [%d] [%d]", index, peer->get_idx());
        return;
    }

    strand_post_job(boost::bind(&channel::broadcast, m_channels[index].get(), peer, packet), channel_strand_position(index));
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