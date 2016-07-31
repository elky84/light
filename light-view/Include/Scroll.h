#pragma once

#include "Position.h"

namespace light
{

namespace view
{

class Scroll
{
public:
	Scroll(const int Width, const int Height, const int Unit, const int Check);
	virtual ~Scroll();

	bool Process(const light::POSITION_INT& Position);

	void SetLength(const int MaxX, const int MaxY);

protected:
	int m_x, m_y;

	int m_max_x;
	int m_max_y;

	const int m_width;
	const int m_height;

	const int m_unit;
	const int m_check;
};

} // namespace view

} // namespace view
