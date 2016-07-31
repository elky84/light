#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiTooltip 
	: public Gui
{
public:
	GuiTooltip();

	virtual ~GuiTooltip();

	bool Create(light::weak_raw_ptr<DirectX> directX, TCHAR* szFileName, float Rate = 1.0f);

	void Set(const std::wstring& strMessage, int FontSize = 10, DWORD dwColor = D3DCOLOR_RGBA(0, 0, 0, 255));

	void Show(const std::wstring& strMessage, const light::POSITION_INT& Position, int nStrSize = 10, DWORD dwTooltipTick = 5000, DWORD dwColor = D3DCOLOR_RGBA(0, 0, 0, 255));

	void SetPosition(const light::POSITION_INT& Position);

protected:
	virtual void Show(const light::POSITION_INT& Position);

	virtual void Hide();

	virtual void OnDraw();

	virtual void OnLogic();

protected:
	DWORD m_tick;

	DWORD m_tooltip_tick;

	light::RECT_INT m_resize_rect;

	light::POSITION_INT m_adjust_text_position;
};

} // namespace view

} // namespace view
