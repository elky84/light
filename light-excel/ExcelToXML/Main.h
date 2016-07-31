#pragma once

#include "EnvironmentManager.h"
#include "GuiPrompt.h"

struct SHEET_INFO
{
public:
	std::wstring KEY;
	std::wstring NAME;

public:
	SHEET_INFO(const std::wstring& key, const std::wstring& name)
		: KEY(key)
		, NAME(name)
	{

	}
};

class Main : public light::EnvironmentManager
{
private:
	typedef std::vector<SHEET_INFO> VECTOR_SHEET_INFO;

	struct EXCEL_INFO
	{
	public:
		std::wstring File;
		VECTOR_SHEET_INFO vectorSheet;

	public:
		EXCEL_INFO(const std::wstring& file, const VECTOR_SHEET_INFO& vectorsheet)
			: File(file)
			, vectorSheet(vectorsheet)
		{
		}
	};

	typedef std::vector<EXCEL_INFO> VECTOR_EXCEL;

public:
	Main(const std::wstring& strProgramName, const std::wstring& strArg);
	~Main();

	virtual bool initialize();
	virtual void on_update();
	virtual void release();

private:
	bool LoadFromXML();

	bool LoadExcelListFromXML();

private:
	bool GenerateCode(const std::wstring& strSrc, const VECTOR_SHEET_INFO& vectorSheet);

private:
	VECTOR_EXCEL m_vectorExcel;

	std::wstring m_strDestination;
};
