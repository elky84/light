#pragma once

#include "Mode.h"

#include "GuiProgressBar.h"

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
	float m_Progress;

	light::view::GuiProgressBar m_ProgressBar[light::view::Gui::Direction::MAX];
};
