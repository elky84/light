#include "stdafx.h"
#include "TestMode.h"

#include "Input.h"

#include <boost/bind.hpp>

static const float MAX_PROGRESS = 1000.0f;

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
, m_Progress(0)
{
	m_ProgressBar[light::view::Gui::Direction::RIGHT].Create(m_direct_x, L"Data\\ProgressBar\\ProgressBar_Right.spr", light::POSITION_INT(200, 50), MAX_PROGRESS, 3, light::view::Gui::Direction::RIGHT);
	m_ProgressBar[light::view::Gui::Direction::RIGHT].SetProgress(1, 0);

	m_ProgressBar[light::view::Gui::Direction::LEFT].Create(m_direct_x, L"Data\\ProgressBar\\ProgressBar_Left.spr", light::POSITION_INT(320, 50), MAX_PROGRESS, 3, light::view::Gui::Direction::LEFT);
	m_ProgressBar[light::view::Gui::Direction::LEFT].SetProgress(1, 0);

	m_ProgressBar[light::view::Gui::Direction::DOWN].Create(m_direct_x, L"Data\\ProgressBar\\ProgressBar_Down.spr", light::POSITION_INT(440, 50), MAX_PROGRESS, 3, light::view::Gui::Direction::DOWN);
	m_ProgressBar[light::view::Gui::Direction::DOWN].SetProgress(1, 0);

	m_ProgressBar[light::view::Gui::Direction::UP].Create(m_direct_x, L"Data\\ProgressBar\\ProgressBar_Up.spr", light::POSITION_INT(480, 50), MAX_PROGRESS, 3, light::view::Gui::Direction::UP);
	m_ProgressBar[light::view::Gui::Direction::UP].SetProgress(1, 0);
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	for each(auto& val in m_ProgressBar)
		val.Set();
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
	if(light::Input::Keyboard::Pressing(VK_UP))
	{
		m_Progress += 10;
	}
	else if(light::Input::Keyboard::Pressing(VK_DOWN))
	{
		m_Progress -= 10;
	}
	else if(light::Input::Keyboard::Pressing(VK_LEFT))
	{
		m_Progress -= 50;
	}
	else if(light::Input::Keyboard::Pressing(VK_RIGHT))
	{
		m_Progress += 50;
	}
	
	m_Progress = std::max(0.0f, m_Progress);
	m_Progress = std::min(MAX_PROGRESS / 2, m_Progress);

	for each(auto& val in m_ProgressBar)
	{
		val.SetProgress(1, m_Progress);
		val.SetProgress(2, m_Progress);
	}
}