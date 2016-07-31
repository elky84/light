#pragma once

#include "Mode.h"
#include "weak_raw_ptr.hpp"

#include <map>
#include <memory>

namespace light
{

namespace view
{

class DirectX;

class ModeManager
{
private:
	typedef std::tr1::shared_ptr<Mode> SHARED_PTR_MODE;
	typedef std::map<Mode::ModeId, SHARED_PTR_MODE> MAP_MODE;

public:
	ModeManager(light::weak_raw_ptr<DirectX> directX);

	~ModeManager();

public:
	bool Active(const Mode::ModeId Id);

	bool Change(const Mode::ModeId Id);

	Mode* get(const Mode::ModeId Id);

	bool Reserve(const Mode::ModeId Id);

public:
	bool Register(Mode* pMode);

	bool Unregister(const Mode::ModeId Id);

public:
	void OnRender();

	void OnRenderPostUI();

	void OnLogic();

private:
	MAP_MODE m_modes;

	light::weak_raw_ptr<Mode> m_active_mode;

	light::weak_raw_ptr<DirectX> m_direct_x;

	Mode::ModeId m_reserve;
};

} // namespace view

} // namespace view
