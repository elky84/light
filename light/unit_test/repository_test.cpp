#include "stdafx.h"
#include "Repository.h"

#include "Statistics.h"

enum class REPOSITORY : int
{
    TEST,
    NOT_USED,
};

TEST(Repository, Test)
{
    light::Repository<REPOSITORY, light::StatisticsMinMax<int> > repository;
    repository.add(REPOSITORY::TEST, new light::StatisticsMinMax<int>());

    auto statistics = repository.get(REPOSITORY::TEST);
    ASSERT_NE(statistics.get(), nullptr);

    ASSERT_EQ(repository.get(REPOSITORY::NOT_USED).get(), nullptr);

    static const int min = 64;
    static const int max = 6464;

    statistics->record(min);
    statistics->record(max);

    ASSERT_EQ(statistics->minimum(), min);
    ASSERT_EQ(statistics->maximum(), max);

    ASSERT_EQ(statistics->count(), 2);
}