#pragma once

#include <vector>
#include "Position.h"
#include "Rectangle.h"

#include "weak_raw_ptr.hpp"

struct ID3DXLine;

namespace light
{

namespace view
{

class DirectX;

class Rectangle2D
{
public:
	Rectangle2D();

	~Rectangle2D();

	void Create(light::weak_raw_ptr<DirectX> directX, const DWORD dwColor, const float fWidth = 1);

	void SetPosition(const light::POSITION_INT& Src, const light::POSITION_INT& Dest);

	void Render();

	void Clear();

	light::RECT_INT& Rect();

	void Rect(const light::RECT_INT& rect);

private:
	DWORD m_color;

	ID3DXLine* m_line; 

	std::vector<D3DXVECTOR2> m_vectors;

	light::RECT_INT m_rect;

	/// DirectX
	light::weak_raw_ptr<DirectX> m_direct_x;
};

} // namespace view

} // namespace view
