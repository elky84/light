#pragma once

#include "Socket.h"

namespace light
{

namespace net
{

/**
@brief AsyncSelect 인터페이스를 제공해주는 클래스.
*/
class AsyncSelectInterface
{
protected:
	///기본 소켓 윈도우 메시지 번호
	static const int WM_SOCKET = WM_USER + 5000;

public:
	///윈도우 통지 메시지를 받기 위해 등록하는 메소드
	virtual void set(HWND hWnd, unsigned int wMsg = WM_SOCKET, long lEvent = FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	///윈도우 메시지를 전달 받아, 어떠한 네트웍 이벤트가 발생했는지 전달 받는 메소드
	bool SocketProc(WPARAM wParam, LPARAM lParam);

protected:
	///생성자
	AsyncSelectInterface(const SOCKET& rSocket);

	///소멸자
	virtual ~AsyncSelectInterface();

	/**
	@brief Send 후 처리 메소드. 재정의를 유도한다
	@return 성공 여부
	*/
	virtual bool on_send()
    {
        return true;
    }

	/**
	@brief Receive 후 처리 메소드. 재정의를 유도한다
	@return 성공 여부
	*/
	virtual bool on_receive()
    {
        return false;
    }

	/**
	@brief Disconnect 후 처리 메소드. 재정의를 유도한다
	*/
	virtual void on_disconnect()
    {
    }

	/**
	@brief Connect 후 처리 메소드. 재정의를 유도한다
	*/
	virtual void on_connect()
    {
    }

private:
	///AsyncSelect로 사용할 소켓의 참조자.
	const SOCKET& m_rSocket;
};

} // namespace net

} // namespace light
