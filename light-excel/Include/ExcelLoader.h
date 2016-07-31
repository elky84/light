#pragma once

#include "Singleton.h"
#include <set>

#include "XMLParser.h"

namespace light
{

namespace excel
{


class Data
{
public:
	Data();

	virtual ~Data();

	virtual bool Load() = 0;
};


class Loader
{
private:
	typedef std::set<Data*> SET_LOADER;

public:
	void Register(Data* data);
	void UnRegister(Data* data);

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
	SET_LOADER m_set_Data;
};

} // namespace excel

} // namespace light
