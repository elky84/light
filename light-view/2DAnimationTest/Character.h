#pragma once

#include <vector>
#include "Animation2D.h"
#include "weak_raw_ptr.hpp"

namespace light
{

namespace view
{

class TilePainter;

} // namespace view

} // namespace view

struct Direction
{
	enum Type
	{
		NONE = -1,
		DOWN = 0,
		LEFT,
		RIGHT,
		UP,
	};
};

class Character
{
public:
	Character(light::weak_raw_ptr<light::view::DirectX> directX, INT Unit);
	~Character();

	void SetPosition(const light::POSITION_INT& position)
	{
		m_position = position;
	}

	light::POSITION_INT& GetPosition()
	{
		return m_position;
	}

	INT X() const
	{
		return m_position.X();
	}

	INT Y() const
	{
		return m_position.Y();
	}

	void Render();

	void Draw(light::POSITION_INT Pos);

	void SetDirection(Direction::Type direction);

	void Logic();

	void Load();

private:
	INT m_unit;

	DWORD m_Move;

	light::POSITION_INT m_position;

	light::view::Animation2D m_Animation;

	light::weak_raw_ptr<light::view::DirectX> m_direct_x;

	Direction::Type m_direction;
};