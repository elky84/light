#pragma once

#include "Mode.h"

#include "Animation2D.h"

#include "Line2D.h"
#include "Rectangle2D.h"

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

public:
	bool OnSingleModeButtonClick();
	bool OnExitButtonClick();

private:
	light::view::GuiButton	m_Button[3];

	light::view::Animation2D m_Animation;
};
