#include "light-view.h"
#include "Tile.h"

namespace light
{

namespace view
{

Tile::Tile(Sprite2D& sprite)
	: m_sprite(sprite)
{
}

Tile::~Tile()
{
}

void Tile::Draw(Layer2D::NO LayerNo, light::POSITION_INT Pos, unsigned int drawingframe)
{
	m_sprite.DrawSprite(LayerNo, Pos, drawingframe);
}

} // namespace view

} // namespace view
