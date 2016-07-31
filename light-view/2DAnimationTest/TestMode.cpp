#include "stdafx.h"
#include "TestMode.h"

#include "Character.h"
#include "Input.h"
#include "TilePainter.h"
#include "DirectX.h"

#include <boost/bind.hpp>

#pragma comment(lib, "winmm.lib")

static const INT UNIT = 4;

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
	: light::view::Mode(0, directX)
{
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	m_Character.reset(new Character(m_direct_x, UNIT));
	m_Character->SetPosition(light::POSITION_INT(TILE_SIZE * 2, TILE_SIZE * 2));
	m_Character->Load();
}

void TestMode::End()
{
}

void TestMode::Render()
{
}

void TestMode::RenderPostUI()
{
	std::wstringstream stream_character;
	stream_character << L"CharacterX:" << m_Character->GetPosition().X() << L" CharacterY:" << m_Character->GetPosition().Y();
	DrawString(stream_character.str(), light::POSITION_INT(580, 50), 12);

	light::POSITION_INT position((LONG)m_Character->GetPosition().X(), (LONG)m_Character->GetPosition().Y());
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

	m_Character->Logic();
}