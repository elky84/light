#pragma once

#include "SocketCommon.h"

#include "AsyncSelectInterface.h"
#include "IoInterface.h"
#include "ConnectorInterface.h"

#include "TCPSocket.h"

#include "LinearBuffer.h"

namespace light
{

class Tick;

} //namespace light

namespace light
{

namespace net
{

class Peer;

/**
@brief AsyncSelect로 Connector 기능을 제공하는 클래스
*/
class ConnectorAsyncSelect 
	: public AsyncSelectInterface
	, public TCPSocket
	, public ConnectorInterface
	, public IoInterface
{
public:
	///생성자
	ConnectorAsyncSelect(Peer* pPeer);

	///소멸자
	virtual ~ConnectorAsyncSelect();

	///데이터 전송 메소드
	virtual bool Send(unsigned int nLen, void* pData);

	///접속 해제 요청 메소드
	virtual void Disconnect(DWORD dwCode);

	///접속 중인지 여부
	virtual BOOL isConnected(){return m_Connect;}

	///AliveCheck메소드. 일정 시간동안 패킷이 도착하지 않았다면 서버와 연결을 끊는다.
	void AliveCheck();

	///접속 시도 메소드
	bool Connect(const char* szIP, PORT nPort);

private:
	///접속 해제 이벤트
	virtual void OnDisconnect();	

	///접속 완료 이벤트
	virtual void OnConnect();

	///Send 완료 이벤트
	virtual bool OnSend();

	///Recv 완료 이벤트
	virtual bool OnReceive();

	///완성된 패킷 처리 이벤트
	virtual bool OnReceive(unsigned int nLen);

	///소켓 재사용 요청시
	virtual void Reuse();

private:
	///받는 데에 사용하는 버퍼
	UniquePtr<LinearNormalBuffer> m_RecvBuffer;

	///보내는데에 사용하는 버퍼
	UniquePtr<LinearNormalBuffer> m_SendBuffer;

	///접속중인지 여부
	BOOL m_Connect;

	///AliveCheck용 틱
	UniquePtr<Tick> m_Tick;

};

} // Net

} // light
