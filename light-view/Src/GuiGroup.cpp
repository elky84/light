#include "light-view.h"
#include "GuiGroup.h"
#include "DirectX.h"

namespace light
{

namespace view
{

GuiGroup::GuiGroup()
{
}

GuiGroup::~GuiGroup()
{
}

bool GuiGroup::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, int Width, int Height)
{
	m_direct_x = directX;
	SetPosition(Position);
	m_width = Width;
	m_height = Height;
	return Load(file_name);
}

void GuiGroup::Add(WEAK_GUI pGui)
{
	m_guies.push_back(pGui);
}

void GuiGroup::Set(const light::POSITION_INT& Position)
{
	light::POSITION_INT Pos = Position;
	for each(WEAK_GUI pGui in m_guies)
	{
		pGui->SetPosition(Pos);
		pGui->Set();
		
		Pos.Y(Pos.Y() + pGui->GetRect().Height());
	}
}

void GuiGroup::OnLogic()
{

}

void GuiGroup::Release()
{
	for each(WEAK_GUI pGui in m_guies)
	{
		pGui->Release();
	}
}

} // namespace view

} // namespace view
