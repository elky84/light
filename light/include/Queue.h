#pragma once

#include "Node.h"
#include "Macro.h"

namespace light
{

template <typename T>
class Queue
{
public:
	typedef Node<T> iterator;

public:
	Queue()
		: m_head(NULL)
		, m_tail(NULL)
	{
	}

	~Queue()
	{
		clear();
	}

	T& front()
	{
		if(m_head == NULL)
		{
			LOG_ERROR(L"Not exist head.");
		}
		return m_head->get();
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
		if(m_head)
		{
			iterator* pHead = m_head;
			m_head = NULL;
			m_head = pHead->next();
			if(m_head)
			{
				m_head->Prev(NULL);
			}
			else //head가 없단건 아무것도 없단 얘기.
			{
				m_tail = NULL;
			}

			delete pHead;
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

template <typename T> Node<T> Queue<T>::EndPosition = Node<T>();

} //namespace light
