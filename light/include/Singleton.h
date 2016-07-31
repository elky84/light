#pragma once

#include <assert.h>

namespace light
{

template <typename T> 
class Singleton
{
	static T* m_tr;
public:
	Singleton(void)
	{
		//assert(!m_tr);
		intptr_t offset = (intptr_t)(T*)1 - (intptr_t)(Singleton<T>*)(T*)1 ;
		m_tr = (T*)((intptr_t)this + offset) ;
	}

	virtual ~Singleton(void)
	{
		m_tr = 0;
	}

	static T* Instance(void)
	{
		//assert(m_tr);
		return (m_tr);
	}

	static void Destroy()
	{
		if(m_tr)
		{
			delete m_tr;
			m_tr = 0;
		}
	}
};

template <typename T>T* Singleton<T>::m_tr = 0;

#define STATIC_SINGLETON( cls ) \
public: \
	static cls* Instance()\
	{\
		static cls s_Instance; \
		return &s_Instance; \
	}

} //namespace light
