#pragma once

#include <memory>

#include "channel.h"
#include "async_worker.h"

class client_peer;

class channel_manager
{
public:
    channel_manager();
    ~channel_manager();

    void start();

    void on_update();

public: // packet events
    bool on_join(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

    bool on_leave(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

    bool on_send(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

    bool on_spawn(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

    bool on_ready(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

public:
    void on_packet(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

public:
    void on_redis_event(const std::string& msg);

public:

    template <typename T>
    inline void	strand_post_job(T handler, int position)
    {
        m_async_worker->strand_post_job(handler, position);
    }

public:
    int channel_manager_strand_position() const // 0번은 채널매니저와 외부 이벤트간의 strand 포지션.
    {
        return 0;
    }

    int channel_strand_position(int channel_no) const
    {
        return (channel_no % std::max<int>(m_channels.size() - 1, 1)) + 1; // 1~최대 채널갯수를 뽑아서 접근시키게하려 하는 의도.
    }

private:
    std::shared_ptr<light::async_worker> m_async_worker;

    std::unordered_map<channel::index, std::shared_ptr<channel> > m_channels;

    light::Dispatcher<light::net::PACKET_ID, client_peer*, std::shared_ptr<light::net::PacketStream> > m_dispatcher;
};
