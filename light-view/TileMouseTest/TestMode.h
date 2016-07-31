#pragma once

#include "Mode.h"
#include "Sprite2D.h"
#include "GuiButton.h"
#include "GuiGroup.h"

#include "Scroll.h"

namespace light
{

namespace view
{

class TilePainter;
class DirectX;

} // namespace view

} // namespace view

struct MENU
{
	enum type
	{
		NONE = -1,
		ATTACK = 0,
		MOVE,
		INFO,
	};
};

class Character;

class TestMode
	: public light::view::Mode
	, public light::view::Scroll
{
public:
	TestMode(light::weak_raw_ptr<light::view::DirectX> directX);

	virtual ~TestMode();

	virtual void Begin();

	virtual void End();

	virtual void Render();

	virtual void RenderPostUI();

	virtual void Logic();

private:
	void DrawArea();

private:
	bool Menu(MENU::type Menu);

	bool OnLButtonClick();

private:

	light::view::Sprite2D m_Cursor;

	light::view::Sprite2D m_Select;

	light::view::Sprite2D m_Moveable;

	light::view::Sprite2D m_Characters[2];

	light::view::GuiButton m_Buttons[3];

	std::unique_ptr<Character> m_Character;

	light::weak_raw_ptr<Character> m_PickCharacter;

	std::unique_ptr<light::view::TilePainter> m_TilePainter;

	light::POSITION_INT m_position;

	light::view::GuiGroup m_GuiGruop;

	MENU::type m_Menu;
};
