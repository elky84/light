#include "light-view.h"
#include "GuiEditBox.h"

#include "ImeInput.h"
#include <boost/bind.hpp>

#include "DirectX.h"
#include "GuiManager.h"

namespace light
{

namespace view
{

GuiEditBox::GuiEditBox()
	: m_tick(GetTickCount())
{
}

GuiEditBox::~GuiEditBox()
{
}

bool GuiEditBox::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Pos, const int FontSize, const DWORD Color, boost::function<void()> func, const TCHAR* szEnterSound /* = NULL */)
{
	m_direct_x = directX;
	SetPosition(Pos);
	m_font_size = FontSize;
	m_message_color = Color;
	m_align = FreeType2::Align::LEFT;
	m_function = func;

	if(szEnterSound)
	{
		m_enter_sound.Load(szEnterSound);
	}

	Property(PROPERTY::EDITBABLE);
	if( false ==  Load(file_name) )
	{
		return false;
	}

	return true;
}

void GuiEditBox::Push(Gui& gui)
{
	EraseFlash();
	gui.Message(m_message, m_font_size, m_message_color, m_align);
	m_message.clear();
}

void GuiEditBox::OnLogic()
{
	
}

static const std::wstring Flash = L"|";

bool GuiEditBox::InputCondition()
{
	return m_direct_x->InputCondition(m_message, GetRect(), m_font_size);
}

bool GuiEditBox::EraseFlash()
{
	std::wstring::size_type at = m_message.find_last_of(Flash);
	if(std::wstring::npos != at)
	{
		m_message.erase(at);
		return true;
	}
	return false;
}

void GuiEditBox::OnFocus()
{
	if( GetTickCount() - m_tick < 500 )
		return;

	m_tick = GetTickCount();

	if(EraseFlash() == false)
	{
		m_message += Flash;
	}
}

void GuiEditBox::OnTab()
{
	m_direct_x->GuiMgr()->OnTab();
}

void GuiEditBox::OnAcquireFocus()
{
	ImeInput::Instance()->set(m_message);
	ImeInput::Instance()->CarrageReturnCallback(boost::bind(&GuiEditBox::CarrageReturn, this));
	ImeInput::Instance()->TabCallback(boost::bind(&GuiEditBox::OnTab, this));
	ImeInput::Instance()->InputCondition(boost::bind(&GuiEditBox::InputCondition, this));
	m_frame = 1;

	m_enter_sound.Play();
}

void GuiEditBox::CarrageReturn()
{
	if(m_function)
	{
		m_function();
		// ImeInput::Instance()->Clear(); 개발자가 선택하도록 지정.
	}

	if(m_append_gui.get() == NULL)
		return;

	EraseFlash();
	m_append_gui->Message(m_message, m_font_size, m_message_color, m_align);
}

void GuiEditBox::OnLostFocus()
{
	ImeInput::Instance()->unregister_callback();
	m_frame = 0;

	EraseFlash();
}

} // namespace view

} // namespace view
