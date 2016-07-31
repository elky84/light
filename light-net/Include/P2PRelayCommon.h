#pragma once

#include "P2PCommon.h"

namespace light
{

namespace net
{

const int MAX_AUTH_KEY = 128 + 1;

#pragma pack(1)

namespace R2P
{
	enum COMMAND
	{
		AUTH_KEY = 6000,		// 인증키
		AUTH_INFORMATION,		// 인증 정보
		PEERINFORMATION,		// 피어 정보 수신
		DISCONNECT_PEER,		// 피어와의 연결 종료 알림
		PING,					// 핑
		RELAY,
	};

	struct AuthKey
	{
		char m_szHashKey[MAX_AUTH_KEY];
		AuthKey(char* szHashKey)
		{
			strcpy_s(m_szHashKey, _countof(m_szHashKey), szHashKey);
		}

		AuthKey()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct AuthInformation
	{
		int m_idx;
		AuthInformation(int idx)
			: m_idx(idx)
		{

		}

		AuthInformation()
			: m_idx(0)
		{

		}
	};

	struct DisconnectPeer
	{
		int m_idx;

		DisconnectPeer()
			: m_idx(0)
		{

		}

		DisconnectPeer(int idx) 
			: m_idx(idx)
		{

		}
	};

} // namespace R2P


namespace P2R
{
	enum COMMAND
	{
		ENTER = 7000,		// 그룹 합류
		EXIT,				// 그룹 퇴장
		PING,				// 핑
		RELAY,
	};

	struct lock
	{
		int m_GroupNo;
		lock(int group_no) 
			: m_GroupNo(group_no)
		{

		}

		lock()
			: m_GroupNo(0)
		{

		}
	};

} // namespace P2R

#pragma pack()

} // namespace net

} // namespace light
