#include "light-excel.hpp"
#include "ExcelToXMLCodeGenerator.h"
#include "DirectoryUtil.h"

namespace light
{

namespace excel
{

bool ExcelToXMLCodeGenerator::Open(const std::wstring& FileName, const std::wstring& ClassName, const std::wstring& Key)
{
	m_ExcelLoader = ClassName + L"Data";
	m_Key = Key;
	return light::CppCodeGenerator::Open(FileName, ClassName, L"ExcelData");
}

bool ExcelToXMLCodeGenerator::Variable(const std::wstring& Type, const std::wstring& Name, const DWORD size /* = 0 */)
{
	if(Name == m_Key)
	{
		m_KeyType = Type;
	}
	return __super::Variable(Type, Name, size);
}

bool ExcelToXMLCodeGenerator::Write()
{
	if ( false == light::CppCodeGenerator::Write() )
	{
		LOG_ERROR(L"light::CppCodeGenerator::Write() Failed.");
		return false;
	}

	ClassBegin(m_Stream, m_ExcelLoader, L"light::excel::Data");
	Public(m_Stream);

	MethodBegin(m_Stream, L"virtual bool", L"Load");

	const std::wstring mapMethodName = L"Map";
	const std::wstring mapMemberVariable = Prefix(mapMethodName, L"m_");
	const std::wstring IntanceName = L"Instance";

	const std::wstring mapType = MapType(m_KeyType, m_ClassName);

	/// Instance Create
	MethodCode(m_Stream, Instance(m_ClassName, IntanceName));


	const std::wstring parser = L"parser";
	MethodCode(m_Stream, L"light::XMLParser " + parser + L";");

	std::wstring strPath = L"./XML/" + m_ClassName + L".XML";
	MethodCode(m_Stream, L"light::ScopeProfiler profiler(__FUNCTIONW__, 10, " + WrapQuot(strPath) + L");");

	MethodCode(m_Stream, L"if( false == " + MemberFunctionCall(parser, L"read_file", WrapQuot(strPath)) +  L")");
	MethodCode(m_Stream, L"{");
	MethodCode(m_Stream, std::tab + L"LOG_ERROR(L\"%s, Open() Failed. Path(" + strPath + L"\");");
	MethodCode(m_Stream, std::tab + L"return false;");
	MethodCode(m_Stream, L"}");
	EndLine(m_Stream);

	std::wstring strExecuteElem = L"/" + m_ClassName + L"List";
	MethodCode(m_Stream, L"if ( false == " + MemberFunctionCall(parser, L"execute", WrapQuot(strExecuteElem)) + L")");
	MethodCode(m_Stream, L"{");
	MethodCode(m_Stream, std::tab + L"LOG_ERROR(L\"%s, execute() Failed. " + strExecuteElem + L"\");");
	MethodCode(m_Stream, std::tab + L"return false;");
	MethodCode(m_Stream, L"}");
	EndLine(m_Stream);

	std::wstring strElem = L"Data";
	MethodCode(m_Stream, L"if ( false == " + MemberFunctionCall(parser, L"bind_elem", WrapQuot(strElem)) + L")");
	MethodCode(m_Stream, L"{");
	MethodCode(m_Stream, std::tab + L"LOG_ERROR(L\"%s, execute() Failed. " + strElem + L"\");");
	MethodCode(m_Stream, std::tab + L"return false;");
	MethodCode(m_Stream, L"}");
	EndLine(m_Stream);

	for (MAP_VARIABLE::iterator it = m_Variable.begin(); it != m_Variable.end(); ++it)
	{
		VARIABLE_INFO& Info = it->second;

		/// Bind
		std::wstring strGetMethodName = LinkToInstance( IntanceName, Info.NAME + L"Reference()");

		MethodCode(m_Stream, L"if ( false == " + MemberFunctionCall(parser, L"bind_attrib", WrapQuot(Info.NAME), strGetMethodName) + L")");
		MethodCode(m_Stream, L"{");
		MethodCode(m_Stream, std::tab + L"LOG_ERROR(L\"bind_attrib() Failed. " + strGetMethodName + L"\");");
		MethodCode(m_Stream, std::tab + L"return false;");
		MethodCode(m_Stream, L"}");
		EndLine(m_Stream);
	}

	/// Fetch
	MethodCode(m_Stream, L"while (" + MemberFunctionCall(parser, L"next" ) + L")");
	MethodCode(m_Stream, L"{");
	MethodCode(m_Stream, std::tab + MemberFunctionCall(mapMemberVariable, L"insert", mapType + L"::value_type(" + MemberFunctionCall(IntanceName, m_Key), IntanceName + L")" ) + L";");
	MethodCode(m_Stream, L"}");
	EndLine(m_Stream);

	MethodCode(m_Stream, L"return true;");
	MethodEnd(m_Stream);

	/// Get Method (Find)
	MethodBegin(m_Stream, L"bool", L"Get", TypeToReference(m_KeyType), L"key", TypeToReference(m_ClassName), L"Instance");

	MethodCode(m_Stream, L"auto it = " + MemberFunctionCall(mapMemberVariable, L"find", L"key") + L";");
	MethodCode(m_Stream, L"{");
	MethodCode(m_Stream, std::tab + L"return false;");
	MethodCode(m_Stream, L"}");

	MethodCode(m_Stream, L"Instance = it->second;");
	MethodCode(m_Stream, L"return true;");

	MethodEnd(m_Stream);


	/// Map Get Method
	GetMethod(m_Stream, TypeToReference(mapType), mapMethodName, mapMemberVariable);

	/// private
	Private(m_Stream);

	/// Map Declare
	Declare(m_Stream, mapType, mapMemberVariable);

	// Class 선언 종료
	ClassEnd(m_Stream);

	EndLine(m_Stream);

	GlobalDeclare(m_Stream, m_ExcelLoader, m_ExcelLoader + L"Instance");
	return true;
}

} // namespace excel

} // namespace light