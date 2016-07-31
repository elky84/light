#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiSlider : public Gui
{
public:
	GuiSlider();

	virtual ~GuiSlider();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& strFile, const light::POSITION_INT& Pos, Gui::Callback callback = NULL);

private:
	virtual void OnLogic();

	virtual void OnFocus();

	virtual void OnAcquireFocus();

	virtual void OnLostFocus();

	virtual GUI_STATE::Type Check();

	virtual void OnDraw();

	virtual void MouseEvent();

private:
	void OnClick();

public:
	INT GetValue() const
	{
		return m_value;
	}

private:
	light::POSITION_INT m_bar_position;

	INT m_value;
};

} // namespace view

} // namespace view
