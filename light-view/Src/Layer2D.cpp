#include "light-view.h"
#include "Layer2D.h"

namespace light
{

namespace view
{

Layer2D::Layer2D()
: m_now_frame(0)
, m_total_loop(0)
, m_now_loop(0)
{

}

Layer2D::~Layer2D()
{
	Release();
}

void Layer2D::Release()
{
	m_polygon.Release();
}

void Layer2D::reset()
{
	m_now_frame = 0;
	m_now_loop = 0;
	m_polygon.reset();
}

bool Layer2D::isComplete()
{
	return m_total_loop == m_now_loop && Maxframe() == m_now_frame;
}


void Layer2D::Update()
{
	if ( Maxframe() <= m_now_frame ) 
	{
		if ( m_total_loop > m_now_loop ) 
		{
			m_now_loop++;
			m_now_frame = 0;
		}
	}

	else
	{
		m_now_frame++;
	}
}


void Layer2D::Render(const light::POSITION_INT& position, const bool Mirror)
{
	Frame2D& frame = m_frames[m_now_frame];
	m_polygon.SetTransformColor(frame.Color());
	m_polygon.Blt(light::RECT_FLOAT(frame.Rect().Left() + position.X(), frame.Rect().Top() + position.Y(), frame.Rect().Right(), frame.Rect().Bottom()), frame.UV(), Mirror, frame.Rotate());
}

void Layer2D::Render(const light::POSITION_INT& position, const bool Mirror,  float fRot)
{
	Frame2D& frame = m_frames[m_now_frame];
	m_polygon.SetTransformColor(frame.Color());
	m_polygon.Blt(light::RECT_FLOAT(frame.Rect().Left() + position.X(), frame.Rect().Top() + position.Y(), frame.Rect().Right(), frame.Rect().Bottom()), frame.UV(), Mirror, fRot);
}

float Layer2D::Interpolate(float fMin, float fMax, float f, int Array)
{
	return fMin + (fMax - fMin) / Array * f;
}

void Layer2D::Interpolate(const Frame2D& frame, const Frame2D::NO no)
{
	int beforeframe = Maxframe();
	int ArrayNum = no - beforeframe;

	m_frames.resize(no + 1); //reason is frame counting started 0. so no + 1.
	m_frames[no] = frame;

	for( int i = 0;i < ArrayNum;i++ )
	{
		float j = (float)i;

		Frame2D& minframe = m_frames[beforeframe];
		Frame2D& maxframe = m_frames[i + beforeframe];

		minframe.UV(frame.UV());
		maxframe.UV(frame.UV());

		light::RECT_FLOAT rect;
		rect.Left(Interpolate(minframe.Rect().Left(), frame.Rect().Left(), j, ArrayNum));
		rect.Top(Interpolate(minframe.Rect().Top(), frame.Rect().Top(), j, ArrayNum));
		rect.Right(Interpolate(minframe.Rect().Right(), frame.Rect().Right(), j, ArrayNum));
		rect.Bottom(Interpolate(minframe.Rect().Bottom(), frame.Rect().Bottom(), j, ArrayNum));
		maxframe.OriginalRect(rect);
		maxframe.Rect(rect);

		COLORS_FLOAT color;
		color.R(Interpolate(minframe.Color().R(), frame.Color().R(), j, ArrayNum));
		color.G(Interpolate(minframe.Color().G(), frame.Color().G(), j, ArrayNum));
		color.B(Interpolate(minframe.Color().B(), frame.Color().B(), j, ArrayNum));
		color.A(Interpolate(minframe.Color().A(), frame.Color().A(), j, ArrayNum));

		float fRotate = Interpolate(minframe.Rotate(), frame.Rotate(), j, ArrayNum);
		maxframe.Rotate(fRotate);

		maxframe.Color(color);
	}
}

void Layer2D::LoadTexture(const std::wstring& strName)
{
	m_polygon.LoadTexture(strName);
}

void Layer2D::LoopCount(const int Count)
{
	m_total_loop = Count;
}

Frame2D::NO Layer2D::Maxframe()
{
	return m_frames.empty() ? 0 : (Frame2D::NO)(m_frames.size() - 1);
}

void Layer2D::SetDirectX(light::weak_raw_ptr<DirectX> directX)
{
	m_polygon.SetDirectX(directX);
}

} // namespace view

} // namespace view
