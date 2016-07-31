#pragma once

#include "light-net.h"


enum class ACTION_TYPE
{
    NONE = -1,
    TRY_CONNECT_DISCONNECT = 0,
    SEND_RANDOM_PACKET = 1,
    SEND_HEAVY_PACKET = 2,
    SEND_LIGHT_PACKET = 3,
};