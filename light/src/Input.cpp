#include "light.h"

#include "Input.h"

namespace light
{

namespace Input
{

light::POSITION_INT Mouse::m_os;
light::POSITION_INT Mouse::m_LButtonClickPos;
light::POSITION_INT Mouse::m_RButtonClickPos;

int Mouse::m_Wheel = 0;

bool Mouse::m_LButtonClick = false;
bool Mouse::m_LButtonDoubleClick = false;

bool Mouse::m_RButtonClick = false;
bool Mouse::m_RButtonDoubleClick = false;


light::POSITION_INT& Mouse::Pos()
{
	return m_os;
}

void Mouse::Pos(const light::POSITION_INT& pos)
{
	m_os = pos;
}

void Mouse::LButtonClickPos(const light::POSITION_INT& pos)
{
	m_LButtonClickPos = pos;
}

light::POSITION_INT& Mouse::LButtonClickPos()
{
	return m_LButtonClickPos;
}

void Mouse::RButtonClickPos(const light::POSITION_INT& pos)
{
	m_RButtonClickPos = pos;
}

light::POSITION_INT& Mouse::RButtonClickPos()
{
	return m_RButtonClickPos;
}

void Mouse::LButtonClick(const bool flag)
{
	m_LButtonClick = flag;
}

void Mouse::RButtonClick(const bool flag)
{
	m_RButtonClick = flag;
}

void Mouse::LButtonDoubleClick(const bool click)
{
	m_LButtonDoubleClick = click;
}

void Mouse::RButtonDoubleClick(const bool click)
{
	m_RButtonDoubleClick = click;
}

void Mouse::Wheel(const int nWheel)
{
	m_Wheel = nWheel;
}

bool Mouse::LButtonClick()
{
	return m_LButtonClick;
}

bool Mouse::RButtonClick()
{
	return m_RButtonClick;
}

bool Mouse::LButtonDoubleClick()
{
	return m_LButtonDoubleClick;
}

bool Mouse::RButtonDoubleClick()
{
	return m_RButtonDoubleClick;
}

int Mouse::Wheel()
{
	return m_Wheel;
}

void Mouse::reset()
{
	m_Wheel = false;

	m_LButtonClick = false;
	m_RButtonClick = false;

	m_LButtonDoubleClick = false;
	m_RButtonDoubleClick = false;
}

std::vector<bool> Keyboard::m_key(MAX_KEY);

void Keyboard::Up(int key)
{
	m_key[key] = false;
}

void Keyboard::Down(int key)
{
	m_key[key] = true;
}

bool Keyboard::Pressing(int key)
{
	return m_key[key];
}

void Keyboard::reset()
{
	m_key.clear();
	m_key.resize(MAX_KEY);
}

} // namespace Input

} //namespace light
