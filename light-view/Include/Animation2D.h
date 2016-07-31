#pragma once

#include "Str.h"
#include "Layer2D.h"
#include "Crop.h"

#include <vector>

namespace light
{

namespace view
{

class Animation2D
{
public:
	Animation2D();
	~Animation2D();

	bool Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& file_name, std::wstring strTexture = L"");

	void Position(const light::POSITION_INT& Pos);

	void Draw(const bool Mirror = false);
	void Draw(const int layer, const bool Mirror = false);
	void Draw(const int layer, const float degree, const bool Mirror = false);
	void Draw(const light::POSITION_INT& Pos, const int layer, const float degree, const bool Mirror = false);

	void DrawLoop(const bool Mirror = false);
	void DrawLoop(const int layer, const bool Mirror = false);
	void DrawLoop(const int layer, const float degree, const bool Mirror = false);
	void DrawLoop(const light::POSITION_INT& Pos, const int layer, const float degree, const bool Mirror = false);

	void reset();
	void Update();
	void Play();
	void Stop();
	bool NowPlaying();

	bool isPlay() const
	{
		return m_play;
	}

	light::POSITION_INT& GetPosition()
	{
		return m_global_position;
	}

	Layer2D& GetLayer(Layer2D::NO no)
	{
		return m_layers[no];
	}

	void Set(light::weak_raw_ptr<DirectX> directX, Crop::VECTOR_TEXTURE& textures, Crop::MAP_CROP_INFO& crop_infos);

private:
	light::POSITION_INT m_global_position;

	std::vector<Layer2D> m_layers;

	int	m_play_time;

	bool m_play;
};

} // namespace view

} // namespace view
