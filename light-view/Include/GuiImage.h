#pragma once

#include "Gui.h"
#include "FmodEx.h"

namespace light
{

namespace view
{

class GuiImage : public Gui
{
public:
	GuiImage();

	virtual ~GuiImage();

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const light::POSITION_INT& Pos);

	bool Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, const std::wstring& strTextureName, const light::POSITION_INT& Pos);

private:
	virtual void OnLogic();

	virtual GUI_STATE::Type Check();
};

} // namespace view

} // namespace view
