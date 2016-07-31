#include "light-view.h"
#include "GuiListBox.h"

#include "GuiScrollBar.h"
#include <boost/range/adaptor/reversed.hpp>

namespace light
{

namespace view
{

GuiListBox::GuiListBox()
	: m_direction(Direction::NONE)
	, m_max_count(0)
	, m_cutted_height(0)
    , m_scroll_bar(new GuiScrollBar())
{
}

GuiListBox::~GuiListBox()
{
}

bool GuiListBox::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strScrollBarFile, light::POSITION_INT Pos, const Direction::Type direction, INT MaxCount /* = MAX_COUNT */)
{
	m_direct_x = directX;
	SetPosition(Pos);
	m_direction = direction;
	m_max_count = MaxCount;

	if( false == Load(file_name) )
	{
		return false;
	}

	m_contents = GetRect();
	m_drawing = GetRect();
	m_scroll_bar->Append(this);
	if( false  == m_scroll_bar->Create( directX, strScrollBarFile, light::POSITION_INT(m_position.X() + GetRect().Width(), m_position.Y() ), m_drawing, direction) )
	{
		return false;
	}

	if( GetRect().Height() != m_scroll_bar->GetRect().Height() )
	{
		LOG_ERROR(L"%s. different ScrollBar[%d] and Listbox[%d] height.", __FUNCTIONW__, GetRect().Height(), m_scroll_bar->GetRect().Height());
		return false;
	}
	return true;
}

void GuiListBox::Set()
{
	m_scroll_bar->Set();
	__super::Set();
}

void GuiListBox::Release()
{
	m_scroll_bar->Release();
	__super::Release();
}

void GuiListBox::OnLogic()
{
	
}

void GuiListBox::OnFocus()
{
}

void GuiListBox::OnAcquireFocus()
{
}

void GuiListBox::OnLostFocus()
{
}

static const int TEXT_INTERVAL = 2;

void GuiListBox::OnDraw()
{
	light::POSITION_INT Position = GetPosition();
	DrawSprite(0, Position, NowFrame());

	if(m_direction == Direction::DOWN)
	{
		INT Y = 0;
		if(m_drawing.Bottom() == m_contents.Bottom())
		{
			Y += m_cutted_height;
		}

		for each(auto& line in m_lines)
		{
			Y = Y + line.m_font_size + TEXT_INTERVAL;

			if( Y <= m_drawing.Top() )
				continue;

			if( m_drawing.Bottom() < Y)
				break;

			DrawString(line.m_message, Position, GetRect(), line.m_font_size, line.m_color, line.m_align);
			Position.Y(Position.Y() + line.m_font_size + TEXT_INTERVAL);
		}
	}
	else if(m_direction == Direction::UP)
	{
		Position.Y(Position.Y() + m_drawing.Height());

		INT Y = m_contents.Height();
		if(m_drawing.Top() == m_contents.Top()) // 사실은 0과 0비교이나...코드 일관성을 위해
		{
			Y += m_cutted_height;
			Position.Y(Position.Y() + m_cutted_height);
		}

		for each(auto& line in boost::adaptors::reverse(m_lines))
		{
			Y = Y - line.m_font_size - TEXT_INTERVAL;
			if( m_drawing.Bottom() <= Y)
				continue;

			if( Y < m_drawing.Top() )
				break;

			Position.Y(Position.Y() - line.m_font_size - TEXT_INTERVAL);
			DrawString(line.m_message, Position, GetRect(), line.m_font_size, line.m_color, line.m_align);
		}
	}
	else
	{
		LOG_ERROR(L"Invalid Direction. [%d]", m_direction);
		ASSERT_MSG(0, "Invalid Direction");
	}
}

void GuiListBox::Message(const std::wstring& message, int FontSize, DWORD Color, const FreeType2::Align::Type align)
{
	if(m_direction == Direction::DOWN)
	{
		m_lines.push_front(LINE(message, Color, FontSize, align));
		INT Y = 0;
		for each(auto& line in m_lines)
		{
			Y = Y + line.m_font_size + TEXT_INTERVAL;

			if( GetRect().Height() < Y ) // 화면에서 짤리는 값을 알아내기 위함.
			{
				m_cutted_height = Y - GetRect().Height();
				break;
			}
		}
	}
	else
	{
		m_lines.push_back(LINE(message, Color, FontSize, align));

		INT Y = 0;
		for each(auto& line in boost::adaptors::reverse(m_lines))
		{
			Y = Y + line.m_font_size + TEXT_INTERVAL;
			if( GetRect().Height() < Y ) // 화면에서 짤리는 값을 알아내기 위함.
			{
				m_cutted_height = Y - GetRect().Height();
				break;
			}
		}
	}

	m_contents.Bottom(0); // init
	for each(auto& line in m_lines)
	{
		m_contents.Bottom(m_contents.Bottom() + line.m_font_size + TEXT_INTERVAL);
	}

	m_scroll_bar->SetContent(m_contents);
}

} // namespace view

} // namespace view
