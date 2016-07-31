#include "stdafx.h"
#include "curl_helper.h"

TEST(curl_helper, Test)
{
    light::curl::helper helper;

    light::curl::helper::ResponseBuffer response;

    std::string strAddress = "http://url.com";

    ASSERT_TRUE(helper.perform_http(response, strAddress, "", "", -1, 30) );
}