#pragma once

#include "light.h"

#include <D3D9.h>
#include <D3Dx9.h>

namespace light
{

namespace view
{

const DWORD	DEFAULT_WINDOW_STYLE = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME;

static WCHAR* DEFAULT_DIRECTX_CLASSNAME = L"light-view";

struct Vertex2D
{	
	float x, y, z;	/// 위치 정보
	float rhw;		/// 정점 변환 값
	D3DCOLOR color;		/// Polygon2D(다각형) 칼라
	float tu, tv;		/// texture 좌표

	Vertex2D()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, color(0)
		, tu(0.0f)
		, tv(0.0f)
	{

	}

	Vertex2D(const Vertex2D& vertex)
		: x(vertex.x)
		, y(vertex.y)
		, z(vertex.z)
		, rhw(vertex.rhw)
		, color(vertex.color)
		, tu(vertex.tu)
		, tv(vertex.tv)
	{
	}

	const Vertex2D& operator= (const Vertex2D& vertex)
	{
		x = vertex.x;
		y = vertex.y;
		z = vertex.z;
		rhw = vertex.rhw;
		color = vertex.color;
		tu = vertex.tu;
		tv = vertex.tv;
		return *this;
	}

	static const DWORD FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
};

struct Vertex3D
{
	Vertex3D()
	{
	}

	Vertex3D(float x, float y, float z, 
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}

	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
};

template <typename T>
class Colors
{
public:
	Colors()
		: Red(255)
		, Green(255)
		, Blue(255)
		, Alpha(255)
	{

	}

	Colors(const T& red, const T& green, const T& blue, const T& alpha)
		: Red(red)
		, Green(green)
		, Blue(blue)
		, Alpha(alpha)
	{
	}

	Colors(const Colors<T>& color)
		: Red(color.R())
		, Green(color.G())
		, Blue(color.B())
		, Alpha(color.A())
	{
	}

	const T R() const
	{
		return Red;
	}

	void R(const T& red)
	{
		Red = red;
	}

	const T G() const
	{
		return Green;
	}

	void G(const T& green)
	{
		Green = green;
	}

	const T B() const
	{
		return Blue;
	}

	void B(const T& blue)
	{
		Blue = blue;
	}

	const T A() const
	{
		return Alpha;
	}

	void A(const T& alpha)
	{
		Alpha = alpha;
	}

	DWORD RGBA() const
	{
		return D3DCOLOR_RGBA((BYTE)Red, (BYTE)Green, (BYTE)Blue, (BYTE)Alpha);
	}

	void reset()
	{
		Red = 255;
		Green = 255;
		Blue = 255;
		Alpha = 255;
	}

private:
	T Red;
	T Green;
	T Blue;
	T Alpha;
};

typedef Colors<float> COLORS_FLOAT;
typedef Colors<BYTE> COLORS_BYTE;

static const float ALPHA_USE_ORIGINAL = -1.0f;

} // namespace view

} // namespace view