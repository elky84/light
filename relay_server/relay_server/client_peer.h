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

	virtual void on_connect();
	virtual void on_disconnect();

    virtual void on_receive(light::net::PACKET_SIZE len, const void* ptr);

    virtual void on_update();

public:
    void user_name(const std::wstring& name)
    {
        m_user_name = name;
    }

    const std::wstring& user_name() const
    {
        return m_user_name;
    }

    void user_index(int index)
    {
        m_user_index = index;
    }

    int user_index() const
    {
        return m_user_index;
    }

    void ready(bool flag)
    {
        m_ready = flag;
    }

    bool ready() const
    {
        return m_ready;
    }

    void set_channel(light::weak_raw_ptr<channel> ptr)
    {
        m_channel = ptr;
    }

    light::weak_raw_ptr<channel> get_channel()
    {
        return m_channel;
    }


private:
    bool m_ready = false;

    light::weak_raw_ptr<channel> m_channel;

    light::weak_raw_ptr<channel_manager> m_channel_manager;

private:
    std::wstring m_user_name;

    int m_user_index;
};
