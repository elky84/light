#pragma once

#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <map>

template<typename T>
class FSM
{
	static const T STATE_NONE = static_cast<T>(-1);

	struct FSM_ATTRIBUTES
	{
		FSM_ATTRIBUTES() 
			: m_NextStateID(STATE_NONE)
		{
		}

		FSM_ATTRIBUTES(T nextStateID, boost::function<void()> funcEnter, boost::function<void()> funcLeave, boost::function<void()> funcUpdate) 
			: m_NextStateID(nextStateID)
			, m_funcEnter(funcEnter)
			, m_funcLeave(funcLeave)
			, m_funcUpdate(funcUpdate)
		{
		}

		T m_NextStateID;

		boost::function<void()> m_funcEnter;

		boost::function<void()> m_funcLeave;

		boost::function<void()> m_funcUpdate;
	};


	typedef std::map<T, FSM_ATTRIBUTES>	MAP_FSM_STATE;

public:
	FSM()
		: m_PrevStateID(STATE_NONE)
		, m_CurrentStateID(STATE_NONE)
		, m_CurrentStateAttribute()
	{
	}

	virtual ~FSM()
	{
	}

	void Destroy()
	{
		m_map_State.clear();
		m_PrevStateID = STATE_NONE;
		m_CurrentStateID = STATE_NONE;
	}

protected:
	bool AddState(T stateID, T nextStateID, boost::function<void()> funcEnter, boost::function<void()> funcLeave, boost::function<void()> funcUpdate)
	{
		auto pair = m_map_State.insert(MAP_FSM_STATE::value_type(stateID, FSM_ATTRIBUTES(nextStateID, funcEnter, funcLeave, funcUpdate)));
		if (pair.second == false)
		{
			LOG_ERROR(_T("Cannot insert state attr!! [%d] [%d]"), stateID, nextStateID);
			return false;
		}
		return true;
	}

public:
	inline T Current() const
	{
		return m_CurrentStateID;
	}


	inline T Prev() const
	{
		return m_PrevStateID;
	}

	bool Next()
	{
		return Transit(m_CurrentStateAttribute.m_NextStateID);
	}

	bool Transit(T stateID)
	{
		if(stateID <= STATE_NONE)
		{
			LOG_ERROR(_T("Invalid state!! STATE_ID[%d]"), stateID);
			return false;
		}

		m_PrevStateID = m_CurrentStateID;
		if (m_CurrentStateID != STATE_NONE)
		{
			if (m_CurrentStateAttribute.m_funcLeave != false)
			{
				m_CurrentStateAttribute.m_funcLeave();
			}
		}

		m_CurrentStateID = stateID;

		auto it = m_map_State.find(stateID);
		if (it == m_map_State.end())
		{
            LOG_ERROR(_T("Not find state. STATE_ID[%d]"), stateID);
			return false;
		}

		m_CurrentStateAttribute = it->second;

		if (m_CurrentStateAttribute.m_funcEnter == NULL)
		{
			LOG_ERROR(_T("Not exist enter fuction. STATE_ID[%d]"), stateID);
			return false;
		}

		m_CurrentStateAttribute.m_funcEnter();
		return true;
	}

	void Update()
	{
		if ( m_CurrentStateAttribute.m_funcUpdate != NULL )
		{
			m_CurrentStateAttribute.m_funcUpdate();
		}
	}

protected:
	MAP_FSM_STATE m_map_State;

	FSM_ATTRIBUTES m_CurrentStateAttribute;

	T m_PrevStateID;

	T m_CurrentStateID;
};