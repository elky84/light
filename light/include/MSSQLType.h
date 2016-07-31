#pragma once

// ctype for ms-sql
#ifdef UNICODE
#define SQL_TVARCHAR        SQL_WVARCHAR
#define SQL_TLONGVARCHAR    SQL_WLONGVARCHAR
#else
#define SQL_TVARCHAR        SQL_VARCHAR
#define SQL_TLONGVARCHAR    SQL_LONGVARCHAR
#endif

//////////////////////////////////////////////////////////////////////////
// for enum type
template <bool> struct Enumerator;
template <> struct Enumerator<true>
{
	static const SQLSMALLINT c_type_value = SQL_C_LONG;
	static const SQLSMALLINT sql_type_value = SQL_INTEGER;
};

//////////////////////////////////////////////////////////////////////////
template <typename T> inline SQLSMALLINT convert_type(T)        { return Enumerator<std::is_enum<T>::value>::c_type_value; }
template <> inline SQLSMALLINT convert_type(WCHAR)				{ return SQL_C_STINYINT; }
template <> inline SQLSMALLINT convert_type(char)               { return SQL_C_CHAR; }   
template <> inline SQLSMALLINT convert_type(unsigned char)      { return SQL_C_TINYINT; }
template <> inline SQLSMALLINT convert_type(bool)               { return SQL_C_TINYINT; }
template <> inline SQLSMALLINT convert_type(short)              { return SQL_C_SHORT; }
template <> inline SQLSMALLINT convert_type(unsigned short)     { return SQL_C_SHORT; }
template <> inline SQLSMALLINT convert_type(int)                { return SQL_C_LONG; }
template <> inline SQLSMALLINT convert_type(unsigned int)       { return SQL_C_ULONG; }
template <> inline SQLSMALLINT convert_type(long)               { return SQL_C_LONG; }
template <> inline SQLSMALLINT convert_type(unsigned long)      { return SQL_C_ULONG; }
template <> inline SQLSMALLINT convert_type(__int64)            { return SQL_C_SBIGINT; }
template <> inline SQLSMALLINT convert_type(unsigned __int64)   { return SQL_C_UBIGINT; }
template <> inline SQLSMALLINT convert_type(float)              { return SQL_C_FLOAT; }
template <> inline SQLSMALLINT convert_type(double)             { return SQL_C_DOUBLE; }
template <> inline SQLSMALLINT convert_type(TIMESTAMP_STRUCT)   { return SQL_C_TYPE_TIMESTAMP; }

template <> inline SQLSMALLINT convert_type(char*)             { return SQL_C_CHAR; }
template <> inline SQLSMALLINT convert_type(const char*)       { return SQL_C_CHAR; }
template <> inline SQLSMALLINT convert_type(WCHAR*)             { return SQL_C_WCHAR; }
template <> inline SQLSMALLINT convert_type(const WCHAR*)       { return SQL_C_WCHAR; }
template <> inline SQLSMALLINT convert_type(void*)              { return SQL_C_BINARY; }
template <> inline SQLSMALLINT convert_type(const void*)        { return SQL_C_BINARY; }

//////////////////////////////////////////////////////////////////////////
template <typename T> inline SQLSMALLINT GetSqlType(T)        { return Enumerator<std::is_enum<T>::value>::sql_type_value; }
template <> inline SQLSMALLINT GetSqlType(WCHAR)			  { return SQL_WCHAR; }
template <> inline SQLSMALLINT GetSqlType(char)               { return SQL_CHAR; }   
template <> inline SQLSMALLINT GetSqlType(unsigned char)      { return SQL_TINYINT; }
template <> inline SQLSMALLINT GetSqlType(bool)               { return SQL_TINYINT; }
template <> inline SQLSMALLINT GetSqlType(short)              { return SQL_SMALLINT; }
template <> inline SQLSMALLINT GetSqlType(unsigned short)     { return SQL_SMALLINT; }
template <> inline SQLSMALLINT GetSqlType(int)                { return SQL_INTEGER; }
template <> inline SQLSMALLINT GetSqlType(unsigned int)       { return SQL_INTEGER; }
template <> inline SQLSMALLINT GetSqlType(long)               { return SQL_INTEGER; }
template <> inline SQLSMALLINT GetSqlType(unsigned long)      { return SQL_INTEGER; }
template <> inline SQLSMALLINT GetSqlType(__int64)            { return SQL_BIGINT; }
template <> inline SQLSMALLINT GetSqlType(unsigned __int64)   { return SQL_BIGINT; }
template <> inline SQLSMALLINT GetSqlType(float)              { return SQL_REAL; }
template <> inline SQLSMALLINT GetSqlType(double)             { return SQL_DOUBLE; }
template <> inline SQLSMALLINT GetSqlType(TIMESTAMP_STRUCT)   { return SQL_TYPE_TIMESTAMP; }

template <> inline SQLSMALLINT GetSqlType(WCHAR*)             { return SQL_TVARCHAR; }
template <> inline SQLSMALLINT GetSqlType(const WCHAR*)       { return SQL_TVARCHAR; }
template <> inline SQLSMALLINT GetSqlType(void*)              { return SQL_VARBINARY; }
template <> inline SQLSMALLINT GetSqlType(const void*)        { return SQL_VARBINARY; }
//////////////////////////////////////////////////////////////////////////
