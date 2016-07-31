#pragma once

#include <WinSock2.h>
#include <curl/curl.h>

#include "boost/function.hpp"

namespace light
{

class async_worker;

namespace curl
{

struct HelperData 
{
	char trace_ascii; /* 1 or 0 */ 
};

class helper
{
public:
	typedef std::vector<std::string> ResponseData;
    
    typedef std::ostringstream ResponseBuffer;

public:
    typedef boost::function<void(CURLcode, int, const ResponseBuffer&)> async_callback;

public:
	helper(const bool debug_mode = false);
	~helper(void);

	static bool init_global(void);

	bool is_init(void);

	void add_header(const std::string& param);
	void clear_header(void);
	void apply_header(void);

	template<typename OPTION, typename PARAM>
	void set_option(OPTION option, PARAM param)
	{
		curl_easy_setopt(m_curl, option, param);
	}

	template<typename INFO, typename ARG>
	void get_info(INFO option, ARG arg)
	{
		curl_easy_getinfo(m_curl, option, arg);
	}

	std::string uri_encode(const std::string& content);
	std::string uri_decode(const std::string& content);

	CURLcode perform(void);

	ResponseData& get_response_data(void);

    static bool perform_http(CURLcode& curlCode,
            int& httpCode,
            ResponseBuffer& res,
            const std::string& url,
            const std::string& header,
            const std::string& postfield,
            int port = -1,
            int timeoutSec = 5,
            bool isbinary = false
            );

    static bool perform_http(ResponseBuffer& res,
            const std::string& url,
            const std::string& header,
            const std::string& postfield,
            int port = -1,
            int timeoutSec = 5,
            bool isbinary = false
            );

    static void perform_http_async(async_worker& sp,
            async_callback cb_func,
            const std::string& url,
            const std::string& header,
            const std::string& postfield,
            int port = -1,
            int timeoutSec = 5,
            bool isbinary = false
            );


public :
	static int debug_trace(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);

	static size_t write_func(void *ptr, size_t size, size_t nmemb, helper *self);

	static size_t data_write(void* buf,	size_t size, size_t nmemb, void* userp);

private:
    CURL* m_curl;

	curl_slist* m_header;

	ResponseData m_ResponseData;
};

} // namespace curl

} // namespace light