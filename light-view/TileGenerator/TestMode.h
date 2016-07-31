#pragma once

#include "Mode.h"

#include <vector>

#include "Animation2D.h"
#include "Sprite2D.h"
#include "GuiButton.h"

#include "Scroll.h"

namespace light
{

namespace view
{

	class TilePainter;
	class DirectX;

} // namespace view

} // namespace view

class TestMode 
	: public light::view::Mode
	, public light::view::Scroll
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
	light::view::GuiButton m_Buttons[3];

	std::unique_ptr<light::view::TilePainter> m_TilePainter;

	light::POSITION_INT m_position;
};