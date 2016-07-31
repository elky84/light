#include "stdafx.h"
#include "TestMode.h"

#include "boost/bind.hpp"
#include "ImeInput.h"

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
{
	m_EditBox[light::view::Gui::Direction::DOWN].Create(m_direct_x, L"data\\EditBox\\editbox.spr", light::POSITION_INT(0, 0), 15, D3DCOLOR_RGBA(255, 255, 255, 255), boost::bind(&TestMode::CarrageReturn, this));
	m_ListBox[light::view::Gui::Direction::DOWN].Create(directX, L"data\\ListBox\\ListBox.spr", L"data\\ListBox\\ScrollBar.spr", light::POSITION_INT(0, 20), light::view::Gui::Direction::DOWN);
	m_EditBox[light::view::Gui::Direction::DOWN].Append(&m_ListBox[light::view::Gui::Direction::DOWN]);

	m_ListBox[light::view::Gui::Direction::UP].Create(directX, L"data\\ListBox\\ListBox.spr", L"data\\ListBox\\ScrollBar.spr", light::POSITION_INT(0, 300), light::view::Gui::Direction::UP);
	m_EditBox[light::view::Gui::Direction::UP].Create(m_direct_x, L"data\\EditBox\\editbox.spr", light::POSITION_INT(0, 500), 15, D3DCOLOR_RGBA(255, 255, 255, 255), boost::bind(&TestMode::CarrageReturn, this));
	m_EditBox[light::view::Gui::Direction::UP].Append(&m_ListBox[light::view::Gui::Direction::UP]);
}

TestMode::~TestMode()
{

}

void TestMode::CarrageReturn()
{
	light::ImeInput::Instance()->Clear();
}

void TestMode::Begin()
{
	
	for each(auto& val in m_ListBox)
		val.Set();

	for each(auto& val in m_EditBox)
		val.Set();

	for(int i = 0; i < 30; ++i)
	{
		std::wstringstream stream;
		stream << i;

		for each(auto& val in m_ListBox)
		{
			val.Message(stream.str(), 15, D3DCOLOR_RGBA(255, 255, 255, 255), light::view::FreeType2::Align::LEFT);
		}
	}
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
