#pragma once

#include <vector>
#include "Position.h"
#include "weak_raw_ptr.hpp"

struct ID3DXLine;

namespace light
{

namespace view
{

class DirectX;

class Line2D
{
public:
	Line2D();

	~Line2D();

	void Create(light::weak_raw_ptr<DirectX> directX, const DWORD dwColor, const float fWidth = 1);

	void SetPosition(const light::POSITION_INT& Src, const light::POSITION_INT& Dest);

	void Clear();

	void Render();

private:
	DWORD m_color;

	ID3DXLine* m_line; 

	std::vector<D3DXVECTOR2> m_vectors;

	/// DirectX
	light::weak_raw_ptr<DirectX> m_direct_x;
};

} // namespace view

} // namespace view
