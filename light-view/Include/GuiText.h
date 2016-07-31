#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiText : public Gui
{
public:
	GuiText();

	virtual ~GuiText();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& strMessage, const light::RECT_INT& Rect, const light::POSITION_INT& Pos, int FontSize = 10, DWORD dwColor = D3DCOLOR_RGBA(0, 0, 0, 255), const FreeType2::Align::Type align = FreeType2::Align::CENTER);

private:
	virtual GUI_STATE::Type Check();

	virtual void OnDraw();

	virtual void OnLogic();

private:
	light::RECT_INT m_rect;
};

} // namespace view

} // namespace view
