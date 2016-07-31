#pragma once

#include "Tile.h"
#include <memory>

namespace light
{

namespace view
{

class DirectX;

struct TileProperty
{
	int X;
	int Y;
	int Type;
	int Value;
};

class TilePainter
{
private:
	typedef std::map<int, Tile> MAP_TILE;

	typedef std::vector<TileProperty> VECTOR_TILE_PROPERTY;

public:
	TilePainter(const light::RECT_INT& Rect);

	virtual ~TilePainter();

	bool Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& MapFile);
	
	void Render();

	virtual void AdjustPosition(light::POSITION_INT& Position);

	virtual bool CheckDraw(const int X, const int Y);

	void Draw(Layer2D::NO LayerNo, light::POSITION_INT Pos, unsigned int drawingframe);

	const int Width() const
	{
		return m_width;
	}

	const int Height() const
	{
		return m_height;
	}

	TileProperty& Property(const int X, const int Y);

protected:
	int m_width, m_height;

	std::unique_ptr<Tile> m_Tile;

	light::RECT_INT m_rect;

	Sprite2D m_sprite;

	VECTOR_TILE_PROPERTY m_vectorTileProperty;
};

class TilePainterScroll
	: public TilePainter
{
public:
	TilePainterScroll(const int& ScrollX, const int& ScrollY, const light::RECT_INT& Rect);

	virtual bool CheckDraw(const int X, const int Y);

	virtual void AdjustPosition(light::POSITION_INT& Position);

private:
	const int& m_ScrollX;

	const int& m_ScrollY;
};

} // namespace view

} // namespace view
