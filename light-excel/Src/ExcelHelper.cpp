#include "light-excel.hpp"
#include "ExcelHelper.h"

#include <odbcinst.h> //SQLGetInstalledDrivers 함수 사용을 위해..
#include <odbcss.h>

#include "XMLWriter.h"

namespace light
{

namespace excel
{

Helper::Helper()
: m_hEnv(NULL)
, m_hDbc(NULL)
, m_hStmt(NULL)
{
}

Helper::~Helper()
{
	Uninitialize();
}

BOOL Helper::Initialize()
{
	// 환경 핸들을 할당하고 버전 속성을 설정한다.
	if( SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) )
	{
		return FALSE;
	}

	if( SQL_SUCCESS != SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) )	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		return FALSE;
	}

	if(!SQL_SUCCEEDED(SQLSetEnvAttr(m_hEnv, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER))) 
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		return FALSE;	
	}

	// 연결 핸들을 할당하고 연결한다.
	if( SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) )
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		return FALSE; 
	}

	return TRUE; 
}

void Helper::Uninitialize()
{
	if(m_hStmt)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	}

	if(m_hDbc)
	{
		SQLDisconnect(m_hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	}

	if(m_hEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	}
}

BOOL Helper::Open(const std::wstring& strFilePath)
{
	if( FALSE == Initialize() )
	{
		return FALSE;
	}

	if(m_hDbc == NULL)
	{
		LOG_INFO(L"invalid ODBC Handle.");
		return FALSE;
	}

	WCHAR szConnectionString[MAX_PATH] = {0, };
	swprintf_s(szConnectionString, L"DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=%s", strFilePath.c_str());

	short cbOut = 0;
	WCHAR szOut[MAX_PATH] = {0, };
	SQLRETURN Ret = SQLDriverConnect(m_hDbc, NULL, (SQLWCHAR*)szConnectionString, sizeof(szConnectionString), (SQLWCHAR*)szOut, sizeof(szOut), &cbOut, SQL_DRIVER_NOPROMPT);
	if(Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

	if( SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt) )
	{
		return FALSE;
	}
	m_strFilePath = strFilePath;
	return TRUE;
}

BOOL Helper::Query(const std::wstring& strQuery)
{
	m_strQuery = strQuery;
	if( FALSE == SQL_SUCCEEDED(SQLExecDirect(m_hStmt, (SQLWCHAR*)m_strQuery.c_str(), SQL_NTS)) )
	{
		Diagnostic(SQL_HANDLE_STMT, m_hStmt);
		return FALSE; 
	}
	return TRUE;
}

SQLRETURN Helper::Fetch()
{
	SQLRETURN retVal = SQLFetch(m_hStmt);
	if( TRUE == SQL_SUCCEEDED(retVal) )
		return SQL_SUCCESS;

	if(retVal != SQL_NO_DATA)
	{
		Diagnostic(SQL_HANDLE_STMT, m_hStmt);
		return SQL_ERROR;
	}

	if( FALSE == SQL_SUCCEEDED(Close()))
	{
		Diagnostic(SQL_HANDLE_STMT, m_hStmt);
		return SQL_ERROR;
	}
	return SQL_NO_DATA;
}

SQLRETURN Helper::Close()
{
	return SQLCloseCursor(m_hStmt);
}

