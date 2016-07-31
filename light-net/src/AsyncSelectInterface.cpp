#include "lightNet.h"
#include "AsyncSelectInterface.h"

namespace light
{

namespace net
{

/**
@brief 생성자. 참조자 세팅만 해준다
*/
AsyncSelectInterface::AsyncSelectInterface(const SOCKET& rSocket) 
: m_rSocket(rSocket)
{
}

/**
@brief 소멸자. 아무일도 안함.
*/
AsyncSelectInterface::~AsyncSelectInterface()
{
}

/**
@brief 윈도우 메시지로 비동기 네트웍 이벤트를 받기 위해, 설정.
@param hWnd 윈도우 핸들
@param wMsg 윈도우 메시지
@param lEvent 받아올 이벤트 종류
*/
void AsyncSelectInterface::set(HWND hWnd, unsigned int wMsg, long lEvent)
{
	WSAAsyncSelect(m_rSocket, hWnd, wMsg, lEvent);
}

/**
@brief 네트웍 이벤트가 발생했을 때 그 것을 받아 처리하는 메소드
@param wParam 발생한 소켓
@param lParam 발생한 네트웍 이벤트
*/
bool AsyncSelectInterface::on_socket_event(WPARAM wParam, LPARAM lParam)
{
	if(wParam != m_rSocket)
	{
		LOG_INFO(_T("%s [%d != %d]"), __FUNCTIONW__, wParam, m_rSocket);
		return false;
	}

	if(WSAGETSELECTERROR(lParam))
	{
		OnDisconnect();
		return false;
	}

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		OnConnect();
		break;
	case FD_READ:
		if(!OnReceive())
		{
			OnDisconnect();
			return false;
		}
		break;
	case FD_WRITE:
		if(!on_send())
		{
			OnDisconnect();
			return false;
		}
		break;
	case FD_CLOSE:
		OnDisconnect();
		break;
	}
	return true;
}

} // namepsace Net

} // namespace light
