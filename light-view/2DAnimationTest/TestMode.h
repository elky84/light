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

class DirectX;

} // namespace view

} // namespace view

class Character;
typedef std::unique_ptr<Character> UNIQUE_CHARACTER;
typedef light::weak_raw_ptr<Character> WEAK_CHARACTER;

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
	UNIQUE_CHARACTER m_Character;

	light::POSITION_INT m_position;
};