#include "stdafx.h"
#include "client_peer.h"

client_peer::client_peer(const int& idx) 
	: PeerEx(idx)
{
}

client_peer::~client_peer()
{
}

void client_peer::on_connect()
{
	LOG_INFO(L"[%d]", get_idx());
}

void client_peer::on_disconnect()
{
	LOG_INFO(L"[%d]", get_idx());
	reuse();
}

void client_peer::on_update()
{
}

void client_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
    send(len, ptr);
}

// native 패킷도 받기 위해서
light::net::PACKET_SIZE client_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(ptr);
    return len;
}