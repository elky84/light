#pragma once

#include "Mode.h"

#include "Camera.h"
#include "GuiButton.h"
#include "Model.h"
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
	IDirect3DVertexBuffer9* m_Floor;
	IDirect3DTexture9*      m_texture;

	light::view::Model m_Model;

	light::view::Camera m_Camera;
};
