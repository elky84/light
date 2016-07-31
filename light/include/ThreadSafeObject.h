#pragma once

namespace light
{

/**
@brief 스레드 세이프하게 동작하도록 자료구조를 랩핑해주는 클래스
*/
template <typename DataStructure, typename SyncObject>
class ThreadSafeObject
{
public:
	/**
	@brief 생성자
	*/
	ThreadSafeObject()
	{
	}

	/**
	@brief 소멸자
	*/
	~ThreadSafeObject()
	{
	}

	/**
	@brief 데이터 추가 메소드
	@param 추가할 데이턴
	*/
	template <typename T>
	void add(const T& Object)
	{
		m_SyncObject.lock();
		m_DataStructure.push_back(Object);
		m_SyncObject.unlock();
	}

	/**
	@brief 데이터 제거 메소드
	@param 제거할 데이터
	*/
	template <typename T>
	bool remove(const T& Object)
	{
		m_SyncObject.lock();
		DataStructure::iterator it = find(m_DataStructure.begin(), m_DataStructure.end(), Object);
		if(it == m_DataStructure.end())
		{
			m_SyncObject.unlock();
			return false;
		}
		m_DataStructure.erase(it);
		m_SyncObject.unlock();
		return true;
	}


	/**
	@brief 데이터 얻어오기 메소드
	@param Func 동작시킬 함수 객체
	*/
	template <typename T, typename Fuction>
	T* get(Fuction Func)
	{
		m_SyncObject.lock();
		for(DataStructure::iterator it = m_DataStructure.begin(); it != m_DataStructure.end(); ++it)
		{
			T& Object = *it;
			if(true == Func(Object))
			{
				m_SyncObject.unlock();
				return &Object;
			}
		}
		m_SyncObject.unlock();
		return NULL;
	}

	/**
	@brief 멤버 순환 메소드 호출 메소드
	@param Func 동작시킬 함수 객체
	*/
	template <typename Fuction>
	void for_each(Fuction Func)
	{
		m_SyncObject.lock();
		for(auto it = m_DataStructure.begin(); it != m_DataStructure.end(); ++it)
		{
			Func(*it);
		}
		m_SyncObject.unlock();
	}

private:
	/// 자료 구조
	DataStructure m_DataStructure;

	/// 동기화 오브젝튼
	SyncObject m_SyncObject;
};

} //namespace light
