#include "light-view.h"
#include "Frame2D.h"

namespace light
{

namespace view
{

Frame2D::Frame2D()
{
}

Frame2D::Frame2D(const COLORS_FLOAT& color, const light::RECT_FLOAT& uv, const light::RECT_FLOAT& rect, const float rotate)
	: m_color(color)
	, m_uv(uv)
	, m_rect(rect)
	, m_original_rect(rect)
	, m_rotate(rotate)
{
}

Frame2D::~Frame2D()
{
}

float Frame2D::Rotate() const
{
	return m_rotate;
}

void Frame2D::Rotate(const float rotate)
{
	m_rotate = rotate;
}

COLORS_FLOAT Frame2D::Color() const
{
	return m_color;
}

void Frame2D::Color(const COLORS_FLOAT& color)
{
	m_color = color;
}

light::RECT_FLOAT Frame2D::UV() const
{
	return m_uv;
}

void Frame2D::UV(const light::RECT_FLOAT& rect)
{
	m_uv = rect;
}

light::RECT_FLOAT Frame2D::Rect() const
{
	return m_rect;
}

void Frame2D::Rect(const light::RECT_FLOAT& rect)
{
	m_rect = rect;
}

light::RECT_FLOAT Frame2D::OriginalRect() const
{
	return m_original_rect;
}

void Frame2D::OriginalRect(const light::RECT_FLOAT& rect)
{
	m_original_rect = rect;
}

} // namespace view

} // namespace view
