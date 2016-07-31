#pragma once

namespace light
{

template <typename T>
class smart_singleton
{
public:
	smart_singleton()
	{
		if(++m_ref_count == 1)
		{
			m_ptr = new T;
		}
	}

	virtual ~smart_singleton()
	{
		if(--m_ref_count == 0)
		{
			SAFE_DELETE(m_ptr);
		}
	}

	static T* smart_instance()
	{
		return m_ptr;
	}

private:
    static int m_ref_count;

    static T* m_ptr;
};

template <typename T> int smart_singleton<T>::m_ref_count = 0;
template <typename T> T* smart_singleton<T>::m_ptr = 0;

} //namespace light
