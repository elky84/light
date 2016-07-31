#include "lightNet.h"
#include "ConnectorAsyncSelect.h"
#include "Tick.h"
#include "Peer.h"

namespace light
{

namespace net
{

/**
@brief 생성자. IO를 담당할 피어를 전달 받는다.
@param pPeer IO를 담당할 피어
*/
ConnectorAsyncSelect::ConnectorAsyncSelect(Peer* pPeer) 
: AsyncSelectInterface(m_Socket)
, IoInterface(pPeer)
, m_Connect(false)
, m_Tick(new Tick())
, m_RecvBuffer(new LinearNormalBuffer(MAX_RECV_BUFFER_SIZE))
, m_SendBuffer(new LinearNormalBuffer(MAX_SEND_BUFFER_SIZE))
{
}

/**
@brief 소멸자. 자원을 해제한다.
*/
ConnectorAsyncSelect::~ConnectorAsyncSelect()
{
}

/**
@brief 접속을 해제하는 메소드. 소켓을 닫는다.
@param dwCode 접속 해제 코드
@desc 이후 OnDisconnect 신호가 온다.
*/
void ConnectorAsyncSelect::Disconnect(DWORD dwCode)
{
	OnDisconnect();
	LOG_INFO(_T("%s [%p] [%d]"), __FUNCTIONW__, this, dwCode);
}

/**
@brief 서버 접속 메소드
@param szIP 접속할 서버 IP
@param nPort 접속할 서버 포트
@return 접속 결과
*/
bool ConnectorAsyncSelect::Connect(const char* szIP, PORT nPort)
{
	if(m_Connect)
	{
		LOG_INFO(_T("%s Already Connect Server"), __FUNCTIONW__);
		return false;
	}

	int value, length;
	length = sizeof(value);
	if(getsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&value ,&length) < 0)
	{
		LOG_ERROR(_T("%s getsockopt() SO_RCVBUF fail"), __FUNCTIONW__);
		return false;
	}

	if(getsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&value ,&length) < 0)
	{
		LOG_ERROR(_T("%s getsockopt() SO_SNDBUF fail"), __FUNCTIONW__);
		return false;
	}

	BOOL bNoDelay = TRUE;
	int nRet = ::setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char FAR* )&bNoDelay, sizeof(bNoDelay));
	if(nRet == SOCKET_ERROR)
	{
		LOG_ERROR(_T("%s setsockopt() Error [%d]"), __FUNCTIONW__, nRet);
		return false;
	}

	int size = MAX_SEND_BUFFER_SIZE;
	setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
	setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));

	m_SockAddrIn = GetSockAddr(szIP, nPort);
	m_Peer->SetSockAddrIn(m_SockAddrIn);
	if(!ConnectorInterface::Connect(m_Socket, &m_SockAddrIn))
		return false;

	OnConnect();
	return true;
}

/**
@brief Send 완료 이벤트
@return 처리 결과
@desc connect 되고 난 후에 자동으로 한번 호출된다.
*/
bool ConnectorAsyncSelect::OnSend() 
{
	unsigned int nRemainSendSize = m_SendBuffer->Length();
	if (send(m_Socket, m_SendBuffer->HeadBuffer(), nRemainSendSize, 0) != SOCKET_ERROR)
	{
		m_SendBuffer->JumpHead(nRemainSendSize);
		m_SendBuffer->Compress();
		return true;
	}
	else 
	{
		if(GetLastError() == WSAEWOULDBLOCK) 
			return true;

		DWORD err = GetLastError();
		LOG_ERROR(_T("%s [%d]"), __FUNCTIONW__, err);
		Disconnect(RESULT_CODE::SEND_FAILED);
		return false;
	}
}

/**
@brief Receive 완료 이벤트
@return 처리 결과
@desc 내부적으로 OnReceive함수 호출을 통해 데이터를 즉시 처리한다.
*/
bool ConnectorAsyncSelect::OnReceive()
{
	unsigned int size = recv(m_Socket, m_RecvBuffer->TailBuffer(), m_RecvBuffer->RemainLength(), 0);
	if(SOCKET_ERROR == size) 
		return false;

	m_Tick->reset(); //패킷이 왔으면 어떤 패킷이든간에 오케이~
	return OnReceive(m_RecvBuffer->Length());
}

/**
@brief 패킷 처리 이벤트.
@param nLen Receive온 패킷 크기
@return 처리 결과
@desc 패킷을 분석하는 처리도 한다. 그리고 큐잉하지 않고 바로 처리한다.
*/
bool ConnectorAsyncSelect::OnReceive(unsigned int nLen)
{
	if(false == m_RecvBuffer->JumpTail(nLen)) 
		return false;

	while(0 < m_RecvBuffer->Length())
	{
		int nPacketLen = m_Peer->Verify(m_RecvBuffer->Length(), m_RecvBuffer->HeadBuffer());
		if(nPacketLen == 0)
			break;

		m_Peer->OnReceive(nPacketLen, m_RecvBuffer->HeadBuffer());
		m_RecvBuffer->JumpHead(nPacketLen);
	}
	m_RecvBuffer->Compress();
	return true;
}

/**
@brief 접속 완료 이벤트
*/
void ConnectorAsyncSelect::OnConnect()
{
	m_Tick->reset();
	m_Connect = true;
	if(m_Peer.get())
		m_Peer->OnConnect();

	LOG_INFO(_T("%s Success."), __FUNCTIONW__);
}

/**
@brief 접속 해제 이벤트
*/
void ConnectorAsyncSelect::OnDisconnect()
{
	if(!isConnected())
		return;

	m_Connect = false;
	SetRefreshing(TRUE);

	LOG_INFO(_T("%s, %d"), __FUNCTIONW__, WSAGetLastError());
	if(m_Peer.get())
		m_Peer->OnDisconnect();

	m_RecvBuffer->Init();
	m_SendBuffer->Init();

	if(m_Socket != INVALID_SOCKET)
		closesocket(m_Socket);

	Socket();
}

/**
@brief 서버에서 일정 시간 내에 패킷이 도달했는지 확인하고, 도달하지 않았으면 접속을 해제한다.
*/
void ConnectorAsyncSelect::AliveCheck()
{
	if(!isConnected())
		return; 

	if(!m_Tick->Check(GetTickCount()))
	{
		LOG_INFO(_T("%s Check() False"), __FUNCTIONW__);
		Disconnect(RESULT_CODE::ALIVE_CHECK_AS_DETECTED);
	}
}

/**
@brief 데이터를 서버로 전달한다.
@param nLen pData의 크기
@param pData 기록할 데이터
@return 전송 결과
*/
bool ConnectorAsyncSelect::Send(unsigned int nLen, void* pData)
{
	if(!isConnected())
		return false;

	if (m_SendBuffer->Write(nLen, pData))  //버퍼 뒤에 데이터를 추가하자
	{ 
		OnSend();	//OnSend를 직접 불러주어야만 버퍼에 있는 데이터를 전송한다
		return true;
	}
	else 
	{
		return false;
	}
}

/**
@brief 소켓 재사용 가능시 호출하는 메소드
*/
void ConnectorAsyncSelect::Reuse()
{
	SetRefreshing(FALSE);
}

} // Net

} // light
