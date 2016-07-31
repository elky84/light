#include "light-view.h"
#include "GuiImage.h"

namespace light
{

namespace view
{

GuiImage::GuiImage()
{
}

GuiImage::~GuiImage()
{
}

bool GuiImage::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Pos)
{
	m_direct_x = directX;
	m_position = Pos;
	return Load(file_name);
}

bool GuiImage::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTextureName, const light::POSITION_INT& Pos)
{
	m_direct_x = directX;
	m_position = Pos;
	return Load(file_name, strTextureName);
}

void GuiImage::OnLogic()
{
}

GUI_STATE::Type GuiImage::Check()
{
	__super::Check();
	m_frame = m_gui_state != GUI_STATE::NONE ? 1 : 0;
	return m_gui_state;
}


} // namespace view

} // namespace view
