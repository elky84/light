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

void server_peer::on_update()
{
	static int i = 0;
	char str[12] = {0, };
	sprintf_s(str, "Seq: %d", ++i);
	_Post(strlen(str) + 1, str); //널까지 전송하려 + 1
}

void server_peer::_Post(light::net::PACKET_SIZE len, const void* ptr)
{
	__super::send(len, ptr);
}