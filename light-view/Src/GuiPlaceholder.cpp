#include "light-view.h"
#include "GuiPlaceholder.h"

namespace light
{

namespace view
{

GuiPlaceholder::GuiPlaceholder()
{
}

GuiPlaceholder::~GuiPlaceholder()
{
}

bool GuiPlaceholder::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, Gui::Callback callback /* = NULL */)
{
	m_direct_x = directX;
	SetPosition(Position);
	SetCallback(callback);
	Property(PROPERTY::PLACEHOLDER);
	return Load(file_name);
}

bool GuiPlaceholder::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTexture, const light::POSITION_INT& Position, Gui::Callback callback /* = NULL */)
{
	m_direct_x = directX;
	SetPosition(Position);
	SetCallback(callback);
	Property(PROPERTY::PLACEHOLDER);
	return Load(file_name, strTexture);
}

void GuiPlaceholder::OnLogic()
{

}

void GuiPlaceholder::Set()
{
	__super::Set();
	if(m_append_gui.get())
	{
		m_append_gui->Set();
	}
}

void GuiPlaceholder::Release()
{
	__super::Release();
	if(m_append_gui.get())
	{
		m_append_gui->Release();
	}
}

bool GuiPlaceholder::Append(light::weak_raw_ptr<Gui> pGui, const light::POSITION_INT& Adjust /* = light::POSITION_INT */)
{
	__super::Append(pGui, Adjust);
	return Call();
}

} // namespace view

} // namespace view
