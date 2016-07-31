#include "light-view.h"
#include "Rectangle2D.h"

#include "DirectX.h"
#include "Input.h"

namespace light
{

namespace view
{

Rectangle2D::Rectangle2D()
	: m_color(D3DCOLOR_XRGB(0, 0, 0))
	, m_line(NULL)
{
}

Rectangle2D::~Rectangle2D()
{

}

void Rectangle2D::Create(light::weak_raw_ptr<DirectX> directX, const DWORD dwColor, const float fWidth)
{
	m_direct_x = directX;

	D3DXCreateLine(m_direct_x->DxDevice(), &m_line); 
	m_line->SetWidth(fWidth);

	m_color = dwColor;
}

void Rectangle2D::SetPosition(const light::POSITION_INT& Src, const light::POSITION_INT& Dest)
{
	m_vectors.resize(5);
	m_rect = light::RECT_INT(std::min(Src.X(), Dest.X()), std::min(Src.Y(), Dest.Y()), std::max(Src.X(), Dest.X()), std::max(Src.Y(), Dest.Y()));

	m_vectors[0] = D3DXVECTOR2((float)Src.X(), (float)Src.Y());
	m_vectors[1] = D3DXVECTOR2((float)Dest.X(), (float)Src.Y());
	m_vectors[2] = D3DXVECTOR2((float)Dest.X(), (float)Dest.Y());
	m_vectors[3] = D3DXVECTOR2((float)Src.X(), (float)Dest.Y());
	m_vectors[4] = D3DXVECTOR2((float)Src.X(), (float)Src.Y());
}

void Rectangle2D::Clear()
{
	m_vectors.clear();
}

void Rectangle2D::Render()
{
	if(m_vectors.empty())
		return;

	m_line->Begin();

	m_line->Draw(m_vectors.data(), m_vectors.size(), m_color);

	m_line->End();
}

light::RECT_INT& Rectangle2D::Rect()
{
	return m_rect;
}

void Rectangle2D::Rect(const light::RECT_INT& rect)
{
	m_rect = rect;

	light::POSITION_INT Src(std::min(m_rect.Left(), m_rect.Right()), std::min(m_rect.Top(), m_rect.Bottom()) );
	light::POSITION_INT Dest(std::max(m_rect.Left(), m_rect.Right()), std::max(m_rect.Top(), m_rect.Bottom()));

	m_vectors.resize(5);
	m_vectors[0] = D3DXVECTOR2((float)Src.X(), (float)Src.Y());
	m_vectors[1] = D3DXVECTOR2((float)Dest.X(), (float)Src.Y());
	m_vectors[2] = D3DXVECTOR2((float)Dest.X(), (float)Dest.Y());
	m_vectors[3] = D3DXVECTOR2((float)Src.X(), (float)Dest.Y());
	m_vectors[4] = D3DXVECTOR2((float)Src.X(), (float)Src.Y());
}

} // namespace view

} // namespace view
