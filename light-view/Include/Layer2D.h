#pragma once

#include "Frame2D.h"
#include "Polygon2D.h"

namespace light
{

namespace view
{

class Layer2D
{
public:
	typedef WORD NO;

public:
	Layer2D();

	~Layer2D();

	void Release();

	void reset();

	bool isComplete();

	void Update();

	void Render(const light::POSITION_INT& Position, const bool Mirror);

	void Render(const light::POSITION_INT& Position, const bool Mirror, const float fRot);

	void Interpolate(const Frame2D& frame, const Frame2D::NO no);

	void LoadTexture(const std::wstring& strName);

	void LoopCount(const int Count);

	Frame2D::NO Maxframe();

	void SetDirectX(light::weak_raw_ptr<DirectX> directX);

public:
	static float Interpolate(float fMin, float fMax, float f, int Array);

private:
	std::vector<Frame2D> m_frames;

	Polygon2D m_polygon;		

	size_t m_total_loop;	

	size_t m_now_loop;		

	size_t m_now_frame;	
};

} // namespace view

} // namespace view
