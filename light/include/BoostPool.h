#pragma once

#include "spin_lock.h"

#pragma warning ( push )
#pragma warning ( disable : 4800 4127)
#include <boost/pool/singleton_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#pragma warning ( pop )

namespace light
{

struct default_pool_tag {};

template<typename T, class _Lock=spin_lock, class _Allocator=boost::default_user_allocator_new_delete, class Tag=default_pool_tag>
class BoostPool
{
private:
	typedef boost::singleton_pool<Tag, sizeof(T), _Allocator, _Lock> pool;

private:
	BoostPool() {}
	~BoostPool() {}

public:
	static BoostPool* Instance()
	{
		static BoostPool obj;
		return &obj;
	}

	void free(T* const ptr)
	{
		_ASSERTE(pool::is_from(ptr));
		ptr->~T();
		pool::free(ptr);
	}
    
    template<typename... ARGS>
	T* allocate(ARGS... args)
	{
		return new (pool::malloc()) T(args...);
	}
};

template<size_t _Unit, typename T, class _Lock, class _Allocator, class Tag>
class BoostPool<T[_Unit], _Lock, _Allocator, Tag>;

template<size_t _Unit, class _Lock, class _Allocator, class Tag>
class BoostPool<char[_Unit], _Lock, _Allocator, Tag>
{
private:
	typedef boost::singleton_pool<Tag, _Unit, _Allocator, _Lock> pool;

private:
	BoostPool() {}
	~BoostPool() {}

public:
	static BoostPool* Instance()
	{
		static BoostPool obj;
		return &obj;
	}

public:
	void free(char* const ptr, size_t size)
	{
		_ASSERTE(pool::is_from(ptr));
		const size_t chunk_size = get_unit( size );
		if(chunk_size > 1) 
			pool::ordered_free(ptr, chunk_size);
		else
			pool::free(ptr);
	}

	char* allocate(size_t size=_Unit)
	{
		const size_t chunk_size = get_unit(size);
		return (static_cast<char*>((chunk_size > 1) ? pool::ordered_malloc(chunk_size) : pool::malloc()));
	}

	size_t get_capacity(size_t size)
	{
		return get_unit(size) * _Unit;
	}

private:
	size_t get_unit(size_t size)
	{
		if (0 == size)
		{
			return 1;
		}

		return (size % _Unit) ? (size / _Unit) + 1 : (size / _Unit);
	}
};

} // namespace light