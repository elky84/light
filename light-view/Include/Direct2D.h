#pragma once

#include "DirectX.h"

namespace light
{

namespace view
{

class Direct2D 
	: public DirectX
{
public:
	Direct2D(const int DevID = DEV_DIRECTX_ID);

	virtual ~Direct2D();

private:
	virtual void Setup();

	virtual void SetupParameter();

};

} // namespace view

} // namespace view
