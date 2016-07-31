#pragma once

#include "Rectangle.h"
#include "weak_raw_ptr.hpp"

namespace light
{

namespace view
{

class Texture;
class DirectX;

/**
@brief Create and management for texture under polygon.
*/
class Polygon2D
{
public:
	/// 持失切.
	Polygon2D();

	/// 持失切.
	Polygon2D(light::weak_raw_ptr<DirectX> directX);

	/// 差紫 持失切.  
	Polygon2D(const Polygon2D& polygon);

	/// 社瑚切.
	~Polygon2D();

    bool is_init()
    {
        return m_direct_x.get() != NULL;
    }

	void SetDirectX(light::weak_raw_ptr<DirectX> directX);

	const Polygon2D& operator = (const Polygon2D& plolygon);

	bool LoadTexture(const std::wstring& strName, DWORD dwColorKey = D3DCOLOR_XRGB(255, 0, 255));

	void DrawTexture(bool bBlur = false);

	void ReleaseTexture();

	void Release();
	
    void Blt(const light::RECT_FLOAT& Rect, const light::RECT_FLOAT& Uv, const bool Mirror = false, float fRot = 0.0f);

	void SetTransformColor(const COLORS_FLOAT& color);

	LONG GetWidth()
	{
		return m_width;
	}

	LONG GetHeight()
	{
		return m_height;
	}

	void reset();

private:
	/// Vertex Position 
	Vertex2D m_vertex_data[4];	

	/// Width of origin polygon
	int	m_width;

	/// Height of origin polygon
	int	m_height;		

	/// Texture
	light::weak_raw_ptr<Texture> m_texture;

	/// DirectX
	light::weak_raw_ptr<DirectX> m_direct_x;
};

} // namespace view

} // namespace view
