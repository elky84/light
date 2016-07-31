#pragma once

#include <sql.h>
#include <sqlext.h>

#include "OdbcParam.h"

/// ODBCCP32.lib 충돌 해결 방법
#ifdef _DEBUG
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(lib, "msvcrtd.lib")

#else //_DEBUG

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(lib, "msvcrt.lib")

#endif //_DEBUG

namespace light
{

namespace db
{

namespace Odbc
{

struct ACCOUNT_INFO
{
	std::wstring m_strDsn;
	std::wstring m_strUser;
	std::wstring m_strPassword;

	ACCOUNT_INFO()
	{
	}

	ACCOUNT_INFO(const TCHAR* szDsn, const TCHAR* szUser, const TCHAR* szPassword)
		: m_strDsn(szDsn)
		, m_strUser(szUser)
		, m_strPassword(szPassword)
	{
	}
};

class Accessor
{
public:
	Accessor();
	~Accessor();

	bool		Connect(const ACCOUNT_INFO& accInfo);
	void		Disconnect();

	void		ResetCursor();

	bool		Bind(SQLUSMALLINT iCol, SQLSMALLINT fCType, SQLPOINTER buf, SQLLEN bufLen);
	bool        BindParameter(SQLUSMALLINT ipar, const BindParameterInfo& param, const TCHAR* name);
	void		Unbind();

	bool		Execute(const std::wstring& query);

	SQLRETURN	Fetch();

	TCHAR**		GetRow();

	SQLRETURN	GetNextResultSet();

	int			GetNumResultCols();

	int			GetAffectedRowCount();

	int			GetLen_Or_Indicator(const SQLUSMALLINT Col);

	bool		ColumnName(const TCHAR* table);

	void		Diagnostic(SQLSMALLINT handle_type, SQLHANDLE handle, bool conn);

	bool		PutData();
	
	SQLLEN		GetNextAlignedPos(SQLLEN nPos);

private:

	class Env
	{
	public:
		Env();
		~Env();

		SQLHENV GetHandle()
		{ 
			return m_Env;
		}

	private:
		SQLHENV	m_Env;
	};

	static Env m_Env;

private:
	SQLHSTMT m_stmt;
	SQLHDBC m_dbc;

	union
	{
		TCHAR* m_Row[ODBC_FIELDNUM];
		SQLLEN m_Length_or_Indicator[ODBC_FIELDNUM];
	};

	TCHAR m_Result[BUFFER_SIZE];
};

class Helper
{
public:
	explicit Helper(const ACCOUNT_INFO& accInfo);

	~Helper();

	bool Prepare(std::wstring& query, const TCHAR* format, ...);

	template < typename T >
	bool Bind(SQLUSMALLINT iCol, T& rBuf)
	{
		return m_Accessor.Bind(iCol, ConvertType(rBuf), &rBuf, sizeof(rBuf));
	}

	template < typename T > 
	bool Bind(SQLUSMALLINT iCol, T* buf, SQLLEN bufLen)
	{
		return m_Accessor.Bind(iCol, GetCType(buf), buf, bufLen);
	}

	bool BindParameter(const BindParameterInfo& param, const TCHAR* name = NULL)
	{
		return m_Accessor.BindParameter(++m_ParamNumber, param, name);
	}

	bool Execute(const std::wstring& str)
	{
		return m_Accessor.Execute(str);
	}

	bool Execute(const TCHAR* format, ...);

	bool Query(const TCHAR* format, ...);

	bool Query(const std::wstring& str);

	SQLRETURN Fetch()
	{
		return m_Accessor.Fetch();
	}

	SQLRETURN GetNextResultSet(bool bUnbind = true)
	{
		if(bUnbind)
		{
			m_Accessor.Unbind();
		}

		return m_Accessor.GetNextResultSet();
	}

	int GetRowCount()
	{
		return m_Accessor.GetAffectedRowCount();
	}

	LONG Result()
	{
		SQLRETURN retVal;

		while(SQL_SUCCESS == (retVal = m_Accessor.GetNextResultSet())) ;

		if(retVal == SQL_ERROR)
			return SQL_ERROR;

		return m_spRetVal;
	}

	void Reset()
	{
		m_Accessor.ResetCursor();
		m_ParamNumber = 0;
		m_spRetVal = SQL_ERROR;	
	}

	void Close()
	{
		m_Accessor.Disconnect();
		m_ParamNumber = 0;
		m_spRetVal = SQL_ERROR;
	}

	bool ColumnName(const TCHAR* szTable)
	{
		return m_Accessor.ColumnName(szTable);
	}

	Accessor& GetAccessor()
	{
		return m_Accessor;
	}

	SQLLEN ColumnLength(SQLUSMALLINT iCol)
	{
		return m_Accessor.GetLen_Or_Indicator(iCol);
	}

private:
	Helper(const Helper&);

	Helper& operator=(const Helper&);

	inline bool _Prepare(std::wstring& query, const TCHAR* format, va_list arglist);

private:
	const ACCOUNT_INFO		m_Account;

	Accessor				m_Accessor;

	SQLUSMALLINT			m_ParamNumber;

	LONG					m_spRetVal;
};


} // namespace Odbc

} // namespace db

} // namespace light