#include "light-view.h"
#include "Mode.h"

#include "ModeManager.h"
#include "Animation2D.h"

#include "Input.h"
#include "DirectX.h"

namespace light
{

namespace view
{

light::weak_raw_ptr<Animation2D> Mode::m_begin_animation;

light::weak_raw_ptr<Animation2D> Mode::m_end_animation;

light::weak_raw_ptr<ModeManager> Mode::m_mode_manager;

Mode::Mode(const ModeId Id, light::weak_raw_ptr<DirectX> directX)
	: m_Id(Id)
	, m_direct_x(directX)
{
}

Mode::~Mode()
{
}

bool Mode::ChangeMode(const ModeId Id)
{
	return m_mode_manager->Change(Id);
}

void Mode::RequestBegin()
{
	if( m_begin_animation.get() != NULL )
	{
		m_Animating = true;
		m_animation_callback = std::tr1::bind(&Mode::BeginAnimation, this);
	}

	Begin();
}

void Mode::RequestEnd(Mode* NextMode)
{
	m_next_mode = NextMode;
	if( m_end_animation.get() != NULL )
	{
		m_animation_callback = std::tr1::bind(&Mode:: EndAnimation, this);
	}
	else
	{
		End();
		m_mode_manager->Active(m_next_mode->Id());
	}
}

const bool Mode::isAnimating() const
{
	return m_animation_callback ? true : false;
}

void Mode::LButtonClickCallback(Callback_t callback)
{
	m_l_button_click_callback = callback;
}

void Mode::LButtonClickCallback()
{
	if(light::Input::Mouse::LButtonClick())
	{
		if(m_l_button_click_callback)
		{
			m_l_button_click_callback();
			light::Input::Mouse::LButtonClick(false);
		}
	}
}

void Mode::DrawString(const std::wstring& str, const light::POSITION_INT& Pos, int nSize, DWORD dwColor)
{
	m_direct_x->DrawString(str, Pos, nSize, dwColor);
}

void Mode::DrawString(const std::wstring& str, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int nSize, const DWORD dwColor /* = D3DCOLOR_RGBA */, const FreeType2::Align::Type align /*= FreeType2::Align::CENTER*/)
{
	m_direct_x->DrawString(str, Pos, Rect, nSize, dwColor, align);
}

bool Mode::BeginAnimation()
{
	if( false == Draw(m_begin_animation.get()))
		return false;

	m_animation_callback = NULL;
	return true;
}
bool Mode::EndAnimation()
{
	if( false == Draw(m_end_animation.get()) )
		return false;

	m_animation_callback = NULL;
	End();
	m_mode_manager->Active(m_next_mode->Id());
	return true;
}

void Mode::SetModeManager(ModeManager* pModeManager)
{
	m_mode_manager = pModeManager;
}

void Mode::SetBeginAnimation(Animation2D* animation)
{
	m_begin_animation = animation;
}

void Mode::SetEndAnimation(Animation2D* animation)
{
	m_end_animation = animation;
}

bool Mode::Draw(Animation2D* animation)
{
	animation->Update();
	animation->Draw();

	if( true == animation->NowPlaying() )
	{
		return false;
	}

	animation->reset();
	return true;
}

void Mode::PostRender()
{
	if( m_animation_callback != NULL )
	{
		m_animation_callback();
	}
}

} // namespace view

} // namespace view
