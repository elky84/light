#include "stdafx.h"
#include "DBCommand.h"

bool TestDBCommand::DoExecute()
{
	std::wstringstream strStream;
	strStream << L"SELECT	OBJECT_NAME(ID) AS OBJ_NAME FROM SYSCOMMENTS WHERE(CHARINDEX('TEST', TEXT) > 0) GROUP BY ID";
	if (false == Query(strStream.str()))
	{
		_ASSERT(false);
		LOG_INFO(L"%s Execute Failed. Query(%s)", __FUNCTIONW__, strStream.str().c_str());
		return false;
	}

	while(SQL_SUCCESS == Fetch())
	{
		LOG_INFO(L"%s Load Data.", __FUNCTIONW__);
	}

	Close();
	return true;
}