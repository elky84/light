#pragma once

#include "Tile.h"

class Character
	: public light::view::Tile
{
public:
	Character(const int X, const int Y, light::view::Sprite2D& sprite);

	virtual ~Character();

	void Move(const int X, const int Y);

	int X() const
	{
		return m_x;
	}

	int Y() const
	{
		return m_y;
	}

private:
	int m_x, m_y;
};