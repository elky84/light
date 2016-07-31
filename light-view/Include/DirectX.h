#pragma once

#include <D3D9.h>
#include <D3Dx9.h>

#include "FrameSkip.h"
#include "FreeType2.h"

#include "Mode.h"

namespace light
{

namespace view
{

class GuiManager;
class TextureManager;
class ModeManager;

const int DEV_DIRECTX_ID = D3DADAPTER_DEFAULT;

const float DEFAULT_FPS = 60.0f;

const int DEFAULT_SCREEN_WIDTH = 800;
const int DEFAULT_SCREEN_HEIGHT = 600;

struct DIRECT3D_CONFIGURATION
{
public:
	DIRECT3D_CONFIGURATION(int nWidth = DEFAULT_SCREEN_WIDTH, int nHeight = DEFAULT_SCREEN_HEIGHT, BOOL bWindow = TRUE)
		: m_window(bWindow)
		, m_width(nWidth)
		, m_height(nHeight)
	{

	}

public:
	int m_width;

	int m_height;

	BOOL m_window;
};

class DirectX
{
public:
	DirectX(const int DevID);
	virtual ~DirectX();

	BOOL Init(const std::wstring& strName, float fFps = DEFAULT_FPS, BOOL bWindow = TRUE);

	BOOL Init(HWND hwnd, const std::wstring& strName, const light::RECT_INT& Rect, float fFps = DEFAULT_FPS);

	BOOL Setting(const std::wstring& strName, const light::RECT_INT& Rect, float fFps = DEFAULT_FPS, BOOL bWindow = TRUE );

	BOOL InitD3D();

	BOOL InitParameters();

	BOOL InitDevice();

	BOOL CreateSprite2D();

	BOOL LoadFont(const std::wstring& strName);

	void on_update();

	LPDIRECT3DTEXTURE9 LoadTexture(const std::wstring& strName, DWORD dwColorKey = D3DCOLOR_XRGB(255, 0, 255), D3DXIMAGE_INFO* pImageInfo = NULL);
	
	LPDIRECT3DTEXTURE9 CreateTexture(int nWidth, int nHeight);

	void DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const int nSize, const DWORD dwColor = D3DCOLOR_RGBA(0,0,0,255));

	void DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int nSize, const DWORD dwColor = D3DCOLOR_RGBA(0,0,0,255), const FreeType2::Align::Type align = FreeType2::Align::CENTER);

	bool DrawSprite(LPDIRECT3DTEXTURE9 pTexture, RECT& SrcRect, float x, float y, BYTE alpha = 255);

	const int Width()
	{
		return m_direct3d_configuration.m_width;
	}

	const int Height()
	{
		return m_direct3d_configuration.m_height;
	}

	LPDIRECT3DDEVICE9 DxDevice()
	{
		return m_d3d_device;
	}

	light::weak_raw_ptr<TextureManager> TextureMgr();

	light::weak_raw_ptr<GuiManager> GuiMgr();

	void ResetCounter();

	void AsyncTextureLoad();

	bool InputCondition(const std::wstring& Message, const light::RECT_INT& Rect, const int Size);

public: // ModeManager의 Redirecting 함수
	bool RegisterMode(Mode* pMode);

	bool ActiveMode(const Mode::ModeId Id);

	bool ChangeMode(const Mode::ModeId Id);

	bool Reserve(const Mode::ModeId Id);

private: // WINDOWS 콜백
	bool SysCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool Destroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool Quit(HWND hWnd, WPARAM wParam, LPARAM lParam);

protected:
	virtual void Reset();

	virtual void Release();

private:
	void Render();

	void Restore();

	virtual void Setup() = 0;
	
	virtual void SetupParameter() =  0;

protected:
	/// D3D 디바이스 설정
	D3DPRESENT_PARAMETERS m_present_parameters;

	/// 윈도우 핸들
	HWND m_hWnd;

	/// Direct3D 설정
	DIRECT3D_CONFIGURATION m_direct3d_configuration;

	/// D3D 백 버퍼 포맷
	D3DFORMAT m_format;

private:
	std::unique_ptr<ModeManager> m_mode_manager;

	std::unique_ptr<GuiManager> m_gui_manager;

	std::unique_ptr<TextureManager> m_texture_manager;

	std::wstring m_name;

	D3DDISPLAYMODE m_display_mode;

	int m_DevId;

	/// 프레임당 렌더 수
	float m_fps;

	/// the pointer to our Direct3D interface
	LPDIRECT3D9 m_direct3d9;    
	
	/// the pointer to the device class
	LPDIRECT3DDEVICE9 m_d3d_device;    

	/// 폰트 입출력을 위한 FreeType2 변수
	FreeType2 m_free_type2;

	/// the pointer to our Direct3D Sprite2D interface
	LPD3DXSPRITE m_d3d_sprite;

	FrameSkip m_frame_skip;
};

class RenderState
{
public:
	RenderState(LPDIRECT3DDEVICE9 pDevice, const D3DRENDERSTATETYPE type)
		: m_device(pDevice)
		, m_type(type)
		, m_value(0)
	{
		m_device->GetRenderState(m_type, &m_value);
		if( m_value == TRUE )
		{
			m_device->SetRenderState(m_type, FALSE);
		}
	}

	~RenderState()
	{
		if( m_value == TRUE )
		{
			m_device->SetRenderState(m_type, TRUE);
		}
	}

private:
	const LPDIRECT3DDEVICE9 m_device;

	const D3DRENDERSTATETYPE m_type;

	DWORD m_value;
};

} //namespace view

} //namespace light
