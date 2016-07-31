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
            JOIN = 10000,
            LEAVE,
            SPAWN,
            READY,
            SEND,
        };
    }

    namespace S2C
    {
        enum type
        {
            JOIN = 20000,
            JOINED,
            LEAVE,
            SPAWN,
            READY,
            SEND,
        };
    }
}