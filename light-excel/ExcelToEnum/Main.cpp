#include "stdafx.h"
#include "Main.h"

#include "ExcelHelper.h"
#include "EnumCodeGenerator.h"

#include "DirectoryUtil.h"
#include "FileUtil.h"
#include "ProcessUtil.h"

#include "XMLParser.h"

Main::Main(const std::wstring& strProgramName, const std::wstring& strArg)
: light::EnvironmentManager(strProgramName, strArg)
, m_strDestination(L".\\Header\\")
{
}

Main::~Main()
{
}

bool Main::initialize()
{
	if( false == LoadFromXML() )
	{
		LOG_ERROR(L"LoadFromXML() failed");
		return false;
	}

	light::Process::execute(L"PreEvent.bat");

	light::Directory::delete_directory(m_strDestination);
	light::Directory::create_directory(m_strDestination);

	for(VECTOR_EXCEL::iterator it = m_vectorExcel.begin(); it != m_vectorExcel.end(); ++it)
	{
		EXCEL_INFO& info = (*it);
		if( false == GenerateCode(info.File, info.vectorSheet) )
		{
			LOG_ERROR(L"%s. GenerateCode Failed. File[%s]", __FUNCTIONW__, info.File.c_str());
			return false;
		}
	}

	light::Process::execute(L"PostEvent.bat", m_strDestination);

	LOG_INFO(L"Code Generated Success");
	LOG_INFO(L"Press any key to quit.");
	getchar();
	stop();
	return true;
}

void Main::on_update()
{
	Sleep(1000);
}

void Main::release()
{
}

bool Main::GenerateCode(const std::wstring& strSrc, const VECTOR_SHEET_INFO& vectorSheet)
{
	light::excel::Helper ExcelHelper;
	if( FALSE == ExcelHelper.Open( strSrc ) )
	{
		LOG_ERROR(L"%s. ExcelHelper Open Failed. File[%s]", __FUNCTIONW__, strSrc.c_str());
		return false;
	}

	light::EnumCodeGenerator CodeGenerator;
	for(VECTOR_SHEET_INFO::const_iterator it = vectorSheet.begin(); it != vectorSheet.end(); ++it)
	{
		const SHEET_INFO& SheetInfo = *it;
		if( FALSE == ExcelHelper.ReadColumns(SheetInfo.NAME) )
		{
            LOG_ERROR(L"called");
			return false;
		}

		if( false == CodeGenerator.Open(strSrc, SheetInfo.NAME, SheetInfo.NAMESPACE) )
		{
            LOG_ERROR(L"called");
			return false;
		}

		/// 코드 제네레이터 클래스 작업 해서 넣어보자~
		const std::vector< light::excel::Helper::VECTOR_STRING >& vectorRowData = ExcelHelper.GetRowData();
		for( std::vector<light::excel::Helper::VECTOR_STRING>::const_iterator it = vectorRowData.begin(); it != vectorRowData.end(); ++it)
		{
			const light::excel::Helper::VECTOR_STRING& vectorString = (*it);
			if( false == CodeGenerator.Info( vectorString[0], _wtoi(vectorString[1].c_str()), vectorString[2] ) )
			{
                LOG_ERROR(L"called");
                return false;
			}
		}

		if( false == CodeGenerator.End() )
		{
			LOG_ERROR(L"called");
			return false;
		}

		std::wstring strResult = light::Directory::path_canonicalize(m_strDestination) + light::get_file_name_without_ext(light::get_file_name_without_path(strSrc, L"/")) + L".h";
		if( false == CodeGenerator.FileWrite(strResult, CodeGenerator.Doc()) )
		{
			LOG_ERROR(L"%s. FileWrite Failed. FileName[%s] Doc[%s]", __FUNCTIONW__, strResult.c_str(), CodeGenerator.Doc().c_str());
			return false;
		}

		LOG_INFO(L"Generated FileName [%s]", strResult.c_str());
	}
	return true;
}

bool Main::LoadFromXML()
{
	if( false == LoadExcelListFromXML() )
		return false;

	return true;
}

bool Main::LoadExcelListFromXML()
{
	m_vectorExcel.clear();
	light::XMLParser parser;
	std::wstring strArg = L"ExcelList.xml";
	if( false == parser.read_file(strArg) )
	{
		LOG_ERROR(L"%s. XMLParser File Open Failed.", __FUNCTIONW__, strArg.c_str());
		return false;
	}

	if( false == parser.execute(L"/ExcelList") ) 
	{
		LOG_ERROR(L"%s. XMLParser execute Failed.", __FUNCTIONW__, strArg.c_str());
		return false;
	}

	std::wstring file;
	parser.bind_elem(L"Excel");
	parser.bind_attrib(L"File", file);
	while(parser.next())
	{
		std::wstring strSheetName, strNameSpace;
		parser.bind_elem(L"Sheet");
		parser.bind_attrib(L"Namespace", strNameSpace);
		parser.bind_attrib(L"Name", strSheetName);
		VECTOR_SHEET_INFO vectorSheet;
		while(parser.next())
		{
			vectorSheet.push_back(SHEET_INFO(strNameSpace, strSheetName));
		}

		m_vectorExcel.push_back(EXCEL_INFO(file, vectorSheet));
	}
	return true;
}
