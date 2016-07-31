#include "stdafx.h"
#include "server_peer.h"

server_peer::server_peer()
{
}

server_peer::~server_peer()
{
}

void server_peer::on_connect()
{
	LOG_INFO(L"called");
}

void server_peer::on_disconnect()
{
	LOG_INFO(L"called");
	reuse();
}

void server_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
	LOG_INFO(L"[%d] [%p]", len, ptr);
}

void server_peer::on_update()
{
	static int i = 0;

	WCHAR buf[512] = {0, };
	wprintf_s(buf, L"%s %d", L"Test", i++);

	send((int)wcslen(buf) + 1, buf);
}

light::net::PACKET_SIZE server_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
	UNREFERENCED_PARAMETER(ptr);
	return len;
}
