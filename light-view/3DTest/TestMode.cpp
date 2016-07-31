#include "stdafx.h"
#include "TestMode.h"

#include "DirectX.h"
#include "D3DUtility.h"
#include "Input.h"

#include <boost/bind.hpp>

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
, m_Model(directX)
, m_Floor(0)
, m_texture(0)
, m_Camera(light::view::Camera::AIRCRAFT)
{
}

TestMode::~TestMode()
{
	light::safe_release(m_Floor);
	light::safe_release(m_texture);
}

void TestMode::Begin()
{
	// they don't exist, create them
	m_direct_x->DxDevice()->CreateVertexBuffer( 6 * sizeof(light::view::Vertex3D), 0, light::view::Vertex3D::FVF, D3DPOOL_MANAGED, &m_Floor, 0);

	light::view::Vertex3D* v = 0;
	m_Floor->Lock(0, 0, (void**)&v, 0);

	v[0] = light::view::Vertex3D(-50.0f, -2.5f, -50.0f, 0.0f, 50.0f, 2.5f, 0.0f, 1.0f);
	v[1] = light::view::Vertex3D(-50.0f, -2.5f,  50.0f, 0.0f, 50.0f, 2.5f, 0.0f, 0.0f);
	v[2] = light::view::Vertex3D( 50.0f, -2.5f,  50.0f, 0.0f, 50.0f, 2.5f, 1.0f, 0.0f);

	v[3] = light::view::Vertex3D(-50.0f, -2.5f, -50.0f, 0.0f, 50.0f, 2.5f, 0.0f, 1.0f);
	v[4] = light::view::Vertex3D( 50.0f, -2.5f,  50.0f, 0.0f, 50.0f, 2.5f, 1.0f, 0.0f);
	v[5] = light::view::Vertex3D( 50.0f, -2.5f, -50.0f, 0.0f, 50.0f, 2.5f, 1.0f, 1.0f);

	m_Floor->Unlock();

	D3DXCreateTextureFromFile(m_direct_x->DxDevice(), L".\\Data\\desert.bmp",&m_texture);

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

	D3DXVECTOR3 vec(0.0f, 30.0f, -300.0f);
	m_Camera.Position(vec);

	m_Model.Load(L".\\Data\\character.x");
}

void TestMode::End()
{
	
}

void TestMode::Render()
{
	D3DXMATRIX T, R, S;
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	// used to rotate cylinders to be parallel with world's y-axis
	D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);
	
	D3DXMatrixIdentity(&T);
	T = T * S;

	// draw floor
	{
		m_direct_x->DxDevice()->SetTransform(D3DTS_WORLD, &T);
		m_direct_x->DxDevice()->SetMaterial(&light::view::D3D::WHITE_MTRL);
		m_direct_x->DxDevice()->SetTexture(0, m_texture);
		m_direct_x->DxDevice()->SetStreamSource(0, m_Floor, 0, sizeof(light::view::Vertex3D));
		m_direct_x->DxDevice()->SetFVF(light::view::Vertex3D::FVF);
		m_direct_x->DxDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}

	// draw x-file
	{
		D3DXMatrixTranslation(&T, 0, 50, 0);
		m_Model.Draw(T);
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