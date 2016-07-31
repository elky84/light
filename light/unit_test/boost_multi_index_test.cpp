#include "stdafx.h"

#include "ObjectPool.h"

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/tag.hpp> 
#include <boost/multi_index/indexed_by.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/member.hpp> 
#include <boost/multi_index/composite_key.hpp> 

struct TestObject
{
public:
	TestObject()
		: m_Value(0)
		, m_Name(_T(""))
	{
	}

	TestObject(const int val, const std::wstring& name)
		: m_Value(val)
		, m_Name(name)
	{
	}

	int Value() const
	{
		return m_Value;
	}

	std::wstring Name() const
	{
		return m_Name;
	}

private:
	int m_Value;

	std::wstring m_Name;
};

class TestMultiIndex
{
private:
	typedef boost::multi_index::const_mem_fun<TestObject, int,	&TestObject::Value>			idx_value;
	typedef boost::multi_index::const_mem_fun<TestObject, std::wstring,	&TestObject::Name>	idx_name;
	typedef boost::multi_index::composite_key<TestObject, idx_value, idx_name>				idx_composite;

	typedef struct indices : public boost::multi_index::indexed_by
		<
		boost::multi_index::ordered_non_unique<idx_value>,
		boost::multi_index::ordered_non_unique<idx_name>,
		boost::multi_index::ordered_unique<idx_composite>
		>
	{
		enum type
		{
			IDX_VALUE		= 0,
			IDX_NAME		= 1,
			IDX_COMPOSITE	= 2,

			IDX_END
		};
	};

	// definition container
	typedef boost::multi_index_container<TestObject*, indices>		containter;
	typedef containter::iterator									iter;
	typedef std::pair<iter, bool>									pair_value;

public:
	TestMultiIndex(void)
	{

	}

	virtual ~TestMultiIndex(void)
	{

	}

public:
	bool TestMultiIndex::insert(const TestObject& obj)
	{
		TestObject* pObject = m_Pool.malloc();
		*pObject = obj;

		pair_value pair = m_Container.insert(pObject);
		if (pair.second == false)
		{
			LOG_ERROR(_T("Cannot insert object | Value[%d] Name[%s]"), obj.Value(), obj.Name().c_str());
			m_Pool.free(pObject);
			return false;
		}

		return true;
	}

	bool TestMultiIndex::erase(IN const int val)
	{
		static const indices::type n = indices::IDX_VALUE;

		containter::nth_index<n>::type& indexBy = m_Container.get<n>();
		containter::nth_index_iterator<n>::type it = indexBy.find(val);
		if (indexBy.end() == it)
			return false;

		m_Pool.free(*it);
		m_Container.erase(it);
		return true;
	}

	const TestObject* TestMultiIndex::find( IN const int val, IN const std::wstring& name )
	{
		static const indices::type n = indices::IDX_COMPOSITE;

		containter::nth_index<n>::type& indexBy = m_Container.get<n>();
		containter::nth_index_iterator<n>::type it = indexBy.find(boost::make_tuple(val, name));
		if (indexBy.end() == it)
		{
			LOG_DEBUG(_T("not find item | Value[%d] Name[%s]"), val, name.c_str());
			return NULL;
		}

		return *it;
	}

	const TestObject* TestMultiIndex::find( IN const int val )
	{
		static const indices::type n = indices::IDX_VALUE;

		containter::nth_index<n>::type& indexBy = m_Container.get<n>();
		containter::nth_index_iterator<n>::type it = indexBy.find(val);
		if (indexBy.end() == it)
		{
			LOG_DEBUG(_T("not find item | Value[%d]"), val);
			return NULL;
		}

		return *it;
	}

	const TestObject* TestMultiIndex::find( IN const std::wstring& name )
	{
		static const indices::type n = indices::IDX_NAME;

		containter::nth_index<n>::type& indexBy = m_Container.get<n>();
		containter::nth_index_iterator<n>::type it = indexBy.find(name);
		if (indexBy.end() == it)
		{
			LOG_DEBUG(_T("not find | Name[%s]"), name.c_str());
			return NULL;
		}

		return *it;
	}

private:
	containter m_Container;

	light::ObjectPool<TestObject> m_Pool;
};

const int TestValue = 10;
const std::wstring TestName = _T("Test10");

const int TestValue_Additional = 15;
const std::wstring TestName_Additional = _T("Test15");

class MultiIndexFixture 
	: public testing::Test
{
protected:
	virtual void SetUp(void)
	{
		TestObject obj(TestValue, TestName);
		m_MultiIndex.insert(obj);
	}

	virtual void TearDown(void)
	{
	}

public:
	TestMultiIndex m_MultiIndex;
};

TEST_F(MultiIndexFixture, HaveToFail_UniqueKey_DuplicateInsert)
{
	TestObject obj(TestValue, TestName);
	ASSERT_FALSE(m_MultiIndex.insert(obj));
}

TEST_F(MultiIndexFixture, NonUniqueKey_Value_DuplicateInsert)
{
	TestObject obj(TestValue_Additional, TestName);
	ASSERT_TRUE(m_MultiIndex.insert(obj));
}

TEST_F(MultiIndexFixture, NonUniqueKey_Name_DuplicateInsert)
{
	TestObject obj(TestValue,TestName_Additional);
	ASSERT_TRUE(m_MultiIndex.insert(obj));
}

TEST_F(MultiIndexFixture, UniqueKey_Additional_Insert)
{
	TestObject obj(TestValue_Additional, TestName_Additional);
	ASSERT_TRUE(m_MultiIndex.insert(obj));
}

TEST_F(MultiIndexFixture, CompositeKey_find)
{
	ASSERT_TRUE(NULL != m_MultiIndex.find(TestValue, TestName));
}

TEST_F(MultiIndexFixture, ValueKey_find)
{
	ASSERT_TRUE(NULL != m_MultiIndex.find(TestValue));
}

TEST_F(MultiIndexFixture, NameKey_find)
{
	ASSERT_TRUE(NULL != m_MultiIndex.find(TestName));
}

TEST_F(MultiIndexFixture, ValueKey_erase)
{
	ASSERT_TRUE(m_MultiIndex.erase(TestValue));
}