BOOL Helper::ReadColumns(const std::wstring& strSheet)
{
	m_strQuery = L"select * from " + SheetNameToODBCStyle(strSheet);
	if( FALSE == SQL_SUCCEEDED(SQLExecDirect(m_hStmt, (SQLWCHAR*)m_strQuery.c_str(), SQL_NTS))  )
	{
		Diagnostic(SQL_HANDLE_STMT, m_hStmt);
		return FALSE; 
	}

	SQLSMALLINT ColumnCount = 0;
	if( FALSE == SQL_SUCCEEDED(SQLNumResultCols(m_hStmt, &ColumnCount)) )
	{
		Diagnostic(SQL_HANDLE_STMT, m_hStmt);
		return FALSE; 
	}

	std::vector<COLUMN_INFO::TYPE> vectorType(ColumnCount);
	std::vector<double> vectorSize(ColumnCount);
	for(;;)
	{
		SQLRETURN retVal = SQLFetch(m_hStmt);
		if(retVal == SQL_NO_DATA)
			break;

		if( FALSE == SQL_SUCCEEDED(retVal) )
		{
			Diagnostic(SQL_HANDLE_STMT, m_hStmt);
			return FALSE; 
		}

		m_vectorRowData.resize(m_vectorRowData.size() + 1);
		for(unsigned short n = 0; n < ColumnCount; ++n)
		{	
			WCHAR szRowData[MAX_PATH] = {0, };
			if( FALSE == SQL_SUCCEEDED(SQLGetData(m_hStmt, n + 1, SQL_C_WCHAR, szRowData, sizeof(szRowData), NULL)) )
			{
				Diagnostic(SQL_HANDLE_STMT, m_hStmt);
				return FALSE;
			}

			m_vectorRowData[m_vectorRowData.size() - 1].push_back(szRowData);
			switch( vectorType[n] )
			{
			case COLUMN_INFO::STRING:  // 한번이라도, 문자열 형식이 포함되어 있다면, 변경안하고 이대로 고정한다.
				{
					vectorSize[n] = std::max(vectorSize[n], (double)_tcslen(szRowData));
				}
				break;
			case COLUMN_INFO::INTEGER:
			case COLUMN_INFO::DOUBLE:
			case COLUMN_INFO::UNKNOWN:
			default:
				{
					vectorType[n] = Decide(vectorType[n], szRowData);
					vectorSize[n] = std::max(vectorSize[n], (double)_ttof(szRowData));
				}
				break;
			}
		}
	}

	for(unsigned short n = 0; n < ColumnCount; ++n)
	{
		SQLINTEGER size = 0;
		WCHAR szColName[SQL_MAX_COLUMN_NAME_LEN] = {0, };
		if( FALSE == SQL_SUCCEEDED( ::SQLColAttribute(m_hStmt, n + 1, SQL_COLUMN_NAME, &szColName, sizeof(szColName), NULL, NULL))  )
		{
			Diagnostic(SQL_HANDLE_STMT, m_hStmt);
			return FALSE; 
		}

		if( FALSE == SQL_SUCCEEDED( ::SQLColAttribute(m_hStmt, n + 1, SQL_COLUMN_LENGTH, NULL, 0, NULL, &size))  )
		{
			Diagnostic(SQL_HANDLE_STMT, m_hStmt);
			return FALSE; 
		}

		m_vectorColumns.push_back(COLUMN_INFO(vectorType[n], szColName, vectorSize[n]));
	}

	return SQL_SUCCEEDED(Close());
}

BOOL Helper::Diagnostic(SQLSMALLINT handle_type, SQLHANDLE handle)
{
	SQLSMALLINT cRecNmbr = 1;
	for(;;)
	{
		SQLINTEGER	NativeError = 0L;
		SWORD		pcbErrorMsg = 0;
		SQLWCHAR	szSqlState[256] = {0, };
		SQLWCHAR	szErrorMsg[1024] = {0, };
		RETCODE retVal = SQLGetDiagRec(handle_type, handle, cRecNmbr, szSqlState, &NativeError, szErrorMsg, sizeof(szErrorMsg)-1, &pcbErrorMsg);
		if( FALSE == SQL_SUCCEEDED(retVal) )
			break;

		LOG_ERROR(L"%s ErrCode(%s) ErrMsg(%s) FilePath(%s) Query(%s) pfNativeError(%d)", __FUNCTIONW__, (WCHAR*)szSqlState, (WCHAR*)szErrorMsg, m_strFilePath.c_str(), m_strQuery.c_str(), NativeError);
		cRecNmbr++;
	}
	return TRUE;
}

bool Helper::WriteXML(const std::wstring& strSheet)
{
	light::XMLWriter generator;
	if(false == generator.open(std::wstring(L"./XML/") + strSheet + L".xml", strSheet + L"List"))
		return false;

	for each(const VECTOR_STRING& vectorString in m_vectorRowData)
	{
		generator.elem(L"Data");
		for(size_t n = 0; n < m_vectorColumns.size(); ++n)
		{
			generator.attribute(m_vectorColumns[n].Name, vectorString[n]);
			generator.elem_end();
		}
	}
	return true;
}


const COLUMN_INFO::TYPE Helper::Decide(const COLUMN_INFO::TYPE Current, const std::wstring& strRowData)
{
	if( false == light::str::is_digit(strRowData.c_str()) ) // 정수형이 아니면 스트링으로 처리한다
	{
		return COLUMN_INFO::STRING;
	}
	else
	{
		if(Current == COLUMN_INFO::DOUBLE) // DOUBLE 이라면 기존 값을 유지한다.
			return Current;

		return _ttof(strRowData.c_str()) != _wtoi64(strRowData.c_str()) ? COLUMN_INFO::DOUBLE : COLUMN_INFO::INTEGER;
	}
}

} // namespace excel

} // namespace light