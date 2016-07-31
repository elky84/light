#include "stdafx.h"
#include "TestMode.h"

#include "Character.h"

#include "Input.h"

#include "TilePainter.h"

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
		LOG_ERROR(L"map setting failed. invalid map size.");
	}

	SetLength(m_TilePainter->Width() * TILE_SIZE, m_TilePainter->Height() * TILE_SIZE);

	m_Character.reset(new Character(m_direct_x, UNIT));
	m_Character->SetPosition(light::POSITION_INT(TILE_SIZE * 2, TILE_SIZE * 2));
	m_Character->Load();
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

	std::wstringstream stream_character;
	stream_character << L"CharacterX:" << m_Character->GetPosition().X() << L" CharacterY:" << m_Character->GetPosition().Y();
	DrawString(stream_character.str(), light::POSITION_INT(580, 50), 12);

	light::POSITION_INT position((LONG)m_Character->GetPosition().X(), (LONG)m_Character->GetPosition().Y());
	m_TilePainter->AdjustPosition(position);
	position -= light::POSITION_INT(0, 16); // 16은 캐릭터윗둥
	m_Character->Draw(position);
}

void TestMode::Logic()
{
	if(light::Input::Keyboard::Pressing(VK_UP))
	{
		m_Character->SetDirection(Direction::UP);
	}
	else if(light::Input::Keyboard::Pressing(VK_DOWN))
	{
		m_Character->SetDirection(Direction::DOWN);
	}
	else if(light::Input::Keyboard::Pressing(VK_LEFT))
	{
		m_Character->SetDirection(Direction::LEFT);
	}
	else if(light::Input::Keyboard::Pressing(VK_RIGHT))
	{
		m_Character->SetDirection(Direction::RIGHT);
	}

	m_Character->Logic(m_TilePainter.get()); // SetDirection에서 애니메이션 루프 다시 돌리므로, 이게 위에 있어야함.
	Process(light::POSITION_INT((LONG)m_Character->GetPosition().X() - m_x, (LONG)m_Character->GetPosition().Y() - m_y));
}