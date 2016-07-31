#include "light-view.h"
#include "Direct3D.h"
#include "D3DUtility.h"

namespace light
{

namespace view
{

Direct3D::Direct3D(const int DevID)
	: DirectX(DevID)
{

}

Direct3D::~Direct3D()
{

}

void Direct3D::Setup()
{
	static const float fProjectionDepth = 5000.0f;

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj, D3DX_PI * 0.25f /*45 - degree*/, (float)m_direct3d_configuration.m_width / (float)m_direct3d_configuration.m_height, 1.0f, fProjectionDepth);

	DxDevice()->SetTransform(D3DTS_PROJECTION, &proj);

	//Do the alpha blending
	DxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	DxDevice()->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	DxDevice()->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
}

void Direct3D::SetupParameter()
{
	m_present_parameters.hDeviceWindow = m_hWnd;
	m_present_parameters.Windowed = m_direct3d_configuration.m_window;
	m_present_parameters.BackBufferWidth = m_direct3d_configuration.m_width;
	m_present_parameters.BackBufferHeight = m_direct3d_configuration.m_height;

	m_present_parameters.Flags = 0;
	m_present_parameters.BackBufferFormat = m_format;
	m_present_parameters.BackBufferCount = 1;
	m_present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_present_parameters.MultiSampleQuality = 0;
	m_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	m_present_parameters.EnableAutoDepthStencil = TRUE; 
	m_present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_present_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

} // namespace view

} // namespace view
