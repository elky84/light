#pragma once

#include "light-net.h"

enum class ACTION_TYPE
{
    NONE = -1,
    CHANNEL_BROADCAST = 0, // 채널까지 돌입 한 후, 브로드 캐스트!
};