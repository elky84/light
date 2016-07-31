#pragma once

#include <vector>
#include <map>

#include "Position.h"
#include "Rectangle.h"

namespace light
{

class CommandLineArguments;

} // namespace view

namespace light
{

namespace view
{

struct CROP_INFO
{
	CROP_INFO(const int frame, const light::RECT_INT& rect, const light::RECT_INT& cut)
		: Frame(frame)
		, Rect(rect)
		, Cut(cut)
		, Color(255.0f, 255.0f, 255.0f, 255.0f)
		, Rotate(0)
	{

	}

	CROP_INFO()
		: Frame(0)
		, Color(255.0f, 255.0f, 255.0f, 255.0f)
		, Rotate(0)
	{

	}

	int Frame;

	light::RECT_INT Rect;

	light::RECT_INT Cut;

	light::RECT_INT Attack;

	light::RECT_INT Damage;

	Colors<float> Color;

	float Rotate;
};

class Crop
{
public:
	typedef std::vector<CROP_INFO> VECTOR_CROP_INFO;
	typedef std::map<int, VECTOR_CROP_INFO> MAP_CROP_INFO;
	typedef std::vector<std::wstring> VECTOR_TEXTURE;

public:
	Crop();

	~Crop();
	
private:
	static bool Read(const std::wstring& strName, VECTOR_TEXTURE& textures, MAP_CROP_INFO& crop_infos, const std::wstring& strTag, const std::wstring& strLayerTag);

public:
	static bool ReadSpr(const std::wstring& strName, VECTOR_TEXTURE& textures,  MAP_CROP_INFO& crop_infos);

	static bool ReadAni(const std::wstring& strName, VECTOR_TEXTURE& textures,  MAP_CROP_INFO& crop_infos);

public:
	bool WriteSpr();

	bool WriteAni();

	void Slice();

private:
	bool Write(const std::wstring& strTag, const std::wstring& strLayerTag);

public:
	bool ParseCmdLine();

	bool DeployAni(const std::wstring& strFile, const std::wstring& strTexture, MAP_CROP_INFO& crop_infos);

	bool DeploySpr(const std::wstring& strFile, const std::wstring& strTexture, MAP_CROP_INFO& crop_infos);

private:
	void HorizontalSlice();

	void VerticalSlice();

private:
	std::wstring m_strFile;

	int m_x, m_y, m_width, m_height, m_CutWidth, m_CutHeight, m_frames;

	std::wstring m_direction;

	VECTOR_TEXTURE m_textures;

	MAP_CROP_INFO m_crop_infos;
};

} // namespace view

} // namespace view
