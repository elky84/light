#pragma once

namespace light
{

namespace net
{

class Peer;

class IoInterface 
{
public:
	IoInterface(Peer* peer);

	virtual ~IoInterface();

	virtual bool send(light::net::PACKET_SIZE len, const void* ptr) = 0;

	virtual bool on_receive(light::net::PACKET_SIZE len) = 0;

	virtual void reuse() = 0;

	void set_peer(Peer* peer);

	Peer* get_peer()
    {
        return m_peer.get();
    }

	virtual bool disconnect(RESULT_CODE::Type code) = 0;

protected:
	std::unique_ptr<Peer> m_peer;
};

} // namespace net

} // namespace light