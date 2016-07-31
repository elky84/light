#include "light-view.h"
#include "GuiButton.h"

namespace light
{

namespace view
{

GuiButton::GuiButton()
{
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, Gui::Callback callback, const light::POSITION_INT& Pos, const TCHAR* szClickSound /* = NULL */, const TCHAR* szOverSound /* = NULL */)
{
	Setting(directX, callback, Pos, szClickSound, szOverSound);
	return Load(file_name);
}

bool GuiButton::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTextureName, Gui::Callback callback, const light::POSITION_INT& Pos, const TCHAR* szClickSound /* = NULL */, const TCHAR* szOverSound /* = NULL */)
{
	Setting(directX, callback, Pos, szClickSound, szOverSound);
	return Load(file_name, strTextureName);
}

void GuiButton::Setting(light::weak_raw_ptr<DirectX> directX, Gui::Callback callback, const light::POSITION_INT& Pos, const TCHAR * szClickSound /* = NULL */, const TCHAR * szOverSound /* = NULL */)
{
	m_direct_x = directX;
	SetCallback(callback);
	SetPosition(Pos);

	if(szOverSound)
	{
		m_over_sound.Load(szOverSound);
	}

	if(szClickSound)
	{
		m_click_sound.Load(szClickSound);
	}
}

void GuiButton::RButtonCallback(Gui::Callback callback)
{
	m_r_button_callback = callback;
}

void GuiButton::OnLogic()
{
}

void GuiButton::MouseEvent()
{
	switch(m_gui_state)
	{
	case GUI_STATE::MOUSE_OVER_FIRST:
		m_over_sound.Play();
		break;
	case GUI_STATE::MOUSE_LBUTTONUP:
		m_click_sound.Play();
		Call();
		break;
	case GUI_STATE::MOUSE_RBUTTONUP:
		m_click_sound.Play();
		if(m_r_button_callback)
		{
			m_r_button_callback();
		}
		break;
	}
	__super::MouseEvent();
}

GUI_STATE::Type GuiButton::Check()
{
	__super::Check();
	m_frame = m_gui_state != GUI_STATE::NONE ? 1 : 0;
	return m_gui_state;
}

} // namespace view

} // namespace view
