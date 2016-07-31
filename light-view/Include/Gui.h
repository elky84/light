#pragma once

#include <D3D9.h>

#include "Position.h"
#include "Rectangle.h"
#include "Sprite2D.h"

#include "weak_raw_ptr.hpp"

#include <boost/function.hpp>

#include "FreeType2.h"

namespace light
{

namespace view
{

class DirectX;

struct GUI_STATE // mouse event is must devide process to begin-end.
{
	enum Type
	{
		NONE = 0,
		MOUSE_OVER,
		MOUSE_OVER_FIRST,
		MOUSE_LBUTTONDOWN,
		MOUSE_LBUTTONUP,	
		MOUSE_RBUTTONDOWN,
		MOUSE_RBUTTONUP,	
		MOUSE_WHEELBUTTONDOWN,
		MOUSE_WHEELBUTTONUP,
		MOUSE_DRAG,
		MOUSE_DROP,
	};
};

class Gui
{
public:
	template <typename T, typename TArray>
	static T* Find(light::weak_raw_ptr<view::Gui> pGui, TArray& array)
	{
		if(_countof(array) <= 0)
			return NULL;

		size_t size = sizeof(array[0]);

		size_t n = (size_t)((char*)(pGui.get()) - (char*)(&array));
		size_t at = n / (size);
		if( _countof(array) < at)
			return NULL;

		return &array[at];
	}

	template <typename T>
	static T* Find(light::weak_raw_ptr<view::Gui> pGui, std::vector<T>& vector)
	{
		if( vector.size() <= 0)
			return NULL;

		size_t size = sizeof(vector[0]);

		size_t n = (size_t)((char*)(pGui.get()) - (char*)(&vector[0]));
		size_t at = n / (size);

		if( vector.size() < at )
			return NULL;

		return &vector[at];
	}

	template <typename T, typename TArray>
	static size_t At(light::weak_raw_ptr<view::Gui> pGui, TArray& array)
	{
		size_t size = sizeof(array[0]);

		size_t n = (size_t)((char*)(pGui.get()) - (char*)(&array));
		size_t at = n / (size);
		if( _countof(array) < at)
			return INFINITE;

		return at;
	}

	template <typename T>
	static size_t At(light::weak_raw_ptr<view::Gui> pGui, std::vector<T>& vector)
	{
		size_t size = sizeof(vector[0]);

		size_t n = (size_t)((char*)(pGui.get()) - (char*)(&vector[0]));
		size_t at = n / (size);

		if( vector.size() < at )
			return INFINITE;

		return at;
	}


public:
	struct Direction
	{
		enum Type
		{
			NONE = -1,
			UP = 0,
			DOWN = 1,
			LEFT = 2,
			RIGHT = 3,
			MAX,
		};
	};

	struct PROPERTY
	{
		enum Type
		{
			NONE = 0x0000,
			MOVABLE = 0x0001,
			MOVABLE_PLACEHOLDER = 0x0002,
			PLACEHOLDER = 0x0004,
			SHOW_ON_MOUSEOVER = 0x0008,
			EDITBABLE = 0x0010,
		};
	};

protected:
	static const BYTE DEFAULT_PRIORITY = 255;

public:
	typedef boost::function<bool()> Callback;

public:
	Gui();

	virtual ~Gui();

	virtual void Set();

	virtual void Release();

	virtual void TopMost();

	bool isShow()
	{
		return m_show;
	}

public:
	void DrawString(const light::POSITION_INT& Position);

	void DrawString(const light::POSITION_INT& Position, const light::RECT_INT& Rect, const FreeType2::Align::Type align = FreeType2::Align::CENTER);

	void DrawString(const std::wstring& strMessage, const light::POSITION_INT& Position, const light::RECT_INT& Rect, const int FontSize, const DWORD Color, const FreeType2::Align::Type align);

	void DrawString(const std::wstring& strMessage, const light::POSITION_INT& Position, const int FontSize = 10, const DWORD Color = D3DCOLOR_RGBA(0, 0, 0, 255));

	std::wstring GetString();

	void SetCallback(Callback callback);

public:
	virtual light::RECT_INT GetRect(Layer2D::NO LayerNo, Frame2D::NO no = 0);

	virtual light::RECT_INT GetRect();

	light::POSITION_INT GetPosition();

	void SetPosition(const light::POSITION_INT& Position);

public:
	virtual bool Append(light::weak_raw_ptr<Gui> pGui, const light::POSITION_INT& Adjust = light::POSITION_INT(0, 0));

	light::weak_raw_ptr<Gui> AppendPtr();

public:
	virtual void Message(const std::wstring& message, int FontSize = 10, DWORD Color = D3DCOLOR_RGBA(0, 0, 0, 255), const FreeType2::Align::Type align = FreeType2::Align::CENTER);

	void MessageAdjust(const light::POSITION_INT& Adjust = light::POSITION_INT(0, 0));

protected:
	bool Load(const std::wstring& strName);

	bool Load(const std::wstring& strName, const std::wstring& strTextureName);

	int NowFrame() const;

	bool Call();

	light::RECT_INT GetResizeRect(float fRate);

	void DrawSprite(Layer2D::NO LayerNo, const light::POSITION_INT& Position, int frame, const bool Mirror = false, float fRot = 0.0f, float fAlpha = ALPHA_USE_ORIGINAL);

	Frame2D& GetFrame(Layer2D::NO LayerNo, Frame2D::NO FrameNo = 0);

protected:
	virtual void AppenderProcess();

public:
	virtual void OnLogic() = 0;

public:
	virtual GUI_STATE::Type Check();

	virtual void OnDraw();

	virtual void OnAcquireFocus() {}

	virtual void OnLostFocus() {}

	virtual void OnFocus() {}

public:
	virtual void Show(const light::POSITION_INT& Position)
	{
		UNREFERENCED_PARAMETER(Position);
		Set();
	}

	virtual void Hide()
	{
		Release();
	}

	void Property(PROPERTY::Type property)
	{
		m_property = property;
	}

	PROPERTY::Type Property() const
	{
		return m_property;
	}

	void Link(light::weak_raw_ptr<Gui> pGui);

protected:
	virtual void MouseEvent();

	void ToCenter(light::POSITION_INT& Position);

protected: // 텍스트 관련
	FreeType2::Align::Type m_align;

	std::wstring m_message;

	int m_font_size;

	DWORD m_message_color;

	light::POSITION_INT m_text_adjust;

protected:
	bool m_show;

	int m_frame;

	GUI_STATE::Type m_gui_state;

	light::weak_raw_ptr<Gui> m_append_gui;

	light::POSITION_INT m_adjust;

	light::weak_raw_ptr<DirectX> m_direct_x;

	light::POSITION_INT m_position;

	light::POSITION_INT m_l_butto_click_position;

	light::POSITION_INT m_mouse_position;

	Sprite2D m_sprite;

	Callback m_callback;

	PROPERTY::Type m_property;
};

} // namespace view

} // namespace view
