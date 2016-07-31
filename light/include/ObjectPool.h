#pragma once

#pragma warning(push)
#pragma warning(disable:4127)
#include	<boost/pool/object_pool.hpp>
#pragma warning(pop)

namespace light
{

template<typename T>
class ObjectPool
{
private:
	typedef boost::object_pool<T> object_pool;


public:
	ObjectPool(void) 
		: m_ool()
	{

	}

	~ObjectPool(void)
	{
	}


public:
	inline T* malloc(void)
	{
		T *obj = m_ool.construct();
		if (obj == NULL)
		{
			LOG_ERROR(L"Not enough system memory");
			_ASSERT(false);
		}

		return obj;
	}


	inline bool	 free(T *obj)
	{
		if (is_from(obj) == false)
		{
			LOG_ERROR(L"Object is not acquired from this pool[%p] | object[%p]", &m_ool, obj);
			_ASSERT(false);
			return false;
		}
		else
		{
			m_ool.destroy(obj);
			return true;
		}
	}


	inline bool	is_from(T * obj)
	{
		_ASSERT(obj != NULL);
		return m_ool.is_from(obj);
	}

private:
	object_pool m_ool;
};

} // namespace light