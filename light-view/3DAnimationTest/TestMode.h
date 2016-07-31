#pragma once

#include "Mode.h"

#include "Camera.h"
#include "Model.h"
#include <memory>

#include "GuiButton.h"
#include "Animation2D.h"

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
	light::view::Camera m_Camera;

	std::unique_ptr<light::view::Model> m_Model;
};
