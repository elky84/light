#pragma once

#include	<boost/thread/mutex.hpp>
#include	<memory>

namespace Kompex
{

class SQLiteStatement;
class SQLiteDatabase;

} // namespace Kompex

#include "SQLiteConverter.h"


namespace light
{

namespace db
{

namespace SQLite
{

class Helper
{
public:
	typedef std::map<std::wstring, CONVERT_INFO> MAP_BIND;

public:
	Helper();
	~Helper();

public:
	bool Init(const std::wstring& Name);

	bool Execute(const std::wstring& query);

	bool Select(const std::wstring& query);

	bool Save();

	bool Next();

public: // Bind 계열 메소드

	/// 특정 엘리먼트 안에 포함된 Attrib와 문자열 객체와 바인드 시켜주는 메소드
	template <typename T>
	bool Bind(const std::wstring& str, T& Data)
	{
		const SQLite::TYPE type = SQLite::ConvertType(Data);
		if( type == SQLite::DENIED )
			return false;

		return Bind(type, str, &Data, sizeof(Data));
	}

	/// 특정 엘리먼트 안에 포함된 Attrib와 문자열 객체와 바인드 시켜주는 메소드
	bool Bind(const std::wstring& str, TCHAR* Str, size_t Length)
	{
		const SQLite::TYPE type = SQLite::ConvertType(Str);
		if(type == SQLite::DENIED)
			return false;

		return Bind(type, str, Str, Length);
	}

private: /// 해당 타입이 넘어오면 컴파일 안되게 하기 위한 선언부
	bool Bind(const std::wstring& str, const TCHAR* Str);

	bool Bind(const std::wstring& str, TCHAR* Str);

private:
	/// 특정 엘리먼트 안에 포함된 Attrib와 버퍼와 바인드 시켜주는 메소드
	bool Bind(const SQLite::TYPE Type, const std::wstring& str, void* Ptr, size_t Length)
	{
		std::pair<MAP_BIND::iterator, bool> Ret = m_mapBind.insert((MAP_BIND::value_type(str, SQLite::CONVERT_INFO(Type, Length, Ptr))));
		return Ret.second;
	}

protected:
	boost::mutex m_Mutex;

	MAP_BIND m_mapBind;

	std::string m_Name;

	std::auto_ptr<Kompex::SQLiteStatement> m_Stmt;

	std::auto_ptr<Kompex::SQLiteDatabase> m_DB;
};

} // namespace SQLite

} // namespace db

} // namespace light