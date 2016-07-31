#pragma once

#include <sql.h>
#include <sqlext.h>

#include "MSSQLType.h"

namespace light
{

namespace excel
{

struct COLUMN_INFO
{
public:
	enum TYPE
	{
		UNKNOWN = 0,
		INTEGER,
		DOUBLE,
		STRING,
	};

public:
	COLUMN_INFO()
	{
	}

	COLUMN_INFO(const TYPE type, const std::wstring& name, const double Max)
		: Type(TypeToString(type, Max))
		, Name(name)
	{
	}

public:
	std::wstring TypeToString(const TYPE type, const double Max)
	{
		switch(type)
		{
		case INTEGER:
			return Max <= (std::numeric_limits<int>::max)() ? L"int" : L"INT64";
		case DOUBLE:
			return Max <= (std::numeric_limits<float>::max)() ? L"float" : L"double";
		default:
			return L"std::wstring";
		}
	}

public:
	std::wstring Type;

	std::wstring Name;
};

class Helper
{
public:
	typedef std::vector<COLUMN_INFO> VECTOR_COLUMNS;
	typedef std::vector<std::wstring> VECTOR_STRING;

public:	
	Helper();
	~Helper();

public:
	BOOL Open(const std::wstring& strFilePath);

	BOOL ReadColumns(const std::wstring& strSheet);

	const COLUMN_INFO::TYPE Decide(const COLUMN_INFO::TYPE Current, const std::wstring& strRowData);

	bool WriteXML(const std::wstring& strSheet);

	const VECTOR_COLUMNS& GetColumns()
	{
		return m_vectorColumns;
	}

public: // 쿼리 실행과 관련된 메소드 모음
	BOOL Query(const std::wstring& strSheet);

	template <typename T>
	BOOL Bind(const SQLSMALLINT Col, T& t)
	{
		if( SQL_SUCCESS != SQLBindCol(m_hStmt, Col, GetCType(t), &t,  sizeof(T), NULL) )
			return FALSE;

		return TRUE;
	}

	BOOL Bind(const SQLSMALLINT Col, WCHAR* str, size_t strLength)
	{
		if( SQL_SUCCESS != SQLBindCol(m_hStmt, Col, SQL_C_WCHAR, str, strLength - 1, NULL) )
			return FALSE;

		return TRUE;
	}

	/// 컴파일 오류를 내기 위한 선언
	BOOL Bind(const SQLSMALLINT Col, std::wstring& str);

	/// 컴파일 오류를 내기 위한 선언
	BOOL Bind(const SQLSMALLINT Col, WCHAR* str);

	SQLRETURN Fetch();

	SQLRETURN Close();

	static std::wstring SheetNameToODBCStyle(const std::wstring& strSheet)
	{
		return L"[" + strSheet + L"$]";
	}

	const std::wstring& FilePath() const
	{
		return m_strFilePath;
	}

	const std::vector< VECTOR_STRING >& GetRowData() const
	{
		return m_vectorRowData;
	}

private: // 내부에서만 사용되는 private 메소드
	BOOL Initialize();

	void Uninitialize();

	BOOL Diagnostic(SQLSMALLINT handle_type, SQLHANDLE handle);

private:
	std::wstring m_strFilePath;

	VECTOR_COLUMNS m_vectorColumns;

	std::vector< VECTOR_STRING > m_vectorRowData;
	std::wstring m_strQuery;

	SQLHENV m_hEnv;

	SQLHDBC	m_hDbc;

	SQLHSTMT m_hStmt;
};

} // namespace excel

} // namespace light