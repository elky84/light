#include "light-db.h"
#include "Odbc.h"
#include <odbcinst.h>
#include <odbcss.h>

#include "Profiler.h"

namespace light
{

namespace db
{

namespace Odbc
{

Accessor::Env Accessor::m_Env;

static void SafeFreeHandle(SQLHDBC handle)
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, handle);
		handle = INVALID_HANDLE_VALUE;
	}
}

Accessor::Env::Env()
{
	if(!SQL_SUCCEEDED(SQLSetEnvAttr(SQL_NULL_HANDLE, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER))) 
	{
		throw;
	}

	if(!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_Env))) 
	{
		throw;
	}

	if (!SQL_SUCCEEDED(SQLSetEnvAttr(m_Env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER))) 
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_Env);
		throw;
	}

	if(!SQL_SUCCEEDED(SQLSetEnvAttr(m_Env, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER))) 
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_Env);
		throw;
	}
}

Accessor::Env::~Env()
{
	SQLFreeHandle(SQL_HANDLE_ENV, m_Env);
}

Accessor::Accessor(void)
	: m_stmt(INVALID_HANDLE_VALUE)
	, m_dbc(INVALID_HANDLE_VALUE)
{
	m_Row[0] = NULL;
	m_Result[0] = '0';
}

Accessor::~Accessor(void) 
{
	Disconnect();
}

void Accessor::Disconnect()
{
	if(m_stmt != INVALID_HANDLE_VALUE)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_stmt);
		m_stmt = INVALID_HANDLE_VALUE;
	}

	if(m_dbc != INVALID_HANDLE_VALUE)
	{
		SQLDisconnect(m_dbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_dbc);
		m_dbc = INVALID_HANDLE_VALUE;
	}
}

void Accessor::ResetCursor()
{
	if(m_stmt != INVALID_HANDLE_VALUE)
	{
		SQLCloseCursor(m_stmt);
	}
}

bool Accessor::Connect(const ACCOUNT_INFO& accInfo)
{
	static const DWORD dwTimeOut = 20;

	if( m_dbc != INVALID_HANDLE_VALUE )
	{
		if(SQL_SUCCEEDED(SQLGetConnectAttr(m_dbc, SQL_ATTR_CONNECTION_TIMEOUT, NULL, 0, NULL)))
		{
			if( m_stmt != INVALID_HANDLE_VALUE )
			{
				return SQL_SUCCEEDED(SQLGetStmtAttr(m_stmt, SQL_ATTR_QUERY_TIMEOUT, NULL, 0, NULL));
			}
		}

		// if already connected, disconnect!
		Disconnect();
	}

	if( FALSE == SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, m_Env.GetHandle(), &m_dbc)))
	{
		Diagnostic(SQL_HANDLE_ENV, m_Env.GetHandle(), false);
		return false;
	}

	if( FALSE == SQL_SUCCEEDED(SQLConnect(m_dbc, (SQLTCHAR*)accInfo.m_strDsn.c_str(), SQL_NTS, (SQLTCHAR*)accInfo.m_strUser.c_str(), SQL_NTS, (SQLTCHAR*)accInfo.m_strPassword.c_str(), SQL_NTS)) )
	{
		Diagnostic(SQL_HANDLE_DBC, m_dbc, false);
		SQLFreeHandle(SQL_HANDLE_DBC, m_dbc);
		m_dbc = INVALID_HANDLE_VALUE;
		return false;
	}

	if( FALSE == SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &m_stmt)))
	{
		Diagnostic(SQL_HANDLE_DBC, m_dbc, false);

		SQLDisconnect(m_dbc);

		SQLFreeHandle(SQL_HANDLE_DBC, m_dbc);
		m_dbc = INVALID_HANDLE_VALUE;
		return false;
	}

	// http://msdn.microsoft.com/en-us/library/ms713605(VS.85).aspx
	// http://msdn.microsoft.com/en-us/library/ms711810(v=VS.85).aspx
	SQLSetStmtAttr(m_stmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)dwTimeOut, SQL_IS_UINTEGER);
	return true;
}

bool Accessor::Bind(SQLUSMALLINT iCol, SQLSMALLINT fCType, SQLPOINTER buf, SQLLEN bufLen)
{
	if( FALSE == SQL_SUCCEEDED(SQLBindCol(m_stmt, iCol, fCType, buf, bufLen, &m_Length_or_Indicator[iCol])))
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, true);
		return false;
	}

	return true;
}

bool Accessor::ColumnName(const TCHAR* table)
{
	TCHAR szParam[128] = {0, };
	_stprintf_s(szParam, L"\"%s\"", table);

	if ( FALSE == SQL_SUCCEEDED(SQLColumns(m_stmt, NULL, 0, NULL, 0, reinterpret_cast<SQLTCHAR*>(szParam), SQL_NTS, NULL, 0)) )
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
		return false;
	}

	return true;
}

bool Accessor::BindParameter(SQLUSMALLINT ipar, const BindParameterInfo& param, const TCHAR* name)
{
	m_Length_or_Indicator[ipar] = param.Bytes();

	if(!SQL_SUCCEEDED(SQLBindParameter(m_stmt, ipar, param.IoType(), param.CType(), param.SQLType(), param.Bytes(), 0, param.Buf(), param.BufLen(), &m_Length_or_Indicator[ipar])))
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
		return false;
	}

	if (name)
	{
		SQLPOINTER hIpd;
		SQLGetStmtAttr(m_stmt, SQL_ATTR_IMP_PARAM_DESC, &hIpd, 0, 0);

		if (!SQL_SUCCEEDED(SQLSetDescField(hIpd, ipar, SQL_DESC_NAME, const_cast<TCHAR*>(name), SQL_NTS)))
		{
			Diagnostic(SQL_HANDLE_DESC, hIpd, false);
		}
	}

	return true;
}

void Accessor::Unbind()
{
	_ASSERT(m_stmt);

	SQLFreeStmt(m_stmt, SQL_UNBIND);
}

bool Accessor::PutData()
{
	SDWORD Offset = 0;
	BindParameterInfo* pParam = NULL;

	do 
	{
		SQLRETURN retVal = SQLParamData(m_stmt, reinterpret_cast<SQLPOINTER*>(&pParam));
		if(SQL_SUCCEEDED(retVal) || retVal == SQL_NO_DATA)
			return true;

		if(SQL_NEED_DATA != retVal || pParam == NULL) 
			break;

		while (pParam->Bytes() > BUFFER_SIZE &&
			SQL_SUCCEEDED(SQLPutData(m_stmt, const_cast<TCHAR*>(pParam->Buf() + Offset), BUFFER_SIZE)))
		{
			pParam->Bytes(pParam->Bytes() - BUFFER_SIZE);
			Offset += BUFFER_SIZE;
		}

	} while(SQL_SUCCEEDED(SQLPutData(m_stmt, const_cast<TCHAR*>(pParam->Buf()+Offset), pParam->Bytes())));

	Diagnostic(SQL_HANDLE_STMT, m_stmt, true);
	return false;
}

bool Accessor::Execute(const std::wstring& query)
{
	_ASSERT(m_stmt);

	SQLRETURN retval = SQL_INVALID_HANDLE;

	{
		ScopeProfiler profiler(L"SQLExecDirect", Profiler::DEFAULT_BASELINE, query.c_str());
		retval = SQLExecDirect(m_stmt, reinterpret_cast<SQLTCHAR*>(const_cast<TCHAR*>(query.c_str())), SQL_NTS);
	}

	if(SQL_SUCCEEDED(retval) || retval == SQL_NO_DATA)
		return true;

	if(retval == SQL_NEED_DATA) 
		return PutData();

	Diagnostic(SQL_HANDLE_STMT, m_stmt, true);

	return false;
}

SQLRETURN Accessor::Fetch()
{
	_ASSERT(m_stmt);

	SQLRETURN retVal = SQLFetch(m_stmt);

	if(SQL_SUCCEEDED(retVal))
		return SQL_SUCCESS;

	if(retVal != SQL_NO_DATA)
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, true);
		return SQL_ERROR;
	}

	return SQL_NO_DATA;
}

inline SQLLEN Accessor::GetNextAlignedPos(SQLLEN nPos)
{
	return (nPos % sizeof(ULONG_PTR ) ? nPos + sizeof(ULONG_PTR) - (nPos % sizeof(ULONG_PTR)) : nPos);
}

TCHAR** Accessor::GetRow()
{
	int Column = GetNumResultCols();
	if(Column <= 0 || ODBC_FIELDNUM < Column)
	{
		return NULL;
	}

	SQLLEN Pos = 0, len_or_ind = 0;
	for ( SQLUSMALLINT i = 1; i <= Column; i++ )
	{
		m_Row[i] = m_Result + Pos;
		if(!SQL_SUCCEEDED(SQLGetData(m_stmt, i, SQL_C_CHAR, m_Row[i], sizeof(m_Result)-Pos, &len_or_ind )))
		{
			Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
			return NULL;
		}

		_ASSERT(len_or_ind != SQL_NO_TOTAL);

		if(len_or_ind == SQL_NULL_DATA)
		{
			m_Row[i][0] = '\0';
			len_or_ind = 0;
		}

		Pos = GetNextAlignedPos(Pos + len_or_ind + 1);
	}

	return m_Row;
}

SQLRETURN Accessor::GetNextResultSet()
{
	_ASSERT(m_stmt);

	SQLRETURN retVal =  SQLMoreResults(m_stmt);

	if(SQL_SUCCEEDED(retVal))
		return SQL_SUCCESS;

	if(retVal != SQL_NO_DATA)
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
		return SQL_ERROR;
	}

	return SQL_NO_DATA;
}

int Accessor::GetNumResultCols()
{
	_ASSERT(m_stmt);

	SQLSMALLINT ColumnCount = 0;

	if(!SQL_SUCCEEDED(SQLNumResultCols(m_stmt, &ColumnCount)))
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
	}

	return ColumnCount;
}

int Accessor::GetAffectedRowCount()
{
	_ASSERT(m_stmt);

	SQLLEN RowCount = 0;

	if(!SQL_SUCCEEDED(SQLRowCount(m_stmt, &RowCount)))
	{
		Diagnostic(SQL_HANDLE_STMT, m_stmt, false);
	}

	return RowCount;
}

int Accessor::GetLen_Or_Indicator(const SQLUSMALLINT Col)
{
	return m_Length_or_Indicator[Col];
}

