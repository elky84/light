#include "light-excel.hpp"
#include "ExcelLoader.h"

#include "Profiler.h"

namespace light
{

namespace excel
{

Data::Data()
{
	Loader::Instance()->Register(this);
}

Data::~Data()
{
	Loader::Instance()->UnRegister(this);
}

void Loader::Register(Data* data)
{
	m_set_Data.insert(data);
}

void Loader::UnRegister(Data* data)
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
			LOG_ERROR(L"Failed. [%S]", typeid(data).name());
			return false;
		}
	}

	return true;
}

} // namespace excel

} // namespace light
