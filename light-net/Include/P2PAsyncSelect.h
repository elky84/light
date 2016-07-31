#pragma once

#include "P2PCommon.h"
#include "P2PInterface.h"
#include "AsyncSelectInterface.h"

namespace light
{

namespace net
{

class Tick;
class P2PPeer;

class P2PAsyncSelect : public P2PInterface
    , public AsyncSelectInterface
{
public:
	P2PAsyncSelect();

	~P2PAsyncSelect();

	virtual void set(HWND hWnd, unsigned int wMsg = WM_SOCKET, long lEvent = FD_READ)
    {
        __super::set(hWnd, wMsg, lEvent);
    }

	virtual void on_receive(PacketHeader* header);
};

} // namespace net

} // namespace light
