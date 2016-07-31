#pragma once

#include "FmodEx.h"

namespace light
{

namespace view
{

class FmodManager
	: public light::Thread
{


	static FMOD::System* System() 
	{
		return m_FmodSystem;
	}

private:
	FMOD::System* m_FmodSystem;
};

} // namespace view

} // namespace view
