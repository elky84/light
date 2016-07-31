#include "light-view.h"
#include "GuiManager.h"
#include <algorithm>

#include "DirectX.h"

namespace light
{

namespace view
{

GuiManager::GuiManager(light::weak_raw_ptr<DirectX> directX)
	: m_direct_x(directX)
{
}

GuiManager::~GuiManager()
{

}
bool GuiManager::Set(Gui* pGui)
{
	LIST_GUI::iterator it = find(m_list_Gui.begin(), m_list_Gui.end(), pGui);
	if(it != m_list_Gui.end())
	{
		LOG_ERROR(L"%s Duplicate Set. %p", __FUNCTIONW__, pGui);
		return false;
	}

	m_list_Gui.push_back(pGui);
	return true;
}

bool GuiManager::Release(Gui* pGui)
{
	if( m_Focus.get() == pGui)
		m_Focus.reset();

	LIST_GUI::iterator it = find(m_list_Gui.begin(), m_list_Gui.end(), pGui);
	if(it == m_list_Gui.end())
	{
		LOG_ERROR(L"%s Not exist. can't release. %p", __FUNCTIONW__, pGui);
		return false;
	}

	m_list_Gui.erase(it);
	return true;
}

void GuiManager::Release()
{
	light::weak_raw_ptr<Gui> pFocus = m_Focus;

	LIST_GUI listGui = m_list_Gui;
	for each(auto& pGui in listGui)
	{
		pGui->Link(NULL);
		pGui->Release();
	}
}

void GuiManager::OnDraw()
{
	RenderState state1(m_direct_x->DxDevice(), D3DRS_LIGHTING);
	RenderState state2(m_direct_x->DxDevice(), D3DRS_ZENABLE);

	// set vertex format. 버텍스 타입을 하나만 사용하기에 이렇게 지정.
	m_direct_x->DxDevice()->SetFVF(view::Vertex2D::FVF);

	for each(Gui* pGui in m_list_Gui)
	{
		pGui->OnDraw();
	}
}

void GuiManager::Check()
{
	light::weak_raw_ptr<Gui> pFocus = m_Focus;

	LIST_GUI listGui = m_list_Gui;
	for each(auto& pGui in listGui)
	{
		GUI_STATE::Type GuiState = pGui->Check();
		if( GuiState == GUI_STATE::MOUSE_LBUTTONDOWN)
		{
			if(pFocus != pGui)
			{
				if( pFocus.get() )
				{
					pFocus->OnLostFocus();
				}

				m_Focus.reset(pGui);
				m_Focus->OnAcquireFocus();
			}
		}
		else if( GuiState == GUI_STATE::MOUSE_DRAG )
		{
			m_Drag.reset(pGui);
		}
		else if( GuiState == GUI_STATE::NONE )
		{
			if( m_Drag == pGui )
			{
				m_Drag.reset();
			}
		}
	}
}

void GuiManager::OnTab()
{
	light::weak_raw_ptr<Gui> pFocus = m_Focus;
	if(pFocus.get() == NULL)
		return;

	if( m_list_Gui.empty() )
		return;

	LIST_GUI::iterator it = find(m_list_Gui.begin(), m_list_Gui.end(), pFocus.get());
	if(it == m_list_Gui.end())
	{
		LOG_ERROR(L"%s Not exist. can't release. %p", __FUNCTIONW__, pFocus.get());
		return;
	}

	++it;
	if(it == m_list_Gui.end())
		it = m_list_Gui.begin();

	light::weak_raw_ptr<Gui> pNext;
	while( it != m_list_Gui.end())
	{
		if ( Gui::PROPERTY::EDITBABLE == (*it)->Property() )
		{
			pNext = *it;
			break;
		}

		++it;
	}

	if( pNext.get() == NULL )
		return;

	if( pFocus.get() )
	{
		pFocus->OnLostFocus();
	}

	m_Focus = pNext;
	m_Focus->OnAcquireFocus();
}

light::weak_raw_ptr<Gui> GuiManager::Crash(const light::POSITION_INT& Position, const Gui::PROPERTY::Type property)
{
	for each(auto& pGui in m_list_Gui)
	{
		if( property != pGui->Property() )
			continue;

		if ( true == crash_check(pGui->GetRect() + pGui->GetPosition(), Position) )
		{
			return pGui;
		}
	}
	return NULL;
}

light::weak_raw_ptr<Gui> GuiManager::DragPtr()
{
	return m_Drag;
}

void GuiManager::OnLogic()
{
	if(m_list_Gui.empty())
		return;

	LIST_GUI listGui = m_list_Gui;
	for each(Gui* pGui in listGui)
	{
		pGui->OnLogic();
	}

	if(m_Focus.get())
	{
		m_Focus->OnFocus();
	}
}

} // namespace view

} // namespace view
