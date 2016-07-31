#include "stdafx.h"
#include "Character.h"

Character::Character(const int X, const int Y, light::view::Sprite2D& sprite)
	: light::view::Tile(sprite)
	, m_x(X)
	, m_y(Y)
{

}

Character::~Character()
{

}

void Character::Move(const int X, const int Y)
{
	m_x = X;
	m_y = Y;
}