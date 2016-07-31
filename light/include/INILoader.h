#pragma once

#include "Singleton.h"
#include <set>

#include "INIParser.h"

namespace light
{

namespace INI
{

class Data
{
public:
	Data();

	virtual ~Data();

	virtual bool Load() = 0;

	template <typename T>
	T get(const std::string& key)
	{
		return m_parser.get<T>(key);
	}

protected:
	Parser m_parser;
};

class Loader
{
private:
	typedef std::set<Data*> SET_DATA;

public:
	void set(Data* data);
	void remove(Data* data);

	bool Load();

	template <typename T>
	T* find()
	{
		for each(Data* data in m_set_Data)
		{
			if( typeid(T).name() == typeid(*data).name() )
			{
				return (T*)data;
			}
		}

		return NULL;
	}

	STATIC_SINGLETON(Loader);

private:
	SET_DATA m_set_Data;
};

} // namespace INI

} // namespace light
