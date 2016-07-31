#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiProgressBar : public Gui
{
public:
	GuiProgressBar();

	virtual ~GuiProgressBar();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, light::POSITION_INT Pos, float Max, size_t Size, const Direction::Type direction = Direction::Type::RIGHT);

	void SetProgress(Layer2D::NO LayerNo, float fProgress);

protected:
	virtual void OnDraw();

	virtual void OnLogic();

private:
	float m_Max;

	std::vector<float> m_progresses;

	Direction::Type m_direction;

	light::POSITION_INT m_adjust;
};

} // namespace view

} // namespace view
