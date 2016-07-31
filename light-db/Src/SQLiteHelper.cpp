#include "light-db.h"
#include "SQLiteHelper.h"

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"

namespace light
{

namespace db
{

namespace SQLite
{

Helper::Helper(void)
{
}


Helper::~Helper(void)
{
	Save();
	m_Stmt.reset();

	try
	{
		m_DB.reset();
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
	}
}


bool Helper::Init(const std::wstring& Name)
{
	m_Name = WCHAR_TO_CHAR(Name.c_str());

	try
	{
		m_DB.reset(new Kompex::SQLiteDatabase(m_Name, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL));
		m_DB->MoveDatabaseToMemory();
		m_Stmt.reset(new Kompex::SQLiteStatement( m_DB.get() ));
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
		return false;
	}
	return true;
}

bool Helper::Execute(const std::wstring& query)
{
	if(m_Stmt.get() == NULL)
		return false;

	std::string str = WCHAR_TO_CHAR(query.c_str());
	try
	{
		m_Stmt->SqlStatement(str);
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
		return false;
	}
	return true;
}

bool Helper::Select(const std::wstring& query)
{
	if(m_Stmt.get() == NULL)
		return false;

	std::string str = WCHAR_TO_CHAR(query.c_str());
	try
	{
		m_Stmt->Sql(str);
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
		return false;
	}
	return true;
}

bool Helper::Next()
{
	if(m_Stmt.get() == NULL)
		return false;

	try
	{
		if( false == m_Stmt->FetchRow() )
			return false;
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
		return false;
	}

	for(MAP_BIND::iterator it = m_mapBind.begin(); it != m_mapBind.end(); ++it)
	{
		std::string column = WCHAR_TO_CHAR(it->first.c_str());
		CONVERT_INFO& ConvertInfo = it->second;

		std::wstring str = CHAR_TO_WCHAR(m_Stmt->GetColumnString(column).c_str());
		if( true == str.empty())
			continue;

		if( false == Convert(str, ConvertInfo) )
			return false;
	}

	return true;
}

bool Helper::Save()
{
	if(m_DB.get() == NULL)
		return false;

	if(m_Name.empty())
		return false;

	try
	{
		m_DB->SaveDatabaseFromMemoryToFile(m_Name);
	}
	catch (Kompex::SQLiteException &e)
	{
		LOG_ERROR(L"%s", CHAR_TO_WCHAR(e.GetString().c_str()));
		return false;
	}

	return true;
}

} // namespace SQLite

} // namespace db

} // namespace light