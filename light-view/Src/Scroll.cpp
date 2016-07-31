#include "light-view.h"
#include "Scroll.h"

namespace light
{

namespace view
{

Scroll::Scroll(const int Width, const int Height, const int Unit, const int Check)
	: m_max_x(0)
	, m_max_y(0)
	, m_width(Width)
	, m_height(Height)
	, m_unit(Unit)
	, m_check(Check)
	, m_x(0)
	, m_y(0)
{

}

Scroll::~Scroll()
{

}

void Scroll::SetLength(const int MaxX, const int MaxY)
{
	m_max_x = MaxX;
	m_max_y = MaxY;
}

bool Scroll::Process(const light::POSITION_INT& Position)
{
	const int X = m_x;
	const int Y = m_y;

	if(Position.X() < m_check)
	{
		m_x = std::max(0, m_x - m_unit);
	}
	else if(m_width - m_check < Position.X())
	{
		m_x = std::min(m_max_x - m_width, m_x + m_unit);
	}

	if(Position.Y() < m_check)
	{
		m_y = std::max(0, m_y - m_unit);
	}
	else if(m_height - m_check < Position.Y())
	{
		m_y = std::min(m_max_y - m_height, m_y + m_unit);
	}

	if( m_x == X &&	m_y == Y)
	{
		return false;
	}
	return true;
}

} // namespace view

} // namespace view
