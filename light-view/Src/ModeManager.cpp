#include "light-view.h"
#include "ModeManager.h"
#include "GuiManager.h"
#include "DirectX.h"

namespace light
{

namespace view
{

ModeManager::ModeManager(light::weak_raw_ptr<DirectX> directX)
	: m_direct_x(directX)
	, m_reserve(Mode::INVALID)
{
}

ModeManager::~ModeManager()
{
	m_modes.clear();
}

bool ModeManager::Active(const Mode::ModeId Id)
{
	Mode* mode = get(Id);
	if(mode == NULL)
	{
		LOG_ERROR(L"%s not find [%d]", __FUNCTIONW__, Id);
		return false;
	}

	m_active_mode = mode;
	m_active_mode->RequestBegin();
	m_direct_x->ResetCounter();
	return true;
}

bool ModeManager::Reserve(const Mode::ModeId Id)
{
	InterlockedExchange((LONG*)&m_reserve, Id);
	return true;
}

bool ModeManager::Change(const Mode::ModeId Id)
{
	MAP_MODE::iterator it = m_modes.find(Id);
	if(it == m_modes.end())
	{
		LOG_ERROR(L"%s not find [%d]", __FUNCTIONW__, Id);
		return false;
	}

	m_direct_x->GuiMgr()->Release();

	if(m_active_mode.get())
	{
		m_active_mode->RequestEnd(it->second.get());
	}
	return true;
}

Mode* ModeManager::get(const Mode::ModeId Id)
{
	MAP_MODE::iterator it = m_modes.find(Id);
	if(it == m_modes.end())
	{
		LOG_ERROR(L"%s not find [%d]", __FUNCTIONW__, Id);
		return NULL;
	}

	return it->second.get();
}

bool ModeManager::Register(Mode* pMode)
{
	SHARED_PTR_MODE Mode(pMode);
	if(m_modes.end() != m_modes.find(Mode->Id()))
	{
		LOG_ERROR(L"%s not find [%d]", __FUNCTIONW__, Mode->Id());
		return false;
	}

	m_modes.insert(MAP_MODE::value_type(Mode->Id(), Mode));
	return true;
}

bool ModeManager::Unregister(const Mode::ModeId Id)
{
	Mode* pMode = get(Id);
	if( NULL == pMode )
		return false;
	
	m_modes.erase(Id);
	return true;
}

void ModeManager::OnRender()
{
	if( NULL == m_active_mode.get())
		return;

	// 기본 오브젝트 드로우
	m_active_mode->Render();

	// GUI 드로우
	m_direct_x->GuiMgr()->OnDraw();

	// 포스트 드로우 오브젝트를 그리는 메소드
	m_active_mode->PostRender();
}

void ModeManager::OnRenderPostUI()
{
	if( NULL == m_active_mode.get())
		return;

	m_active_mode->RenderPostUI();
}

void ModeManager::OnLogic()
{
	if( NULL == m_active_mode.get())
		return;

	if( false == m_active_mode->isAnimating() )
	{
		m_direct_x->GuiMgr()->Check();
		m_direct_x->GuiMgr()->OnLogic();

		m_active_mode->LButtonClickCallback();
		m_active_mode->Logic();
	}

	if(m_reserve != Mode::INVALID)
	{
		Change(m_reserve);
		InterlockedExchange((LONG*)&m_reserve, Mode::INVALID);

	}
}

} // namespace view

} // namespace view
