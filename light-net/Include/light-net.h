#pragma once

#include "light.h"

#include <memory>
#include "Dispatcher.hpp"

#include <winsock2.h>
#include <WS2tcpip.h>

namespace light
{

namespace net
{

typedef sockaddr_in SOCKET_ADDR_IN;
typedef sockaddr SOCKET_ADDR;

const int MAX_SEND_BUFFER_SIZE = 1024 * 100 * 2;
const int DEFAULT_SEND_BUFFER_SIZE = MAX_SEND_BUFFER_SIZE / 10;


const int MAX_RECEIVE_BUFFER_SIZE = 1024 * 100 * 2;
const int DEFAULT_RECEIVE_BUFFER_SIZE = MAX_RECEIVE_BUFFER_SIZE / 10;

const int ACCEPT_ADDRESS_LENGTH = sizeof(SOCKADDR_IN) + 16;

const int ACCEPT_BUFFER_SIZE = ACCEPT_ADDRESS_LENGTH * 2;

const int MAX_IP = 15 + 1;

typedef BYTE HEADER_SIZE;

typedef unsigned int PACKET_SIZE;

typedef unsigned short PACKET_SEQUENCE;

typedef unsigned short PORT;

typedef unsigned int PACKET_ID;

typedef unsigned int PACKET_INDEX;

class PacketStream;

typedef Dispatcher<PACKET_ID, PacketStream&> PACKET_DISPATCHER;

struct RESULT_CODE //소켓 종료 관련 코드
{
	enum Type
	{
		SUCCESS = 0,
		MIN = 100000,

		PACKET_BUFFER_FULL,
        OVERFLOW_SEND_COUNT,
        OVERFLOW_SEND_SIZE,

        OVERFLOW_RECEIVE_BUFFER,

		SEND_FAILED,

		RECEIVE_FAILED,

		WSASEND_IO_FAILED,

		WSARECV_IO_FAILED,

		GQCS,

		STOP_PROCESS, //프로그램 종료 프로세스 중이다

		ALIVE_CHECK_AS_DETECTED,

		WRONG_DATA, //데이터가 망가져있는 상태 (혹은 코딩이 잘못된 것)

		INVALID_DATA_ON_VERIFY, //상대 피어가 잘못된 데이터 전송시

		NOT_DEFINED_ACTION,	//거부된 동작

		CLOSE_ACTION, //직접 접속 해제 동작

		CONNECT_FAILED_IMMEDIATE,
        CONNECT_FAILED_WORKER,
        CONNECT_FAILED_SOCKET_ERROR,

        DISCONNECTING_COMPLETE,

        MEMORY_ALLOC_FAILED,
        SEND_BUFFER_WRITE_FAILED,
        RECEIVE_BUFFER_WRITE_FAILED,

        REUSE_FAILED,
        TRANSMITFILE_FAILED,
        INVALID_SOCKET_STATE,

        SHUTDOWN_FAILED,

		MAX = 200000,
	};
};


struct IO
{
	enum Type
	{
		SEND, // send IO 
		RECV, // recv IO
		CONNECT, // accept/connect IO
		REUSE, // reuse IO
		MAX,
	};
};


enum ERROR_CODE
{
    ERROR_NOTHING = 500000,
    ERROR_INVALID_SERVER_ID = 100,
    ERROR_INVALID_SIZE = 110,
    ERROR_INVALID_PARAMETER_COUNT = 112,
    ERROR_INVALID_PARAMETER_TYPE = 113,
    ERROR_INVALID_PARAMETER_SIZE_OVERFLOW = 115,
    ERROR_ALREADY_EXIST_COMMANDDESC = 201,
    ERROR_FAIL_CREATE_EVENT = 300,
    ERROR_FAIL_CREATE_THREAD = 301,
    ERROR_FAIL_COMPRESS_DECOMPRESS = 325,
    ERROR_NULL_DISTRIBUTER_POINTER = 401,
    ERROR_NULL_ARGUMENT_POINTER = 412,
    ERROR_NULL_COMMANDDESC_POINTER = 414,
    ERROR_NULL_PARAMETER_POINTER = 415,
    ERROR_NULL_PARAMETERDESC_POINTER = 416,

    ERROR_NULL_POINTER = 417,
    ERROR_NULL_PACKETCRYPTER_POINTER = 418,
    ERROR_FAIL_ENCRYPT_PACKET = 419,

    ERROR_NULL_PACKET_POINTER = 420,
    ERROR_INVALID_PACKET_TYPE = 421,
    ERROR_INVALID_PACKET_SIZE = 422,

    ERROR_FAIL_DECRYPT_PACKET = 430,

    ERROR_INVALID_PACKET_CHECKSUM = 440,

    ERROR_FAILED_TO_ALLOCATE_BUFFER,
    ERROR_INVALID_BUFFER_SIZE,

    ERROR_FAILED_TCP_PUSH_COMMAND,

    ERROR_NOT_CONNECT,
};

std::string get_external_ip(boost::asio::io_service io_service);

inline std::string get_local_ip()
{
    char name[128] = { 0, };
    if (gethostname(name, sizeof(name)) == 0)
    {
        struct addrinfo hints;
        struct addrinfo *result;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags |= AI_CANONNAME;

        if (getaddrinfo(name, NULL, &hints, &result) != 0)
            return "";

        while (result)
        {
            char addr4_str[20];
            void *ptr = NULL;
            switch (result->ai_family)
            {
            case AF_INET:
                ptr = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
                break;
            case AF_INET6:
                result = result->ai_next;
                continue;
            }

            inet_ntop(result->ai_family, ptr, addr4_str, sizeof(addr4_str));
            return addr4_str;
        }
    }
    return "";
}

inline std::wstring get_local_ip_w()
{
    return STRING_TO_WSTRING(get_local_ip());
}

inline bool ip_to_sock_addr(SOCKET_ADDR_IN& sockaddr, const char* ip, PORT port)
{
    unsigned long ulAddr = 0;
    sockaddr.sin_family = AF_INET;
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    if (getaddrinfo(ip, NULL, &hints, &result) != 0)
    {
        return false;
    }

    while (result)
    {
        char addr4_str[20];
        void *ptr = NULL;
        switch (result->ai_family)
        {
        case AF_INET:
            ptr = &((struct sockaddr_in *) result->ai_addr)->sin_addr;
            break;
        case AF_INET6:
            result = result->ai_next;
            continue;
        }

        inet_ntop(result->ai_family, ptr, addr4_str, sizeof(addr4_str));
        inet_pton(AF_INET, ip, &ulAddr);
        sockaddr.sin_addr.s_addr = ulAddr;
        sockaddr.sin_port = htons(static_cast<u_short> (port));
        memset(sockaddr.sin_zero, 0, sizeof(sockaddr.sin_zero));
        return true;
    }

    return false;
}

inline bool get_sock_addr(SOCKET_ADDR_IN& sockaddr, const char* ip, PORT port)
{
    if (strcmp(ip, "127.0.0.1") == 0 || strcmp(ip, "localhost") == 0)
    {
        return ip_to_sock_addr(sockaddr, get_local_ip().c_str(), port);
    }
    return ip_to_sock_addr(sockaddr, ip, port);
}

#pragma pack(1)
class PacketHeader
{
public:
	PacketHeader(const PacketHeader& header)
		: m_command(header.command())
		, m_size(header.get_size())
		, m_sequence(header.sequence())
        , m_index(0)
	{
	}

	PacketHeader(PACKET_ID command, PACKET_SIZE size = 0, PACKET_INDEX index = 0)
		: m_command(command)
		, m_size(size)
		, m_sequence(0)
        , m_index(index)
	{
	}

	PacketHeader()
		: m_command(0)
		, m_size(0)
		, m_sequence(0)
        , m_index(0)
	{
	}

	PACKET_ID command() const
	{
		return m_command;
	}

    void command(PACKET_ID cmd)
    {
        m_command = cmd;
    }

	PACKET_SIZE get_size() const
	{
		return m_size;
	}

	void set_size(PACKET_SIZE size)
	{
		m_size = size;
	}

    PACKET_SEQUENCE sequence() const
	{
		return m_sequence;
	}

	void sequence(PACKET_SEQUENCE seq)
	{
        m_sequence = seq;
	}

    PACKET_INDEX index() const
    {
        return m_index;
    }

    void index(PACKET_INDEX idx)
    {
        m_index = idx;
    }

    BYTE* data()
    {
        return (BYTE*)this + header_size();
    }

    static PACKET_SIZE header_size()
    {
        return sizeof(PacketHeader);
    }

    PACKET_SIZE data_size()
    {
        return get_size() - header_size();
    }

private:
	PACKET_ID m_command;

	PACKET_SIZE m_size;

    PACKET_INDEX m_index;
    
    PACKET_SEQUENCE m_sequence;
};

#pragma pack()

} // namespace net

} // namespace light
