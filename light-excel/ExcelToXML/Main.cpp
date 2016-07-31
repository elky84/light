#include "stdafx.h"
#include "Main.h"

#include "ExcelHelper.h"
#include "ExcelToXMLCodeGenerator.h"

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

	light::excel::ExcelToXMLCodeGenerator CodeGenerator;
	for each(auto sheet_info in vectorSheet)
	{
		if( FALSE == ExcelHelper.ReadColumns(sheet_info.NAME) )
		{
			LOG_ERROR(L"%s ReadColumns(%s) Failed", __FUNCTIONW__, sheet_info.NAME.c_str());
			return false;
		}

		if( false == ExcelHelper.WriteXML(sheet_info.NAME) )
		{
			LOG_ERROR(L"%s WriteXML(%s) Failed", __FUNCTIONW__, sheet_info.NAME.c_str());
			return FALSE;
		}

		if( false == CodeGenerator.Open(strSrc, sheet_info.NAME, sheet_info.KEY) )
		{
			LOG_ERROR(L"called");
			return false;
		}

		const light::excel::Helper::VECTOR_COLUMNS& vectorColumns = ExcelHelper.GetColumns();
		for each(auto column in vectorColumns)
		{
			if( false == CodeGenerator.Variable(column.Type, column.Name) )
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
		std::wstring strSheetName, strKey;
		parser.bind_elem(L"Sheet");
		parser.bind_attrib(L"Key", strKey);
		parser.bind_attrib(L"Name", strSheetName);
		VECTOR_SHEET_INFO vectorSheet;
		while(parser.next())
		{
			vectorSheet.push_back(SHEET_INFO(strKey, strSheetName));
		}

		m_vectorExcel.push_back(EXCEL_INFO(file, vectorSheet));
	}
	return true;
}
