#include "stdafx.h"
#include "TestMode.h"

#include "Input.h"

#include <boost/bind.hpp>

TestMode::TestMode(light::weak_raw_ptr<light::view::DirectX> directX)
: light::view::Mode(0, directX)
{
	m_Slider.Create(directX, L"data\\Slider\\Slider.spr", light::POSITION_INT(350, 50));
}

TestMode::~TestMode()
{

}

void TestMode::Begin()
{
	m_Slider.Set();
}

void TestMode::End()
{
}

void TestMode::Render()
{
}

void TestMode::RenderPostUI()
{
}

void TestMode::Logic()
{
}