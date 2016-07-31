#include "stdafx.h"
#include "TestMode.h"

#include "Input.h"

#include <boost/bind.hpp>

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
{
	m_Placeholder[0].Create(m_direct_x, L"Data\\placeholder.spr", light::POSITION_INT(400, 50));
	m_Placeholder[1].Create(m_direct_x, L"Data\\placeholder.spr", light::POSITION_INT(400, 150));
	m_Placeholder[2].Create(m_direct_x, L"Data\\placeholder.spr", light::POSITION_INT(400, 250));

	m_Button[0].Create(m_direct_x, L"Data\\button.spr", boost::bind(&TestMode::Test, this, L"button1"), light::POSITION_INT(100, 200), L"data\\Click.wav", L"data\\Over.wav");
	m_Button[0].Property(light::view::Gui::PROPERTY::MOVABLE_PLACEHOLDER);

	m_Button[1].Create(m_direct_x, L"Data\\button.spr", boost::bind(&TestMode::Test, this, L"button2"), light::POSITION_INT(100, 300), L"data\\Click.wav", L"data\\Over.wav");
	m_Button[1].Property(light::view::Gui::PROPERTY::MOVABLE_PLACEHOLDER);

	m_Button[2].Create(m_direct_x, L"Data\\button.spr", boost::bind(&TestMode::Test, this, L"free"), light::POSITION_INT(100, 400), L"data\\Click.wav", L"data\\Over.wav");
	m_Button[2].Property(light::view::Gui::PROPERTY::MOVABLE);
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	for each(auto& val in m_Placeholder)
	{
		val.Set();
	}

	for each(auto& val in m_Button)
	{
		val.Set();
	}

	m_Button[0].Message(L"button1", 30, D3DCOLOR_RGBA(255, 255, 255, 255));
	m_Button[1].Message(L"button2", 30, D3DCOLOR_RGBA(255, 255, 255, 255));
	m_Button[2].Message(L"free", 30, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void TestMode::End()
{
}

void TestMode::Render()
{
}

void TestMode::RenderPostUI()
{
}

void TestMode::Logic()
{
	
}

bool TestMode::Test(const std::wstring& str)
{
	::MessageBox(NULL, str.c_str(), L"Test", MB_OK);
	return true;
}