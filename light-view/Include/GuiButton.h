#pragma once

#include "Gui.h"
#include "FmodEx.h"

namespace light
{

namespace view
{

class GuiButton 
	: public Gui
{
public:
	GuiButton();

	virtual ~GuiButton();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, Gui::Callback callback, const light::POSITION_INT& Pos,  const TCHAR* szClickSound = NULL,  const TCHAR* szOverSound = NULL);
	
	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTextureName, Gui::Callback callback, const light::POSITION_INT& Pos,  const TCHAR* szClickSound = NULL,  const TCHAR* szOverSound = NULL);

	void Setting(light::weak_raw_ptr<DirectX> directX, Gui::Callback callback, const light::POSITION_INT& Pos,  const TCHAR * szClickSound = NULL,  const TCHAR * szOverSound = NULL);

	void RButtonCallback(Gui::Callback callback);

private:
	virtual GUI_STATE::Type Check();

	virtual void OnLogic();

	virtual void MouseEvent();

private:
	FmodEx m_click_sound;

	FmodEx m_over_sound;

	Gui::Callback m_r_button_callback;
};

} // namespace view

} // namespace view
