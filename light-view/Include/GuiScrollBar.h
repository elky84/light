#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiScrollBar : public Gui
{
public:
	enum class MOUSE_POSITION
	{
		NONE = -1,
		UP_ARROW,
		BAR_BG,
		BAR,
		DOWN_ARROW,
	};

	struct FRAME
	{
		enum Type
		{
			NONE = 0,
			ACTIVE,
			BG,
		};
	};

	GuiScrollBar();

	virtual ~GuiScrollBar();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, const light::RECT_INT& Drawing, const Direction::Type direction, Gui::Callback callback = NULL);

	void SetContent(light::RECT_INT& Rect);

	virtual light::RECT_INT GetRect();

private:
	virtual void OnDraw();

	virtual void OnLogic();

	virtual GUI_STATE::Type Check();

	virtual void OnAcquireFocus();
	
	virtual void MouseEvent();

private:
	void OnClick();

	void Draw(MOUSE_POSITION mouse_pos);

	void SetDrawingRect();

private:
	INT m_mouse_y;

	INT m_arrow_height;
	INT m_bar_bg_height;
	INT m_bar_height;

	INT m_bar_y_position;

	light::RECT_INT m_rect;

	light::RECT_INT m_contents;

	light::weak_raw_ptr<light::RECT_INT> m_drawing;

	Direction::Type m_direction;
};

} // namespace view

} // namespace view
