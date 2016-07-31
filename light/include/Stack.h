#pragma once

#include "Node.h"
#include "Macro.h"

namespace light
{

template <typename T>
class Stack
{
public:
	typedef Node<T> iterator;

public:
	Stack()
		: m_head(NULL)
		, m_tail(NULL)
	{
	}

	~Stack()
	{
		clear();
	}

	T& back()
	{
		LOG_ERROR(L"Not exist tail.");
		return m_tail->get();
	}

	void push(const T& Value)
	{
		iterator* pNode = new Node<T>(&Value);
		if(m_head == NULL)
		{
			m_head = pNode;
		}

		if(m_tail)
		{
			pNode->Prev(m_tail);
			m_tail->next(pNode);
		}

		m_tail = pNode;
	}

	void pop()
	{
		if(m_tail)
		{
			iterator* pTail = m_tail;
			m_tail = pTail->Prev();

			if(m_tail)
			{
				m_tail->next(NULL);
			}
			else //tail이 없단건 아무것도 없단 얘기.
			{
				m_head = NULL;
			}
			delete pTail;
		}
		else
		{
			LOG_ERROR(L"Not exist tail.");
		}
	}

	bool empty()
	{
		return m_head == NULL ? true : false;
	}

	void clear()
	{
		iterator* pNode = m_head, *pNext = NULL;
		while(pNode != NULL)
		{
			pNext = pNode->next();
			delete pNode;
			pNode = pNext;
		}

		m_head = NULL;
		m_tail = NULL;
	}

	iterator& begin() const
	{
		if(m_head)
		{
			return *m_head;
		}
		else
		{
			return EndPosition;
		}
	}

	iterator& end() const
	{
		return EndPosition;
	}

private:
	iterator* m_head;

	iterator* m_tail;

	static iterator EndPosition;
};

template <typename T> Node<T> Stack<T>::EndPosition = Node<T>();

} //namespace light
