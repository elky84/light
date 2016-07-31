#pragma once

#include "DirectX.h"

namespace light
{

namespace view
{

class Direct3D : public DirectX
{
public:
	Direct3D(const int DevID = DEV_DIRECTX_ID);

	virtual ~Direct3D();

private:
	virtual void Setup();

	virtual void SetupParameter();
};

} // namespace view

} // namespace view
