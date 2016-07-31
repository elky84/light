#include "light-view.h"
#include "DirectX.h"

#include "ModeManager.h"
#include "GuiManager.h"
#include "TextureManager.h"

#include "WindowUtil.h"
#include "EnvironmentManager.h"

#include <boost/bind.hpp>

#pragma comment (lib, "D3D9.lib")
#pragma comment (lib, "D3Dx9.lib")

#pragma warning( disable : 4100 )

namespace light
{

namespace view
{

DirectX::DirectX(const int DevID)
	: m_fps(DEFAULT_FPS)
	, m_hWnd(NULL)
	, m_direct3d9(NULL)
	, m_d3d_sprite(NULL)
	, m_d3d_device(NULL)
	, m_DevId(DevID)
	, m_format(D3DFMT_X8R8G8B8)
	, m_frame_skip(m_fps)
	, m_free_type2(this, m_d3d_sprite)
	, m_mode_manager(new ModeManager(this))
	, m_texture_manager(new TextureManager(this))
	, m_gui_manager(new GuiManager(this))
{
	Mode::SetModeManager(m_mode_manager.get());

	ZeroMemory(&m_present_parameters,sizeof(D3DPRESENT_PARAMETERS));

	{
		light::WindowUtil::AddDispatcher(WM_SYSCOMMAND, boost::bind(&DirectX::SysCommand, this, _1, _2, _3));
		light::WindowUtil::AddDispatcher(WM_DESTROY, boost::bind(&DirectX::Destroy, this, _1, _2, _3));
		light::WindowUtil::AddDispatcher(WM_QUIT, boost::bind(&DirectX::Quit, this, _1, _2, _3));

		light::WindowUtil::SetupCallbacks();
	}
}

DirectX::~DirectX()
{
	Release();
	m_mode_manager.reset(); // ModeManager is must release, before GuiManager and TextureManager to release.

	if(m_texture_manager.get())
	{
		m_texture_manager->stop();
	}
}

void DirectX::Release()
{
	SAFE_RELEASE(m_d3d_sprite);
	SAFE_RELEASE(m_d3d_device);
	SAFE_RELEASE(m_direct3d9);
	SAFE_DESTROY_HWND(m_hWnd);
}

BOOL DirectX::Init(HWND hwnd, const std::wstring& strName, const light::RECT_INT& Rect, float fFps)
{
	m_hWnd = hwnd;
	return Setting(strName, Rect, fFps, TRUE);
}

BOOL DirectX::Init(const std::wstring& strName, float fFps, BOOL bWindow)
{
	m_hWnd = light::WindowUtil::MainHWND();
	light::RECT_INT rect(light::WindowUtil::MainWndRect());
	return Setting(strName, rect, fFps, bWindow);
}

BOOL DirectX::Setting(const std::wstring& strName, const light::RECT_INT& Rect, float fFps /* = DEFAULT_FPS */, BOOL bWindow /* = TRUE */ )
{
	m_name = strName;
	m_fps = fFps;
	m_direct3d_configuration.m_window = TRUE;
	m_direct3d_configuration.m_width = Rect.Right() - Rect.Left();
	m_direct3d_configuration.m_height = Rect.Bottom() - Rect.Top();

	if(FALSE == InitD3D())
		return FALSE;

	if(FALSE == InitDevice())
		return FALSE;

	AsyncTextureLoad();
	return TRUE;
}

BOOL DirectX::InitD3D()
{
	m_direct3d9 = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D object
	if( NULL == m_direct3d9 )
		return FALSE;

	return TRUE;
}

BOOL DirectX::CreateSprite2D()
{
	D3DXCreateSprite(m_d3d_device, &m_d3d_sprite);    // create the Direct3D Sprite2D object
	return m_d3d_sprite != NULL ? TRUE : FALSE;
}

BOOL DirectX::InitDevice()
{
	if( D3D_OK == m_direct3d9->GetAdapterDisplayMode(m_DevId, &m_display_mode) )
		m_format = m_display_mode.Format;

	ZeroMemory(&m_present_parameters, sizeof(m_present_parameters));
	SetupParameter();

	//Hardware rendering HAL(pure vp)
	HRESULT hr = S_OK;
	if(FAILED( hr = m_direct3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &m_present_parameters, &m_d3d_device) ) )
	{
		//Software & Hardware rendering HAL(soft vp)
		if(FAILED( hr = m_direct3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &m_present_parameters, &m_d3d_device) ) )
		{
			//Software rendering REF
			if(FAILED( hr = m_direct3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &m_present_parameters, &m_d3d_device) ) )
			{
				LOG_ERROR(L"%s Error [%u]", __FUNCTIONW__, hr);
				return FALSE;
			}
		}
	}

	if( FALSE == CreateSprite2D() )
		return FALSE;

	Setup();
	return true;
}

