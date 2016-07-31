#include "light-view.h"
#include "GuiTooltip.h"

#include "DirectX.h"

namespace light
{

namespace view
{

GuiTooltip::GuiTooltip()
: m_tooltip_tick(0)
, m_tick(0)
{

}

GuiTooltip::~GuiTooltip()
{

}

bool GuiTooltip::Create(light::weak_raw_ptr<DirectX> directX, TCHAR* szFileName, float Rate /*= 1.0f*/)
{
	m_direct_x = directX;
	if( false == Load(szFileName))
		return false;

	m_resize_rect = GetResizeRect(Rate);
	light::RECT_INT rectGap = GetRect() - m_resize_rect;
	m_adjust_text_position.X(rectGap.Width() / 2);
	m_adjust_text_position.Y(rectGap.Height() / 2);
	return true;
}

void GuiTooltip::OnDraw()
{
	DrawSprite(0, GetPosition(), 0); 
	DrawString(light::POSITION_INT(GetPosition().X() - m_adjust_text_position.X(), GetPosition().Y() - m_adjust_text_position.Y()), m_resize_rect, m_align);
} 

void GuiTooltip::OnLogic()
{
	if(m_tooltip_tick < GetTickCount() - m_tick)
	{
		Hide();
	}
}

void GuiTooltip::Set(const std::wstring& strMessage, int FontSize /* = 10 */, DWORD dwColor /* = D3DCOLOR_RGBA */)
{
	m_message = strMessage;
	m_font_size = FontSize;
	m_message_color = dwColor;
}

void GuiTooltip::Show(const light::POSITION_INT& Position)
{
	m_tooltip_tick = 0xFFFFFFFF;

	SetPosition(Position);
	Gui::Set();
}

void GuiTooltip::SetPosition(const light::POSITION_INT& Position)
{
	INT X = Position.X();
	if( m_direct_x->Width() < Position.X() + m_resize_rect.Width())
		X = Position.X() - m_resize_rect.Width();

	INT Y = Position.Y();
	if( m_direct_x->Height() < Position.Y() + m_resize_rect.Height())
		Y = Position.Y() - m_resize_rect.Height();

	__super::SetPosition(light::POSITION_INT(X, Y));
}

void GuiTooltip::Show(const std::wstring& strMessage, const light::POSITION_INT& Position, int nStrSize /* = 10 */, DWORD dwTooltipTick /* = 5000 */, DWORD dwColor /* = D3DCOLOR_RGBA */)
{
	m_tooltip_tick = dwTooltipTick;
	m_tick = GetTickCount();

	SetPosition(Position);

	Message(strMessage, nStrSize, dwColor);
}

void GuiTooltip::Hide()
{
	Release();
	Call();
}

} // namespace view

} // namespace view
