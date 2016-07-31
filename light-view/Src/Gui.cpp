#include "light-view.h"

#include "Gui.h"
#include "GuiManager.h"

#include "Input.h"
#include "DirectX.h"

namespace light
{

namespace view
{

Gui::Gui()
: m_gui_state(GUI_STATE::NONE)
, m_font_size(0)
, m_message_color(D3DCOLOR_RGBA(0, 0, 0, 255))
, m_frame(0)
, m_align(FreeType2::Align::CENTER)
, m_property(PROPERTY::NONE)
, m_show(false)
{
}

Gui::~Gui()
{
}

void Gui::Set()
{
	if(m_direct_x.get() == NULL)
	{
		LOG_ERROR(L"%s m_direct_x.get() == NULL", __FUNCTIONW__);
		return;
	}

	if( true == m_show )
		return;

	m_direct_x->GuiMgr()->Set(this);
	m_show = true;
}

void Gui::Release()
{
	if(m_direct_x.get() == NULL)
	{
		LOG_ERROR(L"%s m_direct_x.get() == NULL", __FUNCTIONW__);
		return;
	}

	if( false == m_show )
		return;

	m_direct_x->GuiMgr()->Release(this);

	if(m_append_gui.get())
	{
		m_direct_x->GuiMgr()->Release(m_append_gui.get());
	}

	m_show = false;
}

void Gui::TopMost()
{
	Release();
	Set();

	if(m_append_gui.get())
	{
		m_append_gui->Release();
		m_append_gui->Set();
	}

	m_show = true;
}

bool Gui::Load(const std::wstring& strName)
{
	return m_sprite.Load(m_direct_x, strName);
}

bool Gui::Load(const std::wstring& strName, const std::wstring& strTextureName)
{
	return m_sprite.Load(m_direct_x, strName, strTextureName);
}

bool Gui::Call()
{
	if( m_callback )
	{
		return m_callback();
	}

	// 콜백이 없으면 걍 성공처리
	return true;
}

GUI_STATE::Type Gui::Check()
{
	if( false == m_sprite.isLoad() )
	{
		return GUI_STATE::NONE;
	}

	m_mouse_position = light::Input::Mouse::Pos();
	light::RECT_INT Rect = GetRect();
	light::RECT_INT rt(m_position.X(), m_position.Y(), m_position.X() + Rect.Width(), m_position.Y() + Rect.Height());
	if ( m_direct_x->GuiMgr()->DragPtr().get() == NULL &&
		true == crash_check( rt, m_mouse_position ) )
	{
		HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
		SetCursor(hCursor);

		GUI_STATE::Type PrevState = m_gui_state, CurrentState = GUI_STATE::NONE;
		if ( true == light::Input::Mouse::LButtonClick() )
		{
			CurrentState = GUI_STATE::MOUSE_LBUTTONDOWN;
			m_l_butto_click_position = light::Input::Mouse::Pos();
		}
		else if( true == light::Input::Mouse::RButtonClick() )
		{
			CurrentState = GUI_STATE::MOUSE_RBUTTONDOWN;
		}
		else if( 0 != light::Input::Mouse::Wheel() )
		{
			CurrentState = GUI_STATE::MOUSE_WHEELBUTTONDOWN;
		}
		else
		{
			CurrentState = GUI_STATE::MOUSE_OVER;
		}

		switch(PrevState)
		{
		case GUI_STATE::MOUSE_LBUTTONDOWN:
			{
				if( light::Input::Mouse::LButtonClick() == false )
				{
					m_gui_state = GUI_STATE::MOUSE_LBUTTONUP;
				}
			}
			break;
		case GUI_STATE::MOUSE_RBUTTONDOWN:
			{
				if( light::Input::Mouse::RButtonClick() == false )
				{
					m_gui_state = GUI_STATE::MOUSE_RBUTTONUP;
				}
			}
			break;
		case GUI_STATE::MOUSE_WHEELBUTTONDOWN:
			{
				if( light::Input::Mouse::Wheel() == false )
				{
					m_gui_state = GUI_STATE::MOUSE_WHEELBUTTONUP;
				}
			}
			break;
		case GUI_STATE::NONE:
			m_gui_state = GUI_STATE::MOUSE_OVER_FIRST;
			break;
		default:
			m_gui_state = CurrentState;
			break;
		}
	}
	else
	{
		if( m_property & PROPERTY::MOVABLE || 
			m_property & PROPERTY::MOVABLE_PLACEHOLDER )
		{
			if( true == light::Input::Mouse::LButtonClick())
			{
				if( m_gui_state == GUI_STATE::MOUSE_LBUTTONDOWN )
				{
					HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
					SetCursor(hCursor);

					light::POSITION_INT PositionGap = m_l_butto_click_position - m_mouse_position;
					LONG gap = abs(PositionGap.X()) + abs(PositionGap.Y());
					if( 10 < gap )
					{
						m_gui_state = GUI_STATE::MOUSE_DRAG;
						TopMost();
					}
				}
			}
			else
			{
				if( m_gui_state == GUI_STATE::MOUSE_DRAG )
					m_gui_state = GUI_STATE::MOUSE_DROP;
				else
					m_gui_state = GUI_STATE::NONE;
			}
		}
		else
		{
			m_gui_state = GUI_STATE::NONE;
		}
	}

	MouseEvent();

	AppenderProcess();
	return m_gui_state;
}

void Gui::MouseEvent()
{
	switch( m_gui_state ) 
	{
	case GUI_STATE::MOUSE_DROP:
		{
			if( m_property & PROPERTY::MOVABLE_PLACEHOLDER )
			{
				light::weak_raw_ptr<Gui> pCurrent = m_direct_x->GuiMgr()->Crash(m_position, PROPERTY::PLACEHOLDER);
				light::weak_raw_ptr<Gui> pDestination = m_direct_x->GuiMgr()->Crash(m_mouse_position, PROPERTY::PLACEHOLDER);
				if( pDestination.get() ) 
				{
					if( pDestination->GetRect() != GetRect() )
						break;

					light::weak_raw_ptr<Gui> pDestinationAppend = pDestination->AppendPtr();
					light::weak_raw_ptr<Gui> pCurrentAppend;
					if( pCurrent.get() )
						pCurrentAppend = pCurrent->AppendPtr();

					if( pDestinationAppend.get() )   
					{
						light::POSITION_INT Temp = m_position;
						if( false == pDestination->Append(this) )
						{
							pCurrent->Link(pCurrentAppend);
							break;
						}

						if( pCurrent.get() ) 
						{
							if( false == pCurrent->Append(pDestinationAppend) )
							{
								pCurrent->Link(pCurrentAppend);
								break;
							}
						}

						m_position = pDestination->GetPosition();  
						pDestinationAppend->SetPosition(Temp); // swap position.
						pDestinationAppend->TopMost();
					}
					else 
					{
						if( false == pDestination->Append(this) )
						{
							pCurrent->Link(pCurrentAppend);
							break;
						}

						if( pCurrent.get() ) 
						{
							if( false == pCurrent->Append(NULL) )
							{
								// rollback
								pCurrent->Link(pCurrentAppend);
								break;
							}
						}

						m_position = pDestination->GetPosition();
					}
				}
			}
			else
			{
				// Drop failed. drop to original position.
				ToCenter(m_mouse_position);
			}

			if(m_show)
			{
				TopMost();
			}

			m_gui_state = GUI_STATE::NONE;
		}
		break;
	}
}

void Gui::AppenderProcess()
{
	if( m_property & PROPERTY::SHOW_ON_MOUSEOVER )
	{
		switch(m_gui_state)
		{
		case GUI_STATE::NONE:
			if(m_append_gui.get())
			{
				m_append_gui->Hide();
			}
			break;
		case GUI_STATE::MOUSE_OVER:
			if(m_append_gui.get())
			{
				m_append_gui->Show(m_position + m_adjust);
			}
			break;
		}
	}
}

void Gui::Message(const std::wstring& message, int FontSize, DWORD Color, const FreeType2::Align::Type align)
{
	m_message = message;
	m_font_size = FontSize;
	m_message_color = Color;
	m_align = align;
	Set();
}

void Gui::MessageAdjust(const light::POSITION_INT& Adjust)
{
	m_text_adjust = Adjust;
}

std::wstring Gui::GetString()
{
	return m_message;
}

void Gui::DrawString(const light::POSITION_INT& Position)
{
	if(false == m_message.empty())
	{
		m_direct_x->DrawString(m_message, Position + m_text_adjust, m_font_size, m_message_color);
	}
}

void Gui::DrawString(const light::POSITION_INT& Position, const light::RECT_INT& Rect, const FreeType2::Align::Type align)
{
	if(false == m_message.empty())
	{
		m_direct_x->DrawString(m_message, Position + m_text_adjust, Rect, m_font_size, m_message_color, align);
	}
}

void Gui::DrawString(const std::wstring& strMessage, const light::POSITION_INT& Position, const light::RECT_INT& Rect, const int FontSize, const DWORD Color, const FreeType2::Align::Type align)
{
	m_direct_x->DrawString(strMessage, Position, Rect, FontSize, Color, align);
}

void Gui::DrawString(const std::wstring& strMessage, const light::POSITION_INT& Position, const int FontSize, const DWORD Color)
{
	m_direct_x->DrawString(strMessage, Position, FontSize, Color);
}


bool Gui::Append(light::weak_raw_ptr<Gui> pGui, const light::POSITION_INT& Adjust)
{
	m_append_gui = pGui;
	m_adjust = Adjust;
	return true;
}

light::weak_raw_ptr<Gui> Gui::AppendPtr()
{
	return m_append_gui;
}

light::RECT_INT Gui::GetResizeRect(float fRate)
{
	return m_sprite.GetResizeRect(0, fRate);
}

Frame2D& Gui::GetFrame(Layer2D::NO LayerNo, Frame2D::NO FrameNo)
{
	return m_sprite.GetFrame(LayerNo, FrameNo);
}

int Gui::NowFrame() const
{
	return m_frame;
}

void Gui::SetCallback(Callback callback)
{
	m_callback = callback;
}

light::RECT_INT Gui::GetRect(Layer2D::NO LayerNo, Frame2D::NO no)
{
	return m_sprite.GetRect(LayerNo, no);
}

light::RECT_INT Gui::GetRect()
{
	return m_sprite.GetRect(0, 0);
}

light::POSITION_INT Gui::GetPosition()
{
	return m_position;
}

void Gui::SetPosition(const light::POSITION_INT& Position)
{
	m_position = Position;
}

void Gui::OnDraw()
{
	if( m_sprite.isLoad() == false)
		return;

	if(m_gui_state == GUI_STATE::MOUSE_DRAG)
	{
		DrawSprite( 0, GetPosition(), NowFrame(), false, 0.0f, 128.0f );
		DrawString(GetPosition(), GetRect(), m_align);

		light::POSITION_INT Position = m_mouse_position;
		ToCenter(Position);
		DrawSprite( 0, Position, NowFrame(), false, 0.0f, 255.0f );
		DrawString(Position, GetRect(), m_align);
	}
	else
	{
		DrawSprite(0, GetPosition(), NowFrame());
		DrawString(GetPosition(), GetRect(), m_align);
	}
}

void Gui::DrawSprite(Layer2D::NO LayerNo, const light::POSITION_INT& Position, int frame, const bool Mirror /* = false */, float fRot /* = 0.0f */, float fAlpha /* = ALPHA_USE_ORIGINAL */)
{
	m_sprite.DrawSprite(LayerNo, Position, frame, Mirror, fRot, fAlpha);
}

void Gui::ToCenter(light::POSITION_INT& Position)
{
	Position.X(Position.X() - GetRect().Width() / 2);
	Position.Y(Position.Y() - GetRect().Height() / 2);
}

void Gui::Link(light::weak_raw_ptr<Gui> pGui)
{
	m_append_gui = pGui;
}

} // namespace view

} // namespace view