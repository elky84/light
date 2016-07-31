#pragma once

#include "Position.h"
#include "Layer2D.h"

#include "Crop.h"

namespace light
{

namespace view
{

class Sprite2D
{
private:
	struct Information
	{
	public:
		Information()
			: m_nNowDelay(0)
			, m_nNowFrame(0)
		{
		}

	public:
		std::vector<Frame2D> m_vectorFrame;

		unsigned int m_nNowDelay;

		unsigned int m_nNowFrame;

		Polygon2D	m_polygon;
	};

	typedef std::vector<Information> VECTOR_SPRITE;

public:
	Sprite2D();

	~Sprite2D();

	bool Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, std::wstring strTexture = L"");

	light::RECT_INT GetRect(Layer2D::NO LayerNo, Frame2D::NO no = 0);

	light::RECT_INT GetResizeRect(Layer2D::NO LayerNo, float fRate);

	void DrawSprite(Layer2D::NO LayerNo, light::POSITION_INT Pos, unsigned int drawingframe, const bool Mirror = false, float fRot = 0.0f, float fAlpha = ALPHA_USE_ORIGINAL);

	bool Update(Layer2D::NO LayerNo, unsigned int Delay);

	Frame2D& GetFrame(Layer2D::NO LayerNo, int frame);

	void Set(light::weak_raw_ptr<DirectX> directX, Crop::VECTOR_TEXTURE& textures, Crop::MAP_CROP_INFO& crop_infos);

	const int MaxFrame(const Layer2D::NO layer) const;

	const Layer2D::NO MaxLayer() const;

	bool isLoad() const;

private:
	VECTOR_SPRITE m_sprites;
};

} // namespace view

} // namespace view
