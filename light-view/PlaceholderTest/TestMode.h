#pragma once

#include "Mode.h"

#include "GuiPlaceholder.h"
#include "GuiButton.h"

namespace light
{

namespace view
{
	class DirectX;
} // namespace view

} // namespace view

class TestMode : public light::view::Mode
{
public:
	TestMode(light::weak_raw_ptr<light::view::DirectX> directX);

	virtual ~TestMode();

	virtual void Begin();

	virtual void End();

	virtual void Render();

	virtual void RenderPostUI();

	virtual void Logic();

private:
	bool Test(const std::wstring& str);

private:
	light::view::GuiPlaceholder m_Placeholder[3];

	light::view::GuiButton m_Button[3];
};
