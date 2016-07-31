#include "light-view.h"
#include "GuiProgressBar.h"

namespace light
{

namespace view
{


GuiProgressBar::GuiProgressBar()
: m_Max(0.0f)
{
}

GuiProgressBar::~GuiProgressBar()
{
}

bool GuiProgressBar::Create(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, light::POSITION_INT Pos, float Max, size_t Size, const Direction::Type direction)
{
	m_direct_x = directX;
	SetPosition(Pos);
	m_Max = Max;
	m_progresses.resize(Size);
	m_progresses[0] = Max; // first layer is used background.
	m_direction = direction;

	if( false == Load(file_name) )
		return false;

	light::RECT_FLOAT OriginalRect = GetFrame(0).OriginalRect(); // precondition is all image is same size.
	switch(m_direction)
	{
	case Direction::LEFT:
		m_adjust = light::POSITION_INT((LONG)OriginalRect.Width(), 0);
		break;
	case Direction::UP:
		m_adjust = light::POSITION_INT(0, (LONG)OriginalRect.Height());
		break;
	}

	for( Layer2D::NO n = 0 ; n < Size; ++n )
	{
		SetProgress(n, 0);
	}
	return true;
}

/**
@brief 버튼이 눌렸는지 확인합니다.
*/
void GuiProgressBar::OnLogic()
{
}

/**
@brief 프로그레스 상태 설정
@param LayerNo 레이어
@param fProgress 진행률
*/
void GuiProgressBar::SetProgress(Layer2D::NO LayerNo, float Progress)
{
	if(Progress < 0.0f)
		Progress = 0.0f;
	else if(m_Max < Progress)
		Progress = m_Max;

	m_progresses[LayerNo] = Progress;

	float fBeforePosition = 0.0f;
	for(Layer2D::NO no = 1; no < m_progresses.size(); ++no)
	{
		light::RECT_FLOAT OriginalRect = GetFrame(no).OriginalRect();
		switch(m_direction)
		{
		case Direction::LEFT:
			{
				light::RECT_FLOAT rect(-OriginalRect.Width() * fBeforePosition, 0.0f, -OriginalRect.Width() * (m_progresses[no] / m_Max), OriginalRect.Height());
				fBeforePosition += m_progresses[no] / m_Max;
				GetFrame(no).Rect(rect);
			}
			break;
		case Direction::RIGHT:
			{
				light::RECT_FLOAT rect(OriginalRect.Width() * fBeforePosition, 0.0f, OriginalRect.Width() * (m_progresses[no] / m_Max), OriginalRect.Height());
				fBeforePosition += m_progresses[no] / m_Max;
				GetFrame(no).Rect(rect);
			}
			break;
		case Direction::UP:
			{
				light::RECT_FLOAT rect(0.0f, -OriginalRect.Height() * fBeforePosition, OriginalRect.Width(), -OriginalRect.Height() * (m_progresses[no] / m_Max));
				fBeforePosition += m_progresses[no] / m_Max;
				GetFrame(no).Rect(rect);
			}
			break;
		case Direction::DOWN:
			{
				light::RECT_FLOAT rect(0.0f, OriginalRect.Height() * fBeforePosition, OriginalRect.Width(), OriginalRect.Height() * (m_progresses[no] / m_Max));
				fBeforePosition += m_progresses[no] / m_Max;
				GetFrame(no).Rect(rect);
			}
			break;
		}
	}
}

/**
@brief 버튼을 화면에 그립니다.
*/
void GuiProgressBar::OnDraw()
{
	DrawSprite(0, GetPosition(), NowFrame());
	for(Layer2D::NO n = 1; n < m_progresses.size(); ++n) 
	{
		DrawSprite(n, GetPosition() + m_adjust, NowFrame());
	}
}

} // namespace view

} // namespace view
