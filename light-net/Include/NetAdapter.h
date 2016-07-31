#pragma once

#include <IPHlpApi.h>
#include <vector>
#include "Singleton.h"

namespace light
{

namespace net
{

class NetAdapter
{
private:
	NetAdapter();
	~NetAdapter();

public:

	inline BYTE* get_mac_address(PIP_ADAPTER_INFO adapterinfo)
	{
		return adapterinfo->Address;
	}

	inline char* get_primary_wins_ip_address(PIP_ADAPTER_INFO adapterinfo)
	{
		return adapterinfo->PrimaryWinsServer.IpAddress.String;
	}

	inline char* get_second_wins_ip_address(PIP_ADAPTER_INFO adapterinfo)
	{
		return adapterinfo->SecondaryWinsServer.IpAddress.String;
	}

	inline char* get_dhcp_server_ip_address(PIP_ADAPTER_INFO adapterinfo)
	{
		return adapterinfo->DhcpServer.IpAddress.String;
	}

	char* get_ipAddress(PIP_ADAPTER_INFO adapterinfo){
		return adapterinfo->IpAddressList.IpAddress.String;
	}

	char* get_adapter_name(PIP_ADAPTER_INFO adapterinfo){
		return adapterinfo->AdapterName;
	}

	char* get_desc(PIP_ADAPTER_INFO adapterinfo){
		return adapterinfo->Description;
	}

	char* get_mask(PIP_ADAPTER_INFO adapterinfo){
		return adapterinfo->IpAddressList.IpMask.String;
	}

	char* get_gateway(PIP_ADAPTER_INFO adapterinfo){
		return adapterinfo->GatewayList.IpAddress.String;
	}

	PIP_ADAPTER_INFO get_ipAdapterInfo(const char* ip); //통신에 쓰인 IP에 해당하는 PIP 정보 얻어온다.

	///싱글턴으로 선언
	STATIC_SINGLETON(NetAdapter);

private:
	std::unique_ptr<IP_ADAPTER_INFO> m_AdapterInfo;
	std::vector<PIP_ADAPTER_INFO> m_vectorIPAdapterInfo;
};

inline NetAdapter* GetNetAdapter()
{
	return NetAdapter::Instance();
}

} // namespace net

} // namespace light
