#pragma once

#include "Mode.h"

#include "Line2D.h"
#include "Rectangle2D.h"

namespace light
{

namespace view
{
	class DirectX;
} // namespace view

} // namespace view

class TestMode 
	: public light::view::Mode
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
	light::view::Line2D m_line;

	light::view::Rectangle2D m_rect;
};
