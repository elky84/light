#include "stdafx.h"
#include "TestMode.h"

#include "DirectX.h"
#include "D3DUtility.h"
#include "Input.h"

#include <boost/bind.hpp>

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
, m_Camera(light::view::Camera::AIRCRAFT)
{
}

TestMode::~TestMode()
{
}

void TestMode::Begin()
{
	// Turn on the zbuffer
	m_direct_x->DxDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Culling
	m_direct_x->DxDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// No lighting
	m_direct_x->DxDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Do the alpha blending
	m_direct_x->DxDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_direct_x->DxDevice()->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	m_direct_x->DxDevice()->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	D3DXVECTOR3 vec(0.0f, 30.0f, -1200.0f);
	m_Camera.Position(vec);

	m_Model.reset(new light::view::Model(m_direct_x));
	m_Model->Load(L".\\Data\\tiny.x");
}

void TestMode::End()
{

}

void TestMode::Render()
{
	m_Model->SetCurrentAnimation(m_Model->GetCurrentAnimation()+1);

	m_Model->Update(0.02f);

	// draw Model
	{
		// world º¯È¯
		D3DXMATRIX T;
		D3DXMatrixIdentity(&T);
		D3DXMatrixRotationX(&T, 300.0f);

		m_Model->Draw(T);
	}
}

void TestMode::RenderPostUI()
{
	
}

void TestMode::Logic()
{
	
	if( light::Input::Keyboard::Pressing('W') )
		m_Camera.walk(10.0f);

	if( light::Input::Keyboard::Pressing('S') )
		m_Camera.walk(-10.0f);

	if( light::Input::Keyboard::Pressing('A') )
		m_Camera.strafe(-10.0f);

	if( light::Input::Keyboard::Pressing('D') )
		m_Camera.strafe(10.0f);

	if( light::Input::Keyboard::Pressing('R') )
		m_Camera.fly(1.0f);

	if( light::Input::Keyboard::Pressing('F') )
		m_Camera.fly(-1.0f);

	if( light::Input::Keyboard::Pressing(VK_UP) )
		m_Camera.pitch(-0.01f);

	if( light::Input::Keyboard::Pressing(VK_DOWN) )
		m_Camera.pitch(0.01f);

	if( light::Input::Keyboard::Pressing(VK_LEFT) )
		m_Camera.yaw(-0.01f);

	if( light::Input::Keyboard::Pressing(VK_RIGHT) )
		m_Camera.yaw(0.01f);

	if( light::Input::Keyboard::Pressing('N') )
		m_Camera.roll(0.1f);

	if( light::Input::Keyboard::Pressing('M') )
		m_Camera.roll(-0.1f);

	// Update the view matrix representing the cameras 
	// new position/orientation.
	D3DXMATRIX V;
	m_Camera.getViewMatrix(&V);
	m_direct_x->DxDevice()->SetTransform(D3DTS_VIEW, &V);
}