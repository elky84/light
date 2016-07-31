#include "light-view.h"
#include "Line2D.h"

#include "DirectX.h"
#include "Input.h"

namespace light
{

namespace view
{

Line2D::Line2D()
	: m_color(D3DCOLOR_XRGB(0, 0, 0))
	, m_line(NULL)
{
	
}

Line2D::~Line2D()
{

}

void Line2D::Create(light::weak_raw_ptr<DirectX> directX, const DWORD dwColor, const float fWidth)
{
	m_direct_x = directX;

	D3DXCreateLine(m_direct_x->DxDevice(), &m_line); 
	m_line->SetWidth(fWidth);

	m_color = dwColor;
}

void Line2D::SetPosition(const light::POSITION_INT& Src, const light::POSITION_INT& Dest)
{
	m_vectors.resize(2);
	m_vectors[0] = D3DXVECTOR2((float)Src.X(), (float)Src.Y());
	m_vectors[1] = D3DXVECTOR2((float)Dest.X(), (float)Dest.Y());
}

void Line2D::Clear()
{
	m_vectors.clear();
}

void Line2D::Render()
{
	if(m_vectors.empty())
		return;

	m_line->Begin();

	m_line->Draw(m_vectors.data(), m_vectors.size(), m_color);

	m_line->End();
}

} // namespace view

} // namespace view
