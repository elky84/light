#pragma once

#include "Repository.h"
#include "Statistics.h"

namespace light
{

namespace net
{

enum class STAT : int
{
    ON_RECEIVE,
    SEND,
    ON_SEND,
    ACCEPT,
    CONNECT_FAILED,
    CONNECT,
    ON_CONNECT,
    DISCONNECT,
    ON_DISCONNECT,
};

class NetStat
{
public:
    static void initialize();

    static void release();

    template <typename T>
    static void write(T t)
    {
        m_repository->each([&](auto it) { t(it.second->to_log()); });
    }

public:
    static std::unique_ptr < Repository<STAT, Statistics<int> > > m_repository;
};

} // namespace net

} // namespace light