#pragma once

#include "Gui.h"
#include "weak_raw_ptr.hpp"

namespace light
{

namespace view
{

class DirectX;

class GuiGroup
	: public Gui
{
private:
	typedef light::weak_raw_ptr<Gui> WEAK_GUI;

	typedef std::vector<WEAK_GUI> VECTOR_GUI;

public:
	GuiGroup();

	virtual ~GuiGroup();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, int Width, int Height);

	void Add(WEAK_GUI pGui);

	void Set(const light::POSITION_INT& Position);

	void Release();

private:
	virtual void OnLogic();

private:
	VECTOR_GUI m_guies;

	int m_width;

	int m_height;
};

} // namespace view

} // namespace view
