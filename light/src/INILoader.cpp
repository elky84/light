#include "light.h"
#include "INILoader.h"

#include "Profiler.h"

namespace light
{

namespace INI
{

Data::Data()
{
	Loader::Instance()->set(this);
}

Data::~Data()
{
	Loader::Instance()->remove(this);
}

void Loader::set(Data* data)
{
	m_set_Data.insert(data);
}

void Loader::remove(Data* data)
{
	m_set_Data.erase(data);
}

bool Loader::Load()
{
	light::ScopeProfiler profiler(__FUNCTIONW__, 100, L"ElapsedTime");
	if(m_set_Data.empty())
	{
		LOG_ERROR(L"Failed. Data is empty(). Please check linker option. turn on /OPT:NOREF ?");
		return false;
	}

	for each(Data* data in m_set_Data)
	{
		if( data->Load() == false )
		{
			LOG_ERROR(L"Failed. [%s]", typeid(data).name());
			return false;
		}
	}

	return true;
}

} // namespace INI

} // namespace light
