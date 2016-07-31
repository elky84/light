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
	LOG_INFO(_T("%S"), get_ip().c_str());

}

void server_peer::on_disconnect()
{
	LOG_INFO(L"called");
	reuse();
}


void server_peer::on_update()
{
}

void server_peer::on_receive(light::net::PACKET_SIZE len, const void* ptr)
{
    LOG_INFO(_T("received msg [%s, %d]"), ptr, len);
}


// native 패킷도 받기 위해서
light::net::PACKET_SIZE server_peer::verify(light::net::PACKET_SIZE len, const void* ptr)
{
    UNREFERENCED_PARAMETER(ptr);
    return len;
}