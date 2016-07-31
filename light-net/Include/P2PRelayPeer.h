#pragma once

#include "Peer.h"
#include "Dispatcher.hpp"

namespace light
{

namespace net
{

class P2PInterface;

namespace R2P
{

struct Header;
enum COMMAND;
		
} // namespace R2P

namespace P2R
{

struct Header;

} // namespace P2R


/**
@brief 릴레이에서 사용되는 피어
*/
class P2PRelayPeer
	: public Peer
{
public:
	///생성자
	P2PRelayPeer(light::weak_raw_ptr<P2PInterface> P2p, PACKET_DISPATCHER& dispatcher);

	///접속 완료 이벤트
	virtual void on_connect();

	///접속 해제 이벤트
	virtual void on_disconnect();

	///데이터 전송 메소드
	bool send(PacketHeader* header);

	/// 주기적 메소드
	virtual void on_update();


private:
	///릴레이 서버 인증 키
	bool on_auth_key(PacketStream& stream);

	///릴레이 서버 인증 정보
	bool on_auth_information(PacketStream& stream);

	///연결 대상 정보
	bool on_peer_information(PacketStream& stream);

	///접속 해제 알림
	bool on_disconnect_peer(PacketStream& stream);

	///핑
	bool on_ping(PacketStream& stream);

	bool on_relay(PacketStream& stream);

private:
    light::weak_raw_ptr<P2PInterface> m_p2p;
};

} // namespace net

} // namespace light
