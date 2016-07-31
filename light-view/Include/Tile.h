#pragma once

#include "Sprite2D.h"

namespace light
{

namespace view
{

class Tile
{
public:
	Tile(Sprite2D& sprite);
	~Tile();

	const light::RECT_INT GetRect()
	{
		return m_sprite.GetRect(0, 0);
	}

	int Width()
	{
		return m_sprite.GetRect(0, 0).Height();
	}

	int Height()
	{
		return m_sprite.GetRect(0, 0).Width();
	}

	void Draw(Layer2D::NO LayerNo, light::POSITION_INT Pos, unsigned int drawingframe);

private:

	Sprite2D& m_sprite;
};

} // namespace view

} // namespace view
