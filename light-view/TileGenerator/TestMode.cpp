#include "stdafx.h"
#include "TestMode.h"

#include "TilePainter.h"
#include "Input.h"

#include <boost/bind.hpp>

#pragma comment(lib, "winmm.lib")

static const INT UNIT = 4;

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
	: light::view::Mode(0, directX)
	, light::view::Scroll(WINDOW_WIDTH, WINDOW_HEIGHT, UNIT, TILE_SIZE * 4)
{
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	m_TilePainter.reset(new light::view::TilePainterScroll(m_x, m_y, light::RECT_INT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)));
	m_TilePainter->Load(m_direct_x, L".\\Map\\Test.map");

	if(m_TilePainter->Width() * TILE_SIZE < MAX_SCROLL_X || 
		m_TilePainter->Height() * TILE_SIZE < MAX_SCROLL_Y )
	{
		LOG_ERROR(L"맵 설정 파일 크기 오류.");
	}

	SetLength(m_TilePainter->Width() * TILE_SIZE, m_TilePainter->Height() * TILE_SIZE);
}

void TestMode::End()
{
}

void TestMode::Render()
{
	m_TilePainter->Render();
}

void TestMode::RenderPostUI()
{
	std::wstringstream stream_scroll;
	stream_scroll << L"ScrollX:" << m_x << L" ScrollY:" << m_y;
	DrawString(stream_scroll.str(), light::POSITION_INT(580, 20), 12);
}

void TestMode::Logic()
{	
	m_position = light::Input::Mouse::Pos();
	Process(m_position);
}