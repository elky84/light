#pragma once

#include <set>

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
    bool enter(client_peer* peer, light::FlatBuffersPtr<PACKET::ENTER> packet);
    bool leave(client_peer* peer, light::FlatBuffersPtr<PACKET::LEAVE> packet);
    bool broadcast(client_peer* peer, light::FlatBuffersPtr<PACKET::BROADCAST> packet);

private:
    bool broadcast(light::net::PacketStream& stream);

public: // redis events
    bool reservation(int id, const std::string& hash);

public:
    std::set<client_peer*> m_peers;

    int m_index;

    std::map<int, std::string> m_reserved;
};
