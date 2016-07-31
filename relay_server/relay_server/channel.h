#pragma once

#include <map>

class client_peer;

class channel
{
public:
    typedef int index;

public:
    channel(channel::index index);
    ~channel();

    void on_update();

public: // packet events
    bool on_join(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);
    bool on_leave(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);
    bool on_send(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);
    bool on_spawn(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);
    bool on_ready(client_peer* peer, std::shared_ptr<light::net::PacketStream> stream);

    bool broadcast(light::net::PacketStream& stream);

public: // redis events
    bool reservation(int id, const std::string& hash);

public:
    int get_index() const
    {
        return m_index;
    }

private:
    std::map<int, client_peer*> m_peers;

    int m_index;

    std::map<int, std::string> m_reserved;

    std::map<int, light::FlatBuffersPtr<PACKET::REQ_SEND> > m_packets;

};
