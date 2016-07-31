#pragma once



namespace light
{

namespace net
{

///릴레이 서버에 사용할 기본 포트
const int P2P_ECHO_PORT = 50000;

///UDP 소켓 바인드시 재시도 횟수
const int UDP_BIND_TRY_COUNT = 255;

struct PeerInformation
{
	int m_idx;

	SOCKET_ADDR_IN m_udp_sock_addr_in;

	PeerInformation(){
		memset(this, 0, sizeof(*this));
	}

	PeerInformation(int idx, const SOCKET_ADDR_IN& UdpSockAddrIn) 
		: m_idx(idx), m_udp_sock_addr_in (UdpSockAddrIn)
	{
	}
};

namespace UDP
{

enum COMMAND
{ 
	IS_CONNECTED = 30000,		// UDP 연결 확인 요청
	SUCCESSED,					// UDP 연결 확인 완료 패킷
};

#pragma pack(1)

} // namespace 

#pragma pack()

} // namespace net

} // namespace light
