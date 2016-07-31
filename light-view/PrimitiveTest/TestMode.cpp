#include "stdafx.h"
#include "TestMode.h"

#include <boost/bind.hpp>


TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
{
	m_line.Create(m_direct_x, D3DCOLOR_XRGB(0, 0, 0));
	m_line.SetPosition(light::POSITION_INT(600, 400), light::POSITION_INT(600, 500));

	m_rect.Create(directX, D3DCOLOR_XRGB(0, 0, 0));
	m_rect.Rect(light::RECT_INT(600, 200, 700, 300));
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{

}

void TestMode::End()
{
}

void TestMode::Render()
{
	m_line.Render();
	m_rect.Render();
}

void TestMode::RenderPostUI()
{
}

void TestMode::Logic()
{
	
}
