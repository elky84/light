#include "stdafx.h"
#include "TestMode.h"
#include "TilePainter.h"

#include "Input.h"
#include "Character.h"

#include <boost/bind.hpp>

static const int TILE_SIZE = 32;
static const int MOVEABLE_TILE = 3;

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
, light::view::Scroll(WINDOW_WIDTH, WINDOW_HEIGHT, TILE_SIZE, TILE_SIZE)
, m_Menu(MENU::NONE)
{
	m_Cursor.Load(m_direct_x, L".\\Map\\Cursor.spr", L"Cursor.png");
	m_Moveable.Load(m_direct_x, L".\\Map\\Alpha.spr", L"Black.png");
	m_Characters[0].Load(m_direct_x, L".\\Character\\Character.spr", L"박지성.png");
	m_Characters[1].Load(m_direct_x, L".\\Character\\Character.spr", L"호날두.png");

	m_Buttons[0].Create(m_direct_x, L".\\Gui\\button_attack.spr", boost::bind(&TestMode::Menu, this, MENU::ATTACK), light::POSITION_INT(0, 0));
	m_GuiGruop.Add(&m_Buttons[0]);

	m_Buttons[1].Create(m_direct_x, L".\\Gui\\button_move.spr", boost::bind(&TestMode::Menu, this, MENU::MOVE), light::POSITION_INT(0, 0));
	m_GuiGruop.Add(&m_Buttons[1]);

	m_Buttons[2].Create(m_direct_x, L".\\Gui\\button_info.spr", boost::bind(&TestMode::Menu, this, MENU::INFO), light::POSITION_INT(0, 0));
	m_GuiGruop.Add(&m_Buttons[2]);

	LButtonClickCallback(boost::bind(&TestMode::OnLButtonClick, this));
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	m_TilePainter.reset(new light::view::TilePainterScroll(m_x, m_y, light::RECT_INT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)));
	m_TilePainter->Load(m_direct_x, L".\\Map\\Test.map");
	SetLength(m_TilePainter->Width() * TILE_SIZE, m_TilePainter->Height() * TILE_SIZE);

	m_Character.reset(new Character(5, 3, m_Characters[0]));
}

void TestMode::End()
{
}

void TestMode::Render()
{
	m_TilePainter->Render();

	std::wstringstream stream;
	stream << L"MouseX:" << m_position.X() << L" MouseY:" << m_position.Y();

	DrawString(stream.str(), light::POSITION_INT(600, 20), 12);

	std::wstringstream stream_scroll;
	stream_scroll << L"ScrollX:" << m_x << L" ScrollY:" << m_y;

	DrawString(stream_scroll.str(), light::POSITION_INT(600, 50), 12);

	light::POSITION_INT position( m_Character->X() * m_Character->Width(), m_Character->Y() *  m_Character->Height());
	m_TilePainter->AdjustPosition(position);
	m_Character->Draw(0, position, 0);

	if(m_Menu == MENU::MOVE)
	{
		DrawArea();
	}

	light::POSITION_INT CheckPosition = light::Input::Mouse::Pos();
	CheckPosition.X((CheckPosition.X() + m_x) / TILE_SIZE);
	CheckPosition.Y((CheckPosition.Y() + m_y) / TILE_SIZE);

	light::POSITION_INT CursorPosition( CheckPosition.X() * TILE_SIZE, CheckPosition.Y() *  TILE_SIZE);
	m_TilePainter->AdjustPosition(CursorPosition);
	m_Cursor.DrawSprite(0, CursorPosition, 0);
}

void TestMode::DrawArea()
{
	if(m_PickCharacter.get() == NULL)
		return;

	for(int x = -MOVEABLE_TILE; x < MOVEABLE_TILE; x++)
	{
		for(int y = -MOVEABLE_TILE; y < MOVEABLE_TILE; y++)
		{
			if(abs(x) + abs(y) == 0)
				continue;

			if(abs(x) + abs(y) < MOVEABLE_TILE)
			{
				light::POSITION_INT position( (m_Character->X() + x) * m_Character->Width(), (m_Character->Y() + y) *  m_Character->Height());
				m_TilePainter->AdjustPosition(position);
				m_Moveable.DrawSprite(0, position, 0);
			}
		}
	}
}

void TestMode::RenderPostUI()
{

}

bool TestMode::OnLButtonClick()
{
	light::POSITION_INT CheckPosition = light::Input::Mouse::Pos();
	CheckPosition.X(CheckPosition.X() + m_x);
	CheckPosition.Y(CheckPosition.Y() + m_y);

	const int TileX = CheckPosition.X() / TILE_SIZE;
	const int TileY = CheckPosition.Y() / TILE_SIZE;

	if( m_PickCharacter.get() )
	{
		if( m_Menu == MENU::MOVE )
		{
			int fGab = abs(m_PickCharacter->X() - TileX) + abs(m_PickCharacter->Y() - TileY);
			if( fGab < MOVEABLE_TILE)
			{
				m_PickCharacter->Move(TileX, TileY);
			}

			m_Menu = MENU::NONE;
			m_PickCharacter.release();
		}
	}
	else
	{
		if( TileX == m_Character->X() && TileY == m_Character->Y())
		{
			m_PickCharacter.reset(m_Character);
			m_TilePainter->AdjustPosition(CheckPosition);
			m_GuiGruop.Set(CheckPosition);
			return true;
		}

		m_Menu = MENU::NONE;
		m_GuiGruop.Release();
		m_PickCharacter.release();
	}
	return true;
}

void TestMode::Logic()
{
	m_position = light::Input::Mouse::Pos();
	if( true == Process(m_position) )
	{
		m_GuiGruop.Release();
	}
}

bool TestMode::Menu(MENU::type Menu)
{
	m_Menu = Menu;
	if(m_Menu == MENU::INFO ||
		m_Menu == MENU::ATTACK )
	{
		m_Menu = MENU::NONE;
		m_GuiGruop.Release();
		m_PickCharacter.release();
	}
	else
	{
		m_GuiGruop.Release();
	}
	return true;
}