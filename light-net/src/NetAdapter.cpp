#include "light-net.h"
#include "NetAdapter.h"

#pragma comment(lib, "Iphlpapi.lib")

namespace light
{

namespace net
{

NetAdapter::NetAdapter()
{
	DWORD dwRetVal = 0;
	IP_ADAPTER_INFO* pAdapter = NULL;
	m_AdapterInfo.reset((IP_ADAPTER_INFO* ) malloc(sizeof(IP_ADAPTER_INFO)));
	unsigned long ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( m_AdapterInfo.get(), &ulOutBufLen) != ERROR_SUCCESS) 
	{
		m_AdapterInfo.reset();
		m_AdapterInfo.reset((IP_ADAPTER_INFO*) malloc(ulOutBufLen));
	}

	if ((dwRetVal = GetAdaptersInfo(m_AdapterInfo.get(), &ulOutBufLen)) == NO_ERROR) 
	{
		LOG_DEBUG(L"GetAdapterInfo Success");
		pAdapter = m_AdapterInfo.get();
		while(pAdapter)
		{
			m_vectorIPAdapterInfo.push_back(pAdapter);
			pAdapter = pAdapter->Next;
		}
	}
	else 
	{
		LOG_ERROR(L"call to GetAdaptersInfo failed.");
	}
}

NetAdapter::~NetAdapter()
{
	
}

PIP_ADAPTER_INFO NetAdapter::get_ipAdapterInfo(const char* ip)
{
	for(unsigned int i = 0; i < m_vectorIPAdapterInfo.size(); i++)
	{
		if(_stricmp(get_ipAddress(m_vectorIPAdapterInfo[i]), ip) == 0)
		{
			return m_vectorIPAdapterInfo[i];
		}
	}
	return NULL;
}

} // namespace net

} // namespace light