#include "light-net.h"

#include "NetStat.h"

#include "Statistics.h"

namespace light
{

namespace net
{

std::unique_ptr< Repository<STAT, Statistics<int> > > NetStat::m_repository;

void NetStat::initialize()
{
    m_repository.reset(new Repository<STAT, Statistics<int> >());
    m_repository->add(STAT::ON_RECEIVE, new StatisticsMinMax<int>());
    m_repository->add(STAT::SEND, new StatisticsMinMax<int>());
    m_repository->add(STAT::ON_SEND, new StatisticsMinMax<int>());

    m_repository->add(STAT::ACCEPT, new StatisticsSuccess<int>());
    m_repository->add(STAT::CONNECT_FAILED, new StatisticsSuccess<int>());

    m_repository->add(STAT::CONNECT, new StatisticsSuccess<int>());
    m_repository->add(STAT::ON_CONNECT, new StatisticsSuccess<int>());

    m_repository->add(STAT::DISCONNECT, new StatisticsSuccess<int>());
    m_repository->add(STAT::ON_DISCONNECT, new StatisticsSuccess<int>());
}

void NetStat::release()
{
    m_repository->clear();
    m_repository.reset();
}


} // namespace net

} // namespace light