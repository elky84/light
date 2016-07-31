#include "light-view.h"
#include "Direct2D.h"

namespace light
{

namespace view
{
	

Direct2D::Direct2D(const int DevID)
	: DirectX(DevID)
{

}

Direct2D::~Direct2D()
{

}

void Direct2D::Setup()
{
	// Turn off culling
	DxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Turn off D3D lighting
	DxDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Turn on the zbuffer
	DxDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );

	// use for alpha-test value = TextureAlpha * ShadeAlpha
	DxDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	DxDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	DxDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

	// for transparency, apply alpha-value. (if alpha-value is 0, transparency.)
	DxDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	DxDevice()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL );
	DxDevice()->SetRenderState( D3DRS_ALPHAREF, 0 );

	// set vertex format. only 1 vertex type.
	DxDevice()->SetFVF(view::Vertex2D::FVF);
}

void Direct2D::SetupParameter()
{
	m_present_parameters.hDeviceWindow = m_hWnd;
	m_present_parameters.Windowed = m_direct3d_configuration.m_window;
	m_present_parameters.BackBufferWidth = m_direct3d_configuration.m_width;
	m_present_parameters.BackBufferHeight = m_direct3d_configuration.m_height;

	m_present_parameters.Flags = 0;
	m_present_parameters.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_present_parameters.BackBufferFormat = m_format;
	m_present_parameters.EnableAutoDepthStencil = TRUE;
	m_present_parameters.AutoDepthStencilFormat = D3DFMT_D16;// depth is 16BIT buffer.
}

} // namespace view

} // namespace view
