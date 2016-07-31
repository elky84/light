#include "light-view.h"
#include "GuiText.h"

namespace light
{

namespace view
{

GuiText::GuiText()
{
}

GuiText::~GuiText()
{
}

bool GuiText::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& strMessage, const light::RECT_INT& Rect, const light::POSITION_INT& Pos, int FontSize, DWORD dwColor, const FreeType2::Align::Type align)
{
	m_direct_x = directX;
	SetPosition(Pos);
	m_rect = Rect;
	m_message = strMessage;
	m_font_size = FontSize;
	m_message_color = dwColor;
	m_align = align;
	return true;
}

void GuiText::OnDraw()
{
	DrawString(GetPosition(), m_rect, m_align);
}

void GuiText::OnLogic()
{
	
}

GUI_STATE::Type GuiText::Check()
{
	__super::Check();
	m_frame = m_gui_state != GUI_STATE::NONE ? 1 : 0;
	return m_gui_state;
}


} // namespace view

} // namespace view