BOOL DirectX::LoadFont(const std::wstring& strName)
{
	return m_free_type2.LoadFont(strName);
}

LPDIRECT3DTEXTURE9 DirectX::CreateTexture(int nWidth, int nHeight)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	m_d3d_device->CreateTexture(nWidth, nHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, 0);
	return pTexture;
}

LPDIRECT3DTEXTURE9 DirectX::LoadTexture(const std::wstring& strName, DWORD dwColorKey /* = D3DCOLOR_XRGB */, D3DXIMAGE_INFO* pImageInfo /*= NULL*/)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXCreateTextureFromFileEx(m_d3d_device,    // the device pointer
		strName.c_str(),    // the new file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		dwColorKey,    // the hot-pink color key
		pImageInfo,    // no image info struct
		NULL,    // not using 256 colors
		&pTexture);    // load to sprite
	return pTexture;
}

void DirectX::on_update()
{
	MSG msg;
	if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
	{
		if ( false == light::WindowUtil::CallDispatcher(msg.hwnd, msg.message, msg.wParam, msg.lParam) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	m_frame_skip.Update();
	if(  m_frame_skip.Frame() )
	{
		m_mode_manager->OnLogic();
		Render();
	}

}

void DirectX::Render()
{
	HRESULT hr = m_d3d_device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST)
		return;
	else if(hr == D3DERR_DEVICENOTRESET)
		Restore();

	m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// begin
	m_d3d_device->BeginScene();

	m_mode_manager->OnRender();
	m_mode_manager->OnRenderPostUI();

	// ends the 3D scene
	m_d3d_device->EndScene();    

	hr = m_d3d_device->Present(NULL, NULL, NULL, NULL);
	if( D3DERR_DEVICENOTRESET == hr )
	{
		Restore();
	}
}

void DirectX::Reset()
{
	SAFE_RELEASE(m_d3d_sprite);
	SAFE_RELEASE(m_d3d_device);

	InitDevice();

	m_texture_manager->Reset();
}

void DirectX::Restore()
{
	if (FAILED(m_d3d_device->Reset(&m_present_parameters)))
	{
		Reset();
		return;
	}
}

void DirectX::DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const int nSize, const DWORD dwColor)
{
	m_free_type2.Render(str, Pos, nSize, dwColor);
}

void DirectX::DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int nSize, const DWORD dwColor /* = D3DCOLOR_RGBA */, const FreeType2::Align::Type align /*= FreeType2::Align::CENTER*/)
{
	m_free_type2.Render(str, Pos, Rect, nSize, dwColor, align);
}

bool DirectX::DrawSprite(LPDIRECT3DTEXTURE9 pTexture, RECT& SrcRect, float x, float y, BYTE alpha)
{
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f), position(x, y, 0.0f);
	HRESULT hr = m_d3d_sprite->Draw(pTexture, &SrcRect, &center, &position, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	if( S_OK != hr )
	{
		LOG_INFO(L"%s [%p] [%d, %d, %d, %d] [%d, %d, %d]", __FUNCTIONW__, pTexture, SrcRect.top, SrcRect.bottom, SrcRect.left, SrcRect.right, x, y, alpha);
		return false;
	}
	return true;
} 

light::weak_raw_ptr<TextureManager> DirectX::TextureMgr()
{
	return m_texture_manager;
}

light::weak_raw_ptr<GuiManager> DirectX::GuiMgr()
{
	return m_gui_manager;
}

void DirectX::ResetCounter()
{
	m_frame_skip.reset();
}

void DirectX::AsyncTextureLoad()
{
	m_texture_manager->AsyncLoad();
}

bool DirectX::InputCondition(const std::wstring& Message, const light::RECT_INT& Rect, const int Size)
{
	return m_free_type2.InputCondition(Message, Rect, Size);
}

bool DirectX::SysCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam & 0xFFF0) 
	{
	case SC_CLOSE:
		PostQuitMessage(0);
		return true;
	}
	return false;
}

bool DirectX::Destroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	light::EnvironmentManager::Instance()->stop();
	return true;
}

bool DirectX::Quit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	light::EnvironmentManager::Instance()->stop();
	return true;
}

bool DirectX::RegisterMode(Mode* pMode)
{
	return m_mode_manager->Register(pMode);
}

bool DirectX::ActiveMode(const Mode::ModeId Id)
{
	return m_mode_manager->Active(Id);
}

bool DirectX::ChangeMode(const Mode::ModeId Id)
{
	return m_mode_manager->Change(Id);
}

bool DirectX::Reserve(const Mode::ModeId Id)
{
	return m_mode_manager->Reserve(Id);
}

} // namespace view

} // namespace view
