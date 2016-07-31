#include "light-view.h"
#include "GuiSlider.h"

#include "Input.h"

namespace light
{

namespace view
{

GuiSlider::GuiSlider()
{
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& strFile, const light::POSITION_INT& Pos, Gui::Callback callback)
{
	m_direct_x = directX;
	SetPosition(Pos);
	SetCallback(callback);
	m_bar_position = Pos;
	return Load(strFile);
}

void GuiSlider::OnLogic()
{
	
}

void GuiSlider::OnFocus()
{
}

void GuiSlider::OnAcquireFocus()
{
	OnClick();
}

void GuiSlider::OnClick()
{
	INT Before = m_value;

	INT ClickPoint = (INT)((float)(light::Input::Mouse::Pos().X() - m_position.X()));
	m_bar_position.X(m_position.X() + ClickPoint); // 이렇게 함으로써, 바의 위치를 조정한다.

	m_value = (INT)((float)ClickPoint / GetRect().Width() * 100.0f);

	if(m_value != Before)
	{
		Call();
	}
}

void GuiSlider::OnLostFocus()
{
}

GUI_STATE::Type GuiSlider::Check()
{
	__super::Check();

	if(m_gui_state == GUI_STATE::MOUSE_LBUTTONDOWN)
	{
		OnClick();
	}

	m_frame = m_gui_state != GUI_STATE::NONE ? 1 : 0;
	return m_gui_state;
}

void GuiSlider::MouseEvent()
{
	switch(m_gui_state)
	{
	case GUI_STATE::MOUSE_LBUTTONUP:
		Call();
		break;
	}

	__super::MouseEvent();
}

void GuiSlider::OnDraw()
{
	__super::OnDraw();
	DrawSprite(1, m_bar_position, 0);
}

} // namespace view

} // namespace view
