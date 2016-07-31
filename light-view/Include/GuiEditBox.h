#pragma once

#include "Gui.h"
#include "FmodEx.h"

namespace light
{

namespace view
{

class GuiEditBox : public Gui
{
public:
	GuiEditBox();

	virtual ~GuiEditBox();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Pos, const int FontSize, const DWORD Color, boost::function<void()> func = NULL, const TCHAR* szEnterSound = NULL);

	void Push(Gui& gui);

private:
	virtual void OnLogic();

	virtual void OnFocus();

	virtual void OnAcquireFocus();

	virtual void OnTab();

	virtual void OnLostFocus();

private:
	void CarrageReturn();

	bool EraseFlash();

	bool InputCondition();

private:
	FmodEx m_enter_sound;

	DWORD m_tick;

	boost::function<void()> m_function;
};

} // namespace view

} // namespace view
