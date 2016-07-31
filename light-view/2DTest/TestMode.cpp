#include "stdafx.h"
#include "TestMode.h"

#include "Input.h"

#include <boost/bind.hpp>

static const float MAX_PROGRESS = 1000.0f;

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
{
	m_Animation.Load(m_direct_x, L"Data\\Menu\\Menu.ani");

	m_Button[0].Create(m_direct_x, L"data\\Menu\\Button_Single.spr", boost::bind(&TestMode::OnSingleModeButtonClick, this), light::Position<int>(0, 200), L"data\\Sound\\GUI\\Click.wav", L"data\\Sound\\GUI\\Over.wav");
	m_Button[1].Create(m_direct_x, L"data\\Menu\\Button_Multi.spr", NULL, light::Position<int>(0, 300), L"data\\Sound\\GUI\\Click.wav", L"data\\Sound\\GUI\\Over.wav");
	m_Button[2].Create(m_direct_x, L"data\\Menu\\Button_Exit.spr", boost::bind(&TestMode::OnExitButtonClick, this),	light::Position<int>(0, 400), L"data\\Sound\\GUI\\Click.wav", L"data\\Sound\\GUI\\Over.wav");
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	for(size_t i = 0; i < _countof(m_Button); i++)
		m_Button[i].Set();
}

void TestMode::End()
{
}

void TestMode::Render()
{
	m_Animation.DrawLoop();
}

void TestMode::RenderPostUI()
{
}

void TestMode::Logic()
{

}

bool TestMode::OnSingleModeButtonClick()
{
	return true;
}

bool TestMode::OnExitButtonClick()
{
	PostQuitMessage(WM_QUIT);
	return true;
}
