#include "light-view.h"
#include "Sprite2D.h"

#include "DirectoryUtil.h"

namespace light
{

namespace view
{

Sprite2D::Sprite2D()
{
}

Sprite2D::~Sprite2D()
{
}


bool Sprite2D::Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, std::wstring strTexture)
{
	Crop::MAP_CROP_INFO crop_infos;
	Crop::VECTOR_TEXTURE textures;
	if( false == Crop::ReadSpr(file_name, textures, crop_infos) )
	{
		LOG_ERROR(L"Crop::ReadSpr() Failed. %s", file_name.c_str());
		return false;
	}

	if( false == strTexture.empty())
	{
		const std::wstring strPath = light::get_path_without_file_name(file_name);
		for(size_t n = 0; n < crop_infos.size(); ++n)
			textures.push_back(strPath + strTexture);
	}

	Set(directX, textures, crop_infos);
	return true;
}

bool Sprite2D::Update(Layer2D::NO LayerNo, unsigned int nDelay)
{
	if (m_sprites[LayerNo].m_nNowDelay == nDelay)
	{
		if (m_sprites[LayerNo].m_nNowFrame == m_sprites[LayerNo].m_vectorFrame.size()-1)
		{
			m_sprites[LayerNo].m_nNowFrame = 0;
			m_sprites[LayerNo].m_nNowDelay = 0;
			return true;
		}
		else
		{
			m_sprites[LayerNo].m_nNowFrame++;
		}
		m_sprites[LayerNo].m_nNowDelay = 0;
	}
	else
	{
		m_sprites[LayerNo].m_nNowDelay++;
	}
	return false;
}

void Sprite2D::DrawSprite(Layer2D::NO LayerNo, light::POSITION_INT Pos, unsigned int drawingframe, const bool Mirror, float fRot /* = 0 */, float fAlpha /* = ALPHA_USE_ORIGINAL */)
{
	if(m_sprites[LayerNo].m_vectorFrame.size() <= drawingframe)
		drawingframe = 0;

	Information& sprite = m_sprites[LayerNo];
	Frame2D& frame = sprite.m_vectorFrame[drawingframe];

	COLORS_FLOAT color = frame.Color();
	if( fAlpha != ALPHA_USE_ORIGINAL)
		color.A(fAlpha);

	sprite.m_polygon.SetTransformColor(color);
	sprite.m_polygon.Blt(light::RECT_FLOAT(frame.Rect().Left() + Pos.X(), frame.Rect().Top() + Pos.Y(), frame.Rect().Right(), frame.Rect().Bottom()), frame.UV(), Mirror, fRot != 0.0f ? fRot : frame.Rotate());
}

Frame2D& Sprite2D::GetFrame(Layer2D::NO LayerNo, int frame)
{
	Information& Information = m_sprites[LayerNo];
	return Information.m_vectorFrame[frame];
}

light::RECT_INT Sprite2D::GetRect(Layer2D::NO LayerNo, Frame2D::NO no)
{
	if(m_sprites[LayerNo].m_vectorFrame.size() <= no)
	{
		LOG_ERROR(L"%s Size [%u] Frame2D [%u]", __FUNCTIONW__, m_sprites[LayerNo].m_vectorFrame.size(), no);
		return light::RECT_INT(0, 0, 0, 0);
	}

	Frame2D& frame = m_sprites[LayerNo].m_vectorFrame[no];
	return light::RECT_INT((long)frame.Rect().Left(), (long)frame.Rect().Top(), (long)frame.Rect().Right(), (long)frame.Rect().Bottom());
}

light::RECT_INT Sprite2D::GetResizeRect(Layer2D::NO LayerNo, float fRate)
{
	light::RECT_INT rect = GetRect(LayerNo, 0);
	rect.Left((int)(1.0f - fRate) * rect.Right());
	rect.Top((int)(1.0f - fRate) * rect.Bottom());
	rect.Right((int)(rect.Right() * fRate));
	rect.Bottom((int)(rect.Bottom() * fRate));
	return rect;
}

void Sprite2D::Set(light::weak_raw_ptr<DirectX> directX, Crop::VECTOR_TEXTURE& textures, Crop::MAP_CROP_INFO& crop_infos)
{
	m_sprites.clear();
	m_sprites.resize(crop_infos.rbegin()->first + 1);

	for each(const view::Crop::MAP_CROP_INFO::value_type& val in crop_infos)
	{
		int LayerNo = val.first;
		m_sprites[LayerNo].m_polygon.SetDirectX(directX);

		const std::wstring& strTexture = light::Directory::path_canonicalize(textures[LayerNo]);
		m_sprites[LayerNo].m_polygon.LoadTexture(strTexture);

		for each(const view::CROP_INFO& CropInfo in val.second)
		{
			Frame2D frame(CropInfo.Color, light::convert<float>(CropInfo.Cut), light::convert<float>(CropInfo.Rect), 0);

			m_sprites[LayerNo].m_vectorFrame.push_back(frame);
		}
	}
}

const Layer2D::NO Sprite2D::MaxLayer() const
{
	return (Layer2D::NO)m_sprites.size();
}

const int Sprite2D::MaxFrame(const Layer2D::NO layer) const
{
	return (int)m_sprites[layer].m_vectorFrame.size();
}

bool Sprite2D::isLoad() const
{
	return m_sprites.empty() ? false : true;
}

} // namespace view

} // namespace view
