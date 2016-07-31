#include "light-curl.hpp"
#include "curl_helper.h"

#include "async_worker.h"

namespace light
{

namespace curl
{

helper::helper(const bool debug_mode)
: m_curl(NULL)
, m_header(NULL)
, m_ResponseData()
{
	m_curl = curl_easy_init();
	if (m_curl == NULL)
	{
        LOG_ERROR(L"curl_easy_init() failed.");
        return;
	}		

    if(debug_mode == true)
	{
		curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, &helper::debug_trace);
        
        HelperData config;
        curl_easy_setopt(m_curl, CURLOPT_DEBUGDATA, &config);
		
        curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
	}
}

helper::~helper(void)
{
	if(m_header)
	{
		curl_slist_free_all(m_header);
	}

	if(m_curl)
	{
		curl_easy_cleanup(m_curl);
	}

	m_ResponseData.clear();
}


bool helper::init_global(void)
{
	CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
	if(result != CURLE_OK)
	{
		LOG_ERROR(L"curl_global_init failed | code[%d]", result);
		return false;
	}

	return true;
}


bool helper::perform_http(
    CURLcode& curlCode,
    int& httpCode,
    helper::ResponseBuffer& res,
    const std::string& url,
    const std::string& header,
    const std::string& postfield,
    int port,
    int timeoutSec,
    bool isbinary
    )
{
    helper curl;
    if (curl.is_init() == false)
    {
        LOG_ERROR(L"curl_mod is not initialized!!");
        return false;
    }

    curl.set_option(CURLOPT_WRITEFUNCTION, &helper::data_write);
    curl.set_option(CURLOPT_FILE, &res);

    curl.set_option(CURLOPT_URL, url.c_str());

    curl.set_option(CURLOPT_NOPROGRESS, TRUE);
    curl.set_option(CURLOPT_FOLLOWLOCATION, TRUE);

    curl.set_option(CURLOPT_SSL_VERIFYPEER, FALSE);

    if (false == header.empty())
    {
        curl.add_header(header.c_str());
        curl.apply_header();
    }

    if (false == postfield.empty())
    {
        curl.set_option(CURLOPT_POST, TRUE);
        curl.set_option(CURLOPT_POSTFIELDS, postfield.c_str());
    }
    else
    {
        curl.set_option(CURLOPT_POST, FALSE);
    }

    if (0 <= port)
    {
        curl.set_option(CURLOPT_PORT, port);
    }

    if (INFINITE != timeoutSec)
    {
        curl.set_option(CURLOPT_CONNECTTIMEOUT, timeoutSec);
    }

    curlCode = curl.perform();

    if (curlCode != CURLE_OK)
    {
        LOG_ERROR
            (L"Error occurred on curl |"
                " url[%s]"
                " post[%s]"
                " curlCode[%d]"
                , url.c_str()
                , postfield.c_str()
                , curlCode
                );
        return false;
    }

    curl.get_info(CURLINFO_RESPONSE_CODE, &httpCode);

    if (httpCode != 200)
    {
        LOG_ERROR
            (L"Failed to perform on http |"
                " url[%s]"
                " post[%s]"
                " httpCode[%d]"
                , url.c_str()
                , postfield.c_str()
                , httpCode
                );

        return false;
    }

    if (isbinary == false)
    {
        res.put('\0');
    }

    return true;
}

bool helper::perform_http(
    helper::ResponseBuffer& res,
    const std::string& url,
    const std::string& header,
    const std::string& postfield,
    int port,
    int timeoutSec,
    bool isbinary
    )
{
    CURLcode	curlCode = CURL_LAST;
    int			httpCode = -1;

    return perform_http(curlCode, httpCode, res, url, header, postfield, port, timeoutSec, isbinary);
}


void PerformHTTPPostAsyncImpl(
    helper::async_callback cb_func,
    const std::string& url,
    const std::string& header,
    const std::string& postfield,
    int port,
    int timeoutSec,
    bool isbinary
    )
{
    CURLcode				curlCode = CURL_LAST;
    int						httpCode = -1;
    helper::ResponseBuffer	response;

    helper::perform_http(curlCode, httpCode, response, url, header, postfield, port, timeoutSec, isbinary);

    cb_func(curlCode, httpCode, response);
}


void helper::perform_http_async(
    async_worker& sp,
    async_callback cb_func,
    const std::string& url,
    const std::string& header,
    const std::string& postfield,
    int port,
    int timeoutSec,
    bool isbinary
    )
{
    sp.post_job(
        boost::bind(&PerformHTTPPostAsyncImpl, cb_func, url, header, postfield, port, timeoutSec, isbinary)
        );
}

CURLcode helper::perform(void)
{
	return curl_easy_perform(m_curl);
}

bool helper::is_init(void)
{
	return m_curl != NULL ? true : false;
}


void helper::add_header(const std::string& param)
{
	m_header = curl_slist_append(m_header, param.c_str());
}

void helper::clear_header(void)
{
	curl_slist_free_all(m_header);
	m_header = NULL;
}

void helper::apply_header(void)
{
	set_option(CURLOPT_HTTPHEADER, m_header);
}

std::string helper::uri_encode(const std::string& content)
{
	char* enc = curl_easy_escape(m_curl, content.c_str(), content.length());
    std::string ret = enc;
	curl_free(enc);
	return ret;
}

std::string helper::uri_decode(const std::string& content)
{
	int len = 0;
	char* enc = curl_easy_unescape(m_curl, content.c_str(), content.length(), &len);
    std::string ret = enc;
	curl_free(enc);
	return ret;
}

helper::ResponseData& helper::get_response_data(void)
{
	return m_ResponseData;
}


int helper::debug_trace(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
{
	UNREFERENCED_PARAMETER(handle);
	UNREFERENCED_PARAMETER(size);

	struct HelperData *config = (struct HelperData *)userp;

	UNREFERENCED_PARAMETER(config);

	const char *text = NULL;
	switch (type) 
	{
	case CURLINFO_TEXT:
		fprintf(stderr, "== Info: %s", data);

	case CURLINFO_HEADER_OUT:
		text = "=> Send header";
		break;

	case CURLINFO_DATA_OUT:
		text = "=> Send data";
		break;

	case CURLINFO_SSL_DATA_OUT:
		text = "=> Send SSL data";
		break;

	case CURLINFO_HEADER_IN:
		text = "<= Recv header";
		break;

	case CURLINFO_DATA_IN:
		text = "<= Recv data";
		break;

	case CURLINFO_SSL_DATA_IN:
		text = "<= Recv SSL data";
		break;

	default: /* in case a new one is introduced to shock us */ 
		return 0;
	}

	LOG_DEBUG(L"%S", text);
	return 0;
}

size_t helper::write_func(void *ptr, size_t size, size_t nmemb, helper *self)
{
	std::string resp((char *)ptr);
	size_t s = resp.find("X-");

	if (s != std::string::npos)
		self->get_response_data().push_back(resp);

	LOG_DEBUG(L"%S", resp.c_str());
	return size * nmemb;
}

size_t helper::data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream &os = *static_cast<std::ostream *>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return (size_t)len;
	}
	return 0;
}

} // namespace curl

} // namespace light