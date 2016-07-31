#include "stdafx.h"
#include "Character.h"

#include "TilePainter.h"

Character::Character(light::weak_raw_ptr<light::view::DirectX> directX, INT Unit)
	: m_direct_x(directX)
	, m_direction(Direction::DOWN)
	, m_unit(Unit)
	, m_Move(0)
{

}

Character::~Character()
{
}

void Character::Load()
{
	m_Animation.Load(m_direct_x, L".\\Character\\Character.ani");
	m_Animation.Stop();
}

void Character::Draw(light::POSITION_INT Pos)
{
	m_Animation.Position(Pos);
	m_Animation.Draw(m_direction);
}

void Character::Logic(light::weak_raw_ptr<light::view::TilePainter> pTilePainter)
{
    UNREFERENCED_PARAMETER(pTilePainter);

	if( m_Move <= 0 )
	{
		return;
	}

	--m_Move;

	if( m_Animation.NowPlaying() )
	{
		m_Animation.Update();
		switch( m_direction )
		{
		case Direction::LEFT:
			m_position.X(std::max(m_position.X() - m_unit, 0));
			break;
		case Direction::RIGHT:
			m_position.X(std::min(m_position.X() + m_unit, MAX_SCROLL_X - TILE_SIZE));
			break;
		case Direction::UP:
			m_position.Y(std::max(m_position.Y() - m_unit, 0));
			break;
		case Direction::DOWN:
			m_position.Y(std::min(m_position.Y() + m_unit, MAX_SCROLL_Y - TILE_SIZE));
			break;
		}
	}
}

void Character::SetDirection(Direction::Type direction)
{
	INT X_Surplus = m_position.X() % TILE_SIZE;
	INT Y_Surplus = m_position.Y() % TILE_SIZE;
	switch( m_direction )
	{
	case Direction::LEFT:
	case Direction::RIGHT:
		m_Move = TILE_SIZE / m_unit - X_Surplus / m_unit;
		break;
	case Direction::UP:
	case Direction::DOWN:
		m_Move = TILE_SIZE / m_unit - Y_Surplus / m_unit;;
		break;
	}

	m_Animation.Play();
	if( m_direction != direction || m_Animation.NowPlaying() == false)
	{
		m_Animation.reset();
	}

	m_direction = direction;
}