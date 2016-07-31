#include "light-view.h"
#include "Animation2D.h"

#include "DirectoryUtil.h"

namespace light
{

namespace view
{
	
Animation2D::Animation2D()
: m_play_time(0)
, m_play(true)

{
}

Animation2D::~Animation2D()
{
}

void Animation2D::reset()
{
	for ( unsigned int i = 0;i < m_layers.size();i++ )
	{
		m_layers[i].reset();
	}
}

bool Animation2D::Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, std::wstring strTexture)
{
	Crop::MAP_CROP_INFO crop_infos;
	Crop::VECTOR_TEXTURE textures;
	if( false == Crop::ReadAni(file_name, textures, crop_infos) )
	{
		LOG_ERROR(L"Crop::ReadAni() Failed. %s", file_name.c_str());
		return false;
	}

	if( false == strTexture.empty())
	{
		const std::wstring strPath = light::get_path_without_file_name(file_name);
		for(size_t n = 0; n < crop_infos.size(); ++n)
			textures.push_back(strPath + strTexture);
	}

	Set(directX, textures, crop_infos);

	reset();
	Play();	
	return true;
}

void Animation2D::Set(light::weak_raw_ptr<DirectX> directX, Crop::VECTOR_TEXTURE& textures, Crop::MAP_CROP_INFO& crop_infos)
{
	m_layers.clear();
	m_layers.resize(crop_infos.rbegin()->first + 1);

	for each(const view::Crop::MAP_CROP_INFO::value_type& val in crop_infos)
	{
		int LayerNo = val.first;
		m_layers[LayerNo].SetDirectX(directX);

		const std::wstring& strTexture = light::Directory::path_canonicalize(textures[LayerNo]);
		m_layers[LayerNo].LoadTexture(strTexture);

		for each(const view::CROP_INFO& CropInfo in val.second)
		{
			Frame2D frame(CropInfo.Color, light::convert<float>(CropInfo.Cut), light::convert<float>(CropInfo.Rect), 0);

			m_layers[LayerNo].Interpolate(frame, view::Frame2D::NO(CropInfo.Frame));
		}
	}
}


void Animation2D::Stop()
{
	m_play = false;
}


void Animation2D::Play()
{
	m_play = true;
}

bool Animation2D::NowPlaying()
{
	if( isPlay() == false )
		return false;

	unsigned int nCheck = 0;
	for ( unsigned int i = 0; i < m_layers.size();i++ )
	{
		Layer2D& layer = m_layers[i];

		if ( true == layer.isComplete() )
		{
			++nCheck;
		}
	}
	return nCheck == m_layers.size() ? false : true;
}

void Animation2D::Position(const light::POSITION_INT& Pos)
{
	m_global_position = Pos;
}

void Animation2D::Update()
{
	if (m_play == false)
		return;

	for ( unsigned int i = 0;i < m_layers.size();i++ )
	{
		m_layers[i].Update();			
	}
}

void Animation2D::Draw(const bool Mirror)
{
	for ( size_t i = 0; i < m_layers.size(); i++ )
	{
		m_layers[i].Render( m_global_position, Mirror ); // no rotate
	}
}

void Animation2D::Draw(const int layer, const bool Mirror)
{
	m_layers[layer].Render( m_global_position, Mirror ); // no rotate
}

void Animation2D::Draw(const int layer, const float degree, const bool Mirror)
{
	m_layers[layer].Render( m_global_position, Mirror, degree ); // no rotate
}

void Animation2D::Draw(const light::POSITION_INT& Pos, const int layer, const float degree, const bool Mirror /*= false*/)
{
	Position(Pos);
	Draw(layer, degree, Mirror);
}

void Animation2D::DrawLoop(const bool Mirror)
{
	Update();
	Draw(Mirror);

	if( false == NowPlaying())
	{
		reset();
	}
}

void Animation2D::DrawLoop(const int layer, const bool Mirror)
{
	Update();
	Draw(layer, Mirror);

	if( false == NowPlaying())
	{
		reset();
	}
}

void Animation2D::DrawLoop(const int layer, const float degree, const bool Mirror)
{
	Update();
	Draw(layer, degree, Mirror);

	if( false == NowPlaying())
	{
		reset();
	}
}

void Animation2D::DrawLoop(const light::POSITION_INT& Pos, const int layer, const float degree, const bool Mirror /* = false */)
{
	Position(Pos);
	Update();
	Draw(layer, degree, Mirror);

	if( false == NowPlaying())
	{
		reset();
	}
}

} // namespace view

} // namespace view
