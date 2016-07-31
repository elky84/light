#pragma once

namespace light
{

template <typename T>
class Node
{
public:
	Node()
		: m_Next(NULL)
		, m_value(NULL)
		, m_rev(NULL)
	{
	}

	Node(const T* pValue)
		: m_Next(NULL)
		, m_value()
		, m_rev(NULL)
	{
		if(pValue)
		{
			m_value = new T;
			*m_value = *pValue;
		}
	}

	~Node()
	{
		if(m_value)
		{
			delete m_value;
		}
	}

	void Prev(Node<T>* pPrev)
	{
		m_rev = pPrev;
	}

	Node<T>* Prev() const
	{
		return m_rev;
	}

	void next(Node<T>* pNext)
	{
		m_Next = pNext;
	}

	Node<T>* next() const
	{
		return m_Next;
	}

	T& get() const
	{	
		return *m_value;
	}

	Node<T>& operator++()
	{ 
		if(NULL == next())
		{
			m_value = NULL;
		}
		else
		{
			*this = *next();
		}
		return *this;
	}

	bool operator != (const Node<T>& rhs) const
	{
		return m_value != rhs.m_value;
	}

	T& operator*() const
	{ 
		return get();
	}

private:
	T* m_value;

	Node<T>* m_Next;

	Node<T>* m_rev;
};

} //namespace light
