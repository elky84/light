#pragma once

#include "light-net.h"

#include "FlatBuffersPtr.h"
#include "packet_generated.h"

#include "LinearBuffer.h"

namespace PACKET_NUMBER
{
    namespace C2S
    {
        enum type
        {
            ENTER = 10000,
            LEAVE,
            BROADCAST,
        };
    }

    namespace S2C
    {
        enum type
        {
            ENTER = 20000,
            LEAVE,
            BROADCAST,
        };
    }
}