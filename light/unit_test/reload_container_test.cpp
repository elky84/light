#include "stdafx.h"
#include "ReloadContainer.h"

#include <map>
#include <list>
#include <vector>
#include <deque>
#include <set>

static const int TEST_DATA_SIZE = 10;

TEST(ReloadContainer, MapTest)
{
	typedef std::map<int, int> CONTAINER;

	light::ReloadMappedContainer<CONTAINER> Reload;

	/// insert만 했을 때는 find 해도 원하는 엘리먼트가 찾아지지 않아야함. load중인 상태이기 때문
	for( int i = 0; i < TEST_DATA_SIZE; ++i)
		Reload.insert(i, i);

	/// deploy전까지는 데이터가 없음을검증한다.
	ASSERT_TRUE(Reload.empty());

	int n = 0, key = 5;
	ASSERT_FALSE(Reload.find(key, n));

	ASSERT_EQ(n, 0);
	ASSERT_FALSE(Reload[key]);

	/// 실제 컨테이너에 옮겨 담는 동작
	Reload.deploy();

	/// deploy 후에는 데이터가 찾아져야 함을 검증한다.
	ASSERT_TRUE(Reload.find(key, n));
	ASSERT_EQ(n, 5);

	ASSERT_EQ(TEST_DATA_SIZE, Reload.size());
}

TEST(ReloadContainer, SetTest)
{
	typedef std::set<int> CONTAINER;

	light::ReloadContainer<CONTAINER> Reload;

	/// insert만 했을 때는 find 해도 원하는 엘리먼트가 찾아지지 않아야함. load중인 상태이기 때문
	for( int i = 0; i < TEST_DATA_SIZE; ++i)
		Reload.insert(i);

	/// deploy전까지는 데이터가 없음을검증한다.
	ASSERT_TRUE(Reload.empty());

	int val = 5;
	ASSERT_FALSE(Reload.find(val));

	/// 실제 컨테이너에 옮겨 담는 동작
	Reload.deploy();

	/// deploy 후에는 데이터가 찾아져야 함을 검증한다.
	ASSERT_TRUE(Reload.find(val));

	ASSERT_EQ(TEST_DATA_SIZE, Reload.size());
}

TEST(ReloadContainer, ListTest)
{
	typedef std::list<int> CONTAINER;

	light::ReloadContainer<CONTAINER> Reload;

	/// insert만 했을 때는 find 해도 원하는 엘리먼트가 찾아지지 않아야함. load중인 상태이기 때문
	for( int i = 0; i < TEST_DATA_SIZE; ++i)
		Reload.push_back(i);

	/// deploy전까지는 데이터가 없음을검증한다.
	ASSERT_TRUE(Reload.empty());

	/// 실제 컨테이너에 옮겨 담는 동작
	Reload.deploy();

	/// deploy 후에는 데이터가 찾아져야 함을 검증한다.
	ASSERT_EQ(TEST_DATA_SIZE, Reload.size());

	const CONTAINER& container = Reload;
	ASSERT_EQ(container.size(), Reload.size());
}

TEST(ReloadContainer, DequeTest)
{
	typedef std::deque<int> CONTAINER;

	light::ReloadContainer<CONTAINER> Reload;

	/// insert만 했을 때는 find 해도 원하는 엘리먼트가 찾아지지 않아야함. load중인 상태이기 때문
	for( int i = 0; i < TEST_DATA_SIZE; ++i)
		Reload.push_back(i);

	/// deploy전까지는 데이터가 없음을검증한다.
	ASSERT_TRUE(Reload.empty());

	int n = 0, val = 5;
	ASSERT_FALSE(Reload.at(val, n));
	ASSERT_EQ(n, 0);

	/// 실제 컨테이너에 옮겨 담는 동작
	Reload.deploy();

	/// deploy 후에는 데이터가 찾아져야 함을 검증한다.
	ASSERT_TRUE(Reload.at(val, n));
	ASSERT_EQ(n, 5);

	ASSERT_EQ(TEST_DATA_SIZE, Reload.size());
}

TEST(ReloadContainer, VectorTest)
{
	typedef std::vector<int> CONTAINER;

	light::ReloadContainer<CONTAINER> Reload;

	/// insert만 했을 때는 find 해도 원하는 엘리먼트가 찾아지지 않아야함. load중인 상태이기 때문
	for( int i = 0; i < TEST_DATA_SIZE; ++i)
		Reload.push_back(i);

	/// deploy전까지는 데이터가 없음을검증한다.
	ASSERT_TRUE(Reload.empty());

	int n = 0, val = 5;
	ASSERT_FALSE(Reload.at(val, n));
	ASSERT_EQ(n, 0);

	/// 실제 컨테이너에 옮겨 담는 동작
	Reload.deploy();

	/// deploy 후에는 데이터가 찾아져야 함을 검증한다.
	ASSERT_TRUE(Reload.at(5, n));
	ASSERT_EQ(n, 5);

	ASSERT_EQ(TEST_DATA_SIZE, Reload.size());
}

