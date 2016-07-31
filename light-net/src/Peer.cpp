#include "light-net.h"
#include "Peer.h"

#include "IoInterface.h"
#include "PacketStream.h"

namespace light
{

namespace net
{

Peer::Peer(IoInterface* io_interface) 
: m_io_interface(io_interface)
, m_sock_addr_in(NULL)
{
}

Peer::~Peer()
{

}

bool Peer::send(light::net::PACKET_SIZE len, const void* ptr)
{
	LOG_TRACE(L"[%d]", len);
	if(m_io_interface.load())
	{
		return m_io_interface.load()->send(len, ptr);
	}
	return false;
}

void Peer::disconnect(RESULT_CODE::Type code)
{
	if(m_io_interface.load())
	{
		m_io_interface.load()->disconnect(code);
	}
}

void Peer::reuse()
{
	if(m_io_interface.load())
	{
		return m_io_interface.load()->reuse();
	}
}

/**
@brief 패킷을 읽어 완성된 패킷 1개 단위로 짜르는 메소드
@param len 데이터 크기
@param ptr 데이터 포인터
@return 유효한 패킷의 크기. 0이면 패킷이 아직 덜 온것, INVALID_FILE_SIZE면 비정상 패킷
*/
PACKET_SIZE Peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
	if(len < PacketHeader::header_size())
	{
		LOG_DEBUG(L"[%d < %d]", len, PacketHeader::header_size());
		return 0;
	}

	PacketHeader* header = (PacketHeader*)ptr;
	if(header->get_size() <= 0 || 
        MAX_RECEIVE_BUFFER_SIZE < header->get_size() )
	{
		LOG_ERROR(L"[%d] [%d] [%d]", header->command(), header->get_size(), len);
		return INVALID_FILE_SIZE;
	}
    else if (len < header->get_size()) //헤더에서의 길이가, 패킷 길이보다 작으면?
    {
        LOG_TRACE(L"[%d] [%d < %u]", header->command(), len, header->get_size());
        return 0;
    }
	return header->get_size();
}


void Peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
	if(m_dispatcher.get() == NULL)
		return;

	PacketHeader* header = (PacketHeader*)ptr;
	PacketStream stream(len, ptr);
	if( false == m_dispatcher->call(header->command(), stream) )
	{
		LOG_ERROR(L"[%d, %d]", header->command(), header->get_size());
	}
}

} // namespace net

} // namespace light