void Accessor::Diagnostic(SQLSMALLINT handle_type, SQLHANDLE handle, bool conn)
{
	SQLTCHAR szSqlState[BUFFER_SIZE] = { 0, };
	SQLTCHAR szErrorMsg[BUFFER_SIZE] = { 0, };
	SDWORD SS_MsgState = 0, SS_Severity = 0;
	SQLINTEGER  Rownumber = 0;
	USHORT SS_Line = 0;
	SQLSMALLINT cbSS_Procname, cbSS_Srvname;
	SQLTCHAR SS_Procname[BUFFER_SIZE] = { 0, };
	SQLTCHAR SS_Srvname[BUFFER_SIZE] = { 0, };
	TCHAR szErrOut[BUFFER_SIZE];

	for (SQLSMALLINT RecordNumber = 1;; ++RecordNumber)
	{
		SDWORD pfNativeError = 0L;
		SWORD pcbErrorMsg = 0;
		RETCODE retVal = SQLGetDiagRec(handle_type, handle, RecordNumber, szSqlState, &pfNativeError, szErrorMsg, sizeof(szErrorMsg)-1, &pcbErrorMsg);
		if( !SQL_SUCCEEDED(retVal) )
			break;

		if (conn)
		{
			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_ROW_NUMBER, &Rownumber, SQL_IS_INTEGER, NULL);

			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_SS_LINE, &SS_Line, SQL_IS_USMALLINT, NULL);

			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_SS_MSGSTATE, &SS_MsgState, SQL_IS_INTEGER, NULL);

			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_SS_SEVERITY, &SS_Severity, SQL_IS_INTEGER, NULL);

			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_SS_PROCNAME, &SS_Procname, sizeof(SS_Procname)-1, &cbSS_Procname);

			SQLGetDiagField(handle_type, handle, RecordNumber, SQL_DIAG_SS_SRVNAME, &SS_Srvname, sizeof(SS_Srvname)-1, &cbSS_Srvname);

			_stprintf_s(szErrOut, sizeof(szErrOut)-1, L"pfNativeError = %u ODBCRowNumber = %d, SSrvrLine = %u, SSrvrMsgState = %u, SSrvrSeverity = %u, SSrvrSrvname = %s",
				pfNativeError, Rownumber, SS_Line, SS_MsgState, SS_Severity, SS_Srvname );
		}
		else
		{
			_stprintf_s(szErrOut, sizeof(szErrOut)-1, L"pfNativeError = %u", pfNativeError);

		}

		LOG_ERROR(L"[E_ODBC] ErrCode(%s) ErrMsg(%s) %s", szSqlState, szErrorMsg, szErrOut);
	}
}

Helper::Helper(const ACCOUNT_INFO& accInfo) 
	: m_Account(accInfo), m_ParamNumber(0), m_spRetVal(SQL_ERROR)
{
}

Helper::~Helper()
{
	m_Accessor.Disconnect();
}

bool Helper::Prepare(std::wstring& query, const TCHAR* format, ...)
{
	va_list arglist;
	va_start(arglist, format);

	bool bRet = _Prepare(query, format, arglist);
	va_end(arglist);
	return bRet;
}

bool Helper::_Prepare(std::wstring& query, const TCHAR* format, va_list arglist)
{
	TCHAR str[BUFFER_SIZE] = {0, };

	static const TCHAR SPHeader[] = L"{ ? = call ";
	_tcsncpy_s(str, _countof(str), SPHeader, _TRUNCATE);

	int nWritten = _vstprintf_s(str + sizeof(SPHeader)-1, _countof(str) - sizeof(SPHeader)+1, format, arglist);
	if( 0 <= nWritten )
		_tcscat_s(str, _countof(str), L" }");

	if(m_Accessor.Connect(m_Account))
	{
		if(m_Accessor.BindParameter(++m_ParamNumber, OutputParamInfo(m_spRetVal), NULL))
		{
			query = str;
			return true;
		}

		Close();
	}

	return false;
}

bool Helper::Execute(const TCHAR* format, ...)
{
	va_list arglist;
	va_start(arglist, format);

	std::wstring query;
	if( _Prepare(query, format, arglist) )
	{
		va_end(arglist);
		return Execute(query);
	}

	va_end(arglist);

	return false;
}

bool Helper::Query(const TCHAR* format, ...)
{
	va_list arglist;
	va_start(arglist, format);

	TCHAR query[BUFFER_SIZE] = { 0, };
	_vsntprintf_s(query, _countof(query), _countof(query), format, arglist);

	if ( false == m_Accessor.Connect(m_Account) )
	{
		va_end(arglist);
		Close();
		return false;
	}

	va_end(arglist);
	return Execute(query);
}


bool Helper::Query(const std::wstring& str)
{
	if ( false == m_Accessor.Connect(m_Account) )
	{
		Close();
		return false;
	}

	return Execute(str.c_str());
}


} // namespace Odbc

} // namespace db

} // namespace light