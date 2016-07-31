#include "stdafx.h"
#include "client_peer.h"

client_peer::client_peer()
{
}

client_peer::~client_peer()
{
}

void client_peer::on_connect()
{
	LOG_INFO(L"called");
}

void client_peer::on_disconnect()
{
	LOG_INFO(L"called");
	reuse();
}

void client_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
	LOG_INFO(L"[%d] [%p]", len, ptr);
	send(len, ptr);
}

void client_peer::on_update()
{
}

light::net::PACKET_SIZE client_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
	UNREFERENCED_PARAMETER(ptr);
	return len;
}
