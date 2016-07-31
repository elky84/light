#pragma once

#include "MSSQLType.h"

namespace light
{

namespace db
{

namespace Odbc
{

const int ODBC_FIELDNUM = 96;

const int BUFFER_SIZE = 4096;

//////////////////////////////////////////////////////////////////////////

class BindParameterInfo
{
public:

	BindParameterInfo(const TCHAR* param) 
		: m_BufLen(0)
		, m_IoType(SQL_PARAM_INPUT)
	{
		m_Buf = const_cast<TCHAR*>(param);
		m_Bytes = _tcslen(param);

		Init(SQL_TVARCHAR, SQL_C_CHAR);
	}

	BindParameterInfo(TCHAR* param) 
		: m_BufLen(0)
		, m_IoType(SQL_PARAM_INPUT)
	{
		m_Buf = param;
		m_Bytes = _tcslen(param);

		Init(SQL_TVARCHAR, SQL_C_CHAR);
	}

	template <typename T>
	BindParameterInfo(const T& param) 
		: m_Bytes(0)
		, m_BufLen(0)
		, m_IoType(SQL_PARAM_INPUT)
	{
		m_Buf = (TCHAR*)&param;
		Init(GetSqlType(param), convert_type(param));
	}

	SQLLEN Bytes() const
	{
		return m_Bytes;
	}

	void Bytes(SQLLEN bytes)
	{
		m_Bytes = bytes;
	}

	SQLSMALLINT IoType() const
	{
		return m_IoType;
	}

	SQLSMALLINT CType() const
	{
		return m_cType;
	}

	SQLSMALLINT SQLType() const
	{
		return m_sqlType;
	}

	TCHAR* Buf() const
	{
		return m_Buf;
	}

	SQLLEN BufLen() const
	{
		return m_BufLen;
	}

	void SetOutParamInfo(SQLSMALLINT ioType, unsigned int bufLen)
	{
		m_BufLen = bufLen;
		m_Bytes = bufLen;
		m_IoType = ioType;
	}

private:

	void Init(SQLSMALLINT sqlType, SQLSMALLINT cType)
	{
		m_sqlType = sqlType;
		m_cType = cType;
	}

	SQLSMALLINT m_sqlType;

	SQLSMALLINT m_cType;

	SQLLEN m_Bytes;

	SQLSMALLINT m_IoType;

	SQLLEN m_BufLen;

	TCHAR* m_Buf;
};

//////////////////////////////////////////////////////////////////////////

class OutputParamInfo 
	: public BindParameterInfo
{
public:
	OutputParamInfo(TCHAR*& buf, unsigned int bufLen) 
		: BindParameterInfo(buf)
	{
		SetOutParamInfo(SQL_PARAM_INPUT_OUTPUT, bufLen);
	}

	template <size_t _Len>
	OutputParamInfo(TCHAR (&buf)[_Len]) 
		: BindParameterInfo(buf)
	{
		SetOutParamInfo(SQL_PARAM_INPUT_OUTPUT, _Len);
	}

	template <typename T>
	OutputParamInfo(T& param) 
		: BindParameterInfo(param)
	{
		SetOutParamInfo(SQL_PARAM_INPUT_OUTPUT, 0);
	}

private:

	// 사용 하면 안되는 형식들
	OutputParamInfo(char*&buf, bool isOutOnly = true);
	template<typename T> OutputParamInfo(const T& param, bool isOutOnly = true);
};

} // namespace Odbc

} // namespace db

} // namespace light