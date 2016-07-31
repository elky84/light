#include "light-view.h"
#include "GuiScrollBar.h"

#include "Input.h"

namespace light
{

namespace view
{

GuiScrollBar::GuiScrollBar()
	: m_bar_y_position(0)
	, m_mouse_y(0)
	, m_arrow_height(0)
	, m_bar_bg_height(0)
	, m_bar_height(0)
{
}

GuiScrollBar::~GuiScrollBar()
{
}

bool GuiScrollBar::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, const light::RECT_INT& Drawing, const Direction::Type direction, Gui::Callback callback)
{
	m_direct_x = directX;
	SetPosition(Position);
	SetCallback(callback);
	m_direction = direction;

	if( false == Load(file_name) )
		return false;

	m_drawing.reset(&Drawing);
	m_rect = m_sprite.GetRect(0, 0);
	m_bar_height = m_rect.Height();
	m_bar_bg_height = m_rect.Height();
	m_arrow_height = m_sprite.GetRect(1, 0).Height();
	m_rect.Bottom(m_rect.Bottom() + m_arrow_height * 2);
	return true;
}

void GuiScrollBar::SetContent(light::RECT_INT& Rect)
{
	m_contents = Rect;
	if( m_rect.Height() < m_contents.Height() ) 
	{
		m_bar_height = (INT)((float)m_rect.Height() / m_contents.Height() * m_bar_bg_height);
		if(m_direction == Direction::UP)
		{
			m_bar_y_position = m_bar_bg_height - m_bar_height;
		}
		else
		{
			m_bar_y_position = 0;
		}

		SetDrawingRect();

		light::RECT_FLOAT OriginalRect = GetFrame(0, 1).OriginalRect();
		OriginalRect.Bottom((float)m_bar_height);

		GetFrame(0, 1).Rect(OriginalRect); // 1, 2 둘중에머가 쓰일지 모르니 양쪽다 다 같은 크기 적용.
		GetFrame(0, 2).Rect(OriginalRect);
	}
}

void GuiScrollBar::SetDrawingRect()
{
	m_drawing->Top( (INT)((float)m_bar_y_position / m_bar_bg_height * m_contents.Height() ));
	m_drawing->Bottom(m_drawing->Top() + m_rect.Height());
}

void GuiScrollBar::OnDraw()
{
	if( m_mouse_y == -1 )
	{
		// 마우스가 위에 없을 때
		Draw(MOUSE_POSITION::NONE);
	}
	else if( m_mouse_y < m_arrow_height) 
	{
		// 마우스가 화살표 위에쪽 부분에 있을 때
		Draw(MOUSE_POSITION::UP_ARROW);
	}
	else if( m_mouse_y < m_arrow_height + m_bar_bg_height )
	{
		// 마우스가 그리기 영역 내에 있을 떄
		if( m_bar_y_position + m_arrow_height <= m_mouse_y &&
			m_mouse_y <= m_bar_height + m_bar_y_position + m_arrow_height )
		{
			Draw(MOUSE_POSITION::BAR);
		}
		else
		{
			Draw(MOUSE_POSITION::BAR_BG);
		}
	}
	else
	{
		// 마우스가 화살표 아래쪽 부분에 있을 때
		Draw(MOUSE_POSITION::DOWN_ARROW);
	}
}

void GuiScrollBar::Draw(MOUSE_POSITION mouse_pos)
{
	light::POSITION_INT Position = GetPosition();
	// 위쪽 화살표 그리기
	DrawSprite(1, Position, mouse_pos == MOUSE_POSITION::UP_ARROW ? FRAME::ACTIVE : FRAME::NONE);

	// 스크롤바 배경 그리기
	Position.Y(Position.Y() + m_arrow_height);
	DrawSprite(0, Position, 0); // 배경은 index값에 상관없이 무조건 disable

	if( m_bar_bg_height != m_bar_height)
	{
		DrawSprite(0, Position + light::POSITION_INT(0, m_bar_y_position), mouse_pos == MOUSE_POSITION::BAR ? FRAME::ACTIVE : FRAME::BG);
	}

	// 아래쪽 화살표 그리기
	Position.Y(Position.Y() + m_sprite.GetRect(0).Height());
	DrawSprite(1, Position, mouse_pos == MOUSE_POSITION::DOWN_ARROW ? FRAME::ACTIVE : FRAME::NONE, false, 180.0f);
}


GUI_STATE::Type GuiScrollBar::Check()
{
	__super::Check();

	m_mouse_y = light::Input::Mouse::Pos().Y() - m_position.Y();
	if( m_gui_state == GUI_STATE::MOUSE_LBUTTONDOWN ||
		m_gui_state == GUI_STATE::MOUSE_LBUTTONUP )
	{
		OnClick();
	}
	else if(m_gui_state == GUI_STATE::MOUSE_OVER)
	{
	}
	else
	{
		m_mouse_y = -1;
	}

	return m_gui_state;
}

void GuiScrollBar::OnAcquireFocus()
{
	OnClick();
}

void GuiScrollBar::OnClick()
{
	if( m_mouse_y == -1 ||
		m_contents.Height() <= m_rect.Height() )
		return;

	if( m_mouse_y < m_arrow_height) 
	{
		m_bar_y_position = std::max(0, m_bar_y_position - 5);
	}
	else if( m_mouse_y < m_arrow_height + m_bar_bg_height )
	{
		m_bar_y_position = std::max(0, std::min(m_bar_bg_height - m_bar_height, m_mouse_y - m_arrow_height));
	}
	else
	{
		m_bar_y_position = std::min(m_bar_bg_height - m_bar_height, m_bar_y_position + 5);
	}

	SetDrawingRect();
}

light::RECT_INT GuiScrollBar::GetRect()
{
	return m_rect;
}


void GuiScrollBar::OnLogic()
{

}

void GuiScrollBar::MouseEvent()
{
	switch(m_gui_state)
	{
	case GUI_STATE::MOUSE_LBUTTONUP:
		Call();
		break;
	}

	__super::MouseEvent();
}

} // namespace view

} // namespace view
