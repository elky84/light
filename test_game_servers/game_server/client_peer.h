#pragma once

#include "PeerEx.h"

#include "Dispatcher.hpp"

#include "channel.h"

class channel_manager;

class client_peer
	: public light::net::PeerEx
{
public:
	client_peer(const int& idx, channel_manager* channel_manager);
	~client_peer();

	void on_update();

	virtual void on_connect();
	virtual void on_disconnect();

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);

public:
    const std::wstring& user_name() const
    {
        return m_user_name;
    }

    int user_index() const
    {
        return m_user_index;
    }

private:
    bool on_enter(light::net::PacketHeader* header);

    bool on_leave(light::net::PacketHeader* header);

    bool on_broadcast(light::net::PacketHeader* header);

public:
    light::weak_raw_ptr<channel_manager> m_channel_manager;

private:
    typedef light::Dispatcher<light::net::PACKET_ID, light::net::PacketHeader*> DISPATCHER;

    DISPATCHER m_custom_dispatcher;

    channel::index m_channel_index;

    std::wstring m_user_name;

    int m_user_index;
};
