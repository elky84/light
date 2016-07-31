#pragma once

#include <map>
#include "EnvironmentManager.h"
#include "singleton.h"
#include <memory>

#include "P2P.h"
#include "P2PRelayConnector.h"

#include "NetworkConfig.h"

class p2p_test : public light::EnvironmentManager
{
public:
	p2p_test(const std::wstring& program_name, const std::wstring& arg);
	~p2p_test();

	virtual bool initialize();
	virtual void on_update();
	virtual void on_complete();
    virtual void release();

private:
    std::unique_ptr<P2P> m_p2p;

    std::shared_ptr<light::net::P2PRelayConnector> m_RelayConnector;

	light::net::NetworkConfig m_Config;

private:
	/// 패킷 디스패쳐
	light::net::PACKET_DISPATCHER m_dispatcher;
};
