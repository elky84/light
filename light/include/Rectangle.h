#pragma once

#include "Position.h"

namespace light
{
template <typename T>
class Rectangle
{
public:
	Rectangle() 
		: m_LeftTop(0,0)
		, m_RightBottom(0,0) 
	{
	}

	Rectangle(const T left,const T top, const T right, const T bottom) 
		: m_LeftTop(left, top)
		, m_RightBottom(right, bottom) 
	{
	}

	Rectangle(const RECT& rect)
		: m_LeftTop(rect.left, rect.top)
		, m_RightBottom(rect.right, rect.bottom)
	{
	}

	Rectangle<T> operator+(const Position<T>& pos) const
	{
		Rectangle<T> ret(*this);
		return ret+=pos;
	}


	Rectangle<T> operator+(const Rectangle<T>& rhs) const
	{
		Rectangle<T> ret(*this);
		return ret+=rhs;
	}

	Rectangle<T>& operator+=(const Position<T>& pos)
	{
		m_LeftTop += pos;
		m_RightBottom += pos;
		return *this;
	}

	Rectangle<T>& operator+=(const Rectangle<T>& rhs)
	{
		m_LeftTop += rhs.m_LeftTop;
		m_RightBottom += rhs.m_RightBottom;
		return *this;
	}

	Rectangle<T> operator-(Position<T>& pos) const
	{
		Rectangle<T> ret(*this);
		return ret-=pos;
	}

	Rectangle<T>& operator-=(const Position<T>& pos)
	{
		m_LeftTop -= pos;
		m_RightBottom -= pos;
		return *this;
	}

	bool operator==(const Rectangle<T>& other) const
	{
		return (m_LeftTop == other.m_LeftTop && m_RightBottom == other.m_RightBottom);
	}

	Rectangle<T> operator-(const Rectangle<T>& rect)
	{
		Rectangle<T> ret;
		ret.m_LeftTop = m_LeftTop - rect.m_LeftTop;
		ret.m_RightBottom = m_RightBottom - rect.m_RightBottom;
		return ret;
	}

	bool operator!=(const Rectangle<T>& other) const
	{
		return (m_LeftTop != other.m_LeftTop || m_RightBottom != other.m_RightBottom);
	}

	void ToRect(RECT& rect)
	{
		rect.top = m_LeftTop.Y();
		rect.left = m_LeftTop.X();
		rect.bottom = m_RightBottom.Y();
		rect.right = m_RightBottom.X();
	}

	void Clear()
	{
		m_LeftTop.Clear();
		m_RightBottom.Clear();
	}

	void set(const T x, const T y, const T r, const T b)
	{
		Left(l);
		Top(t);
		Right(r);
		Bottom(b);
	}

	T Left() const
	{
		return m_LeftTop.X();
	}

	T Top() const
	{
		return m_LeftTop.Y();
	}

	T Right() const
	{
		return m_RightBottom.X();
	}

	T Bottom() const
	{
		return m_RightBottom.Y();
	}

	T Width() const
	{
		return Right() - Left();
	}

	T Height() const
	{
		return Bottom() - Top();
	}

	void Left(const T t)
	{
		m_LeftTop.X(t);
	}

	void Top(const T t)
	{
		m_LeftTop.Y(t);
	}

	void Right(const T t)
	{
		m_RightBottom.X(t);
	}

	void Bottom(const T t)
	{
		m_RightBottom.Y(t);
	}

	Position<T> LeftTop() const
	{
		return m_LeftTop;
	}

	Position<T> RightBottom() const
	{
		return m_RightBottom;
	}

private:
	Position<T>	m_LeftTop;

	Position<T>	m_RightBottom;
};

typedef Rectangle<float> RECT_FLOAT;
typedef Rectangle<int> RECT_INT;

template <typename T, typename RectType>
Rectangle<T> convert(const RectType& rect)
{
	return Rectangle<T>((T)rect.Left(), (T)rect.Top(), (T)rect.Right(), (T)rect.Bottom());
}

template <typename Rect, typename Position>
inline bool crash_check(const Rect& rectangles, const Position& pt)
{
	if(	rectangles.Left() <= pt.X() &&
		pt.X() <= rectangles.Right() &&
		rectangles.Top() <= pt.Y() && 
		pt.Y() <= rectangles.Bottom() )
	{
		return true;
	}
	return false;
}


} //namespace light
