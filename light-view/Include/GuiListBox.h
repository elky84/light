#pragma once

#include "Gui.h"
#include <list>

namespace light
{

namespace view
{

class GuiScrollBar;

class GuiListBox : public Gui
{
private:
	struct LINE
	{
		std::wstring m_message;
		DWORD m_color;
		int m_font_size;
		FreeType2::Align::Type m_align;
		LINE(const std::wstring& message, DWORD Color, int FontSize, FreeType2::Align::Type align)
			: m_message(message)
			, m_color(Color)
			, m_font_size(FontSize)
			, m_align(align)
		{

		}
	};

public:
	static const INT MAX_COUNT = 30;

public:
	GuiListBox();

	virtual ~GuiListBox();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strScrollBarFile, light::POSITION_INT Pos, const Direction::Type direction, INT MaxCount = MAX_COUNT);

	virtual void Message(const std::wstring& message, int FontSize = 10, DWORD Color = D3DCOLOR_RGBA(0, 0, 0, 255), const FreeType2::Align::Type align = FreeType2::Align::CENTER);

	virtual void Set();

	virtual void Release();

private:
	virtual void OnLogic();

	virtual void OnFocus();

	virtual void OnAcquireFocus();

	virtual void OnLostFocus();

	virtual void OnDraw();
	
private:
	light::RECT_INT m_contents;

	light::RECT_INT m_drawing;

	std::list< LINE > m_lines;

	Direction::Type m_direction;

	std::unique_ptr<GuiScrollBar> m_scroll_bar;

	INT m_max_count;

	INT m_cutted_height;
};

} // namespace view

} // namespace view
