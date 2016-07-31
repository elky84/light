#pragma once

#include "Gui.h"
#include "weak_raw_ptr.hpp"

#include <list>

namespace light
{

namespace view
{

class GuiManager
{
private:
	typedef std::list<Gui*> LIST_GUI;

public:
	GuiManager(light::weak_raw_ptr<DirectX> directX);
	
	~GuiManager();

	bool Set(Gui* pGui);
	
	bool Release(Gui* pGui);
	
	void Release();

	void OnDraw();

	void Check();

	void OnLogic();

	void OnTab();

	light::weak_raw_ptr<Gui> Crash(const light::POSITION_INT& Position, const Gui::PROPERTY::Type property);

	light::weak_raw_ptr<Gui> DragPtr();

private:
	LIST_GUI m_list_Gui;

	light::weak_raw_ptr<Gui> m_Focus;

	light::weak_raw_ptr<Gui> m_Drag;

	light::weak_raw_ptr<DirectX> m_direct_x;
};

} // namespace view

} // namespace view
