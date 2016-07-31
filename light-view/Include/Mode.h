#pragma once

#include "weak_raw_ptr.hpp"

#include "Position.h"
#include "Rectangle.h"
#include "FreeType2.h"

#include <boost/function.hpp>

namespace light
{

namespace view
{

class DirectX;
class Animation2D;
class ModeManager;

/**
@brief 게임의 현재 Mode 단위로 처리할 수 있도록 도와주는 Base 클래스.
*/
class Mode
{	
public:
	typedef boost::function<bool ()> Callback_t;

	typedef int ModeId;

	static const ModeId INVALID = -1;

protected:
	Mode(const ModeId Id, light::weak_raw_ptr<DirectX> directX);

public:
	virtual ~Mode();

	bool ChangeMode(const ModeId Id);

	void RequestBegin();

	void RequestEnd(Mode* NextMode);

	void PostRender();

	const ModeId Id() const
	{
		return m_Id;
	}

	const bool isAnimating() const;

	void DrawString(const std::wstring& str, const light::POSITION_INT& Pos, int nSize, DWORD dwColor = D3DCOLOR_RGBA(0,0,0,255));
	
	void DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int nSize, const DWORD dwColor = D3DCOLOR_RGBA(0,0,0,255), const FreeType2::Align::Type align = FreeType2::Align::CENTER);

	void LButtonClickCallback(Callback_t callback);

	void LButtonClickCallback();

private:
	bool BeginAnimation();

	bool EndAnimation();

	bool Draw(Animation2D* animation);

public: // static method.
	static void SetModeManager(ModeManager* modeManager);

	static void SetBeginAnimation(Animation2D* animation);

	static void SetEndAnimation(Animation2D* animation);

public:
	virtual void Render() = 0;

	virtual void RenderPostUI() = 0;

	virtual void Logic() = 0;

private:
	virtual void Begin() = 0;

	virtual void End() = 0;

protected:
	light::weak_raw_ptr<DirectX> m_direct_x;

private:
	ModeId m_Id;

	bool m_Animating;

	light::weak_raw_ptr<Mode> m_next_mode;

	Callback_t m_animation_callback;

	Callback_t m_l_button_click_callback;

private:
	static light::weak_raw_ptr<Animation2D> m_begin_animation;

	static light::weak_raw_ptr<Animation2D> m_end_animation;

	static light::weak_raw_ptr<ModeManager> m_mode_manager;
};

} // namespace view

} // namespace view
