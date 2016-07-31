#pragma once

#include "Gui.h"

namespace light
{

namespace view
{

class GuiPlaceholder 
	: public Gui
{
public:
	GuiPlaceholder();

	virtual ~GuiPlaceholder();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Position, Gui::Callback callback = NULL);

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTexture, const light::POSITION_INT& Position, Gui::Callback callback = NULL);

	virtual bool Append(light::weak_raw_ptr<Gui> pGui, const light::POSITION_INT& Adjust = light::POSITION_INT(0, 0));

	virtual void Set();

	virtual void Release();

private:
	virtual void OnLogic();

};

} // namespace view

} // namespace view
