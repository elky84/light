#include "light-view.h"
#include "Crop.h"
#include "XMLWriter.h"
#include "XMLParser.h"
#include "CommandLineArguments.h"

#include "DirectoryUtil.h"

namespace light
{

namespace view
{

bool Crop::Read(const std::wstring& strName, VECTOR_TEXTURE& textures, MAP_CROP_INFO& crop_infos, const std::wstring& strTag, const std::wstring& strLayerTag)
{
	light::XMLParser parser;
	if ( false == parser.read_file(strName) ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strName.c_str());
		return false;
	}

	if ( false == parser.execute( strTag ) ) 
	{
		LOG_ERROR(L"%s. %s execute failed.", __FUNCTIONW__, strName.c_str());
		return false;
	}

	if ( false == parser.bind_elem( strLayerTag ) ) 
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
		return false;
	}

	int LayerNo = 0;
	if ( false == parser.bind_attrib(L"Layer", LayerNo) ) 
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
		return false;
	}

	std::wstring strTexture;
	if ( false == parser.bind_attrib(L"TextureName", strTexture) )
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strTexture.c_str());
		return false;
	}

	const std::wstring strPath = light::get_path_without_file_name(strName);
	while (parser.next())
	{
		if( false == strTexture.empty())
		{
			textures.push_back(strPath + strTexture);
		}

		if ( false == parser.bind_elem(L"FrameInfo") ) 
		{
			LOG_ERROR(L"%s. %s bind_elem failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		int nFrame = 0;
		if ( false == parser.bind_attrib(L"Frame", nFrame) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float CutLeft = 0.0f;
		if ( false == parser.bind_attrib(L"CutLeft", CutLeft) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float CutTop = 0.0f;
		if ( false == parser.bind_attrib(L"CutTop", CutTop) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float CutRight = 0.0f;
		if ( false == parser.bind_attrib(L"CutRight", CutRight) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float CutBottom = 0.0f;
		if ( false == parser.bind_attrib(L"CutBottom", CutBottom) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float Left = 0.0f;
		if ( false == parser.bind_attrib(L"Left", Left) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float Top = 0.0f;
		if ( false == parser.bind_attrib(L"Top", Top) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float Right = 0.0f;
		if ( false == parser.bind_attrib(L"Right", Right) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float Bottom = 0.0f;
		if ( false == parser.bind_attrib(L"Bottom", Bottom) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float R = 0.0f;
		if ( false == parser.bind_attrib(L"R", R) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float G = 0.0f;
		if ( false == parser.bind_attrib(L"G", G) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float B = 0.0f;
		if ( false == parser.bind_attrib(L"B", B) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float A = 0.0f;
		if ( false == parser.bind_attrib(L"A", A) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		float Rotate = 0.0f;
		if ( false == parser.bind_attrib(L"Rotate", Rotate) )
		{
			LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, strName.c_str());
			return false;
		}

		while(parser.next())
		{
			light::RECT_INT UV((LONG)CutLeft, (LONG)CutTop, (LONG)CutRight, (LONG)CutBottom);
			_ASSERT(CutRight != 0);
			_ASSERT(CutBottom != 0);

			light::RECT_INT Rect((LONG)Left, (LONG)Top, (LONG)Right, (LONG)Bottom);

			view::Crop::VECTOR_CROP_INFO& vectorCropInfo = crop_infos[LayerNo];
			view::CROP_INFO CropInfo(nFrame, Rect, UV);
			CropInfo.Color = Colors<float>(R, G, B, A);
			CropInfo.Rotate = Rotate;

			vectorCropInfo.push_back(CropInfo);
		}
	}

	return true;
}

bool Crop::ReadAni(const std::wstring& strName, VECTOR_TEXTURE& textures, MAP_CROP_INFO& crop_infos)
{
	return Read(strName, textures, crop_infos, L"/Animation", L"LayerInfo");
}

bool Crop::ReadSpr(const std::wstring& strName, VECTOR_TEXTURE& textures, MAP_CROP_INFO& crop_infos)
{
	return Read(strName, textures, crop_infos, L"/Sprite", L"SpriteInfo");
}


Crop::Crop()
	: m_x(0)
	, m_y(0)
	, m_height(0)
	, m_width(0)
	, m_CutWidth(0)
	, m_CutHeight(0)
	, m_frames(0)
{

}

Crop::~Crop()
{

}

void Crop::Slice()
{
	if(m_direction == L"V")
	{
		VerticalSlice();
	}
	else // H or All
	{
		HorizontalSlice();
	}
}

bool Crop::DeploySpr(const std::wstring& strFile, const std::wstring& strTexture, MAP_CROP_INFO& crop_infos)
{
	m_strFile = strFile;
	m_textures.push_back(strTexture);
	m_crop_infos = crop_infos;
	return WriteSpr();
}

bool Crop::DeployAni(const std::wstring& strFile, const std::wstring& strTexture, MAP_CROP_INFO& crop_infos)
{
	m_strFile = strFile;
	m_textures.push_back(strTexture);
	m_crop_infos = crop_infos;
	return WriteAni();
}

bool Crop::ParseCmdLine()
{
	if ( false == CommandLineArguments::has(L"FORMAT") )
		return false;

	if ( false == CommandLineArguments::has(L"FILE") )
		return false;

	m_strFile = CommandLineArguments::to_str(L"FILE");

	if ( false == CommandLineArguments::has(L"TEXTURE") )
		return false;

	m_textures.push_back(CommandLineArguments::to_str(L"TEXTURE"));

	if ( false == CommandLineArguments::has(L"X") )
		return false;

	m_x = CommandLineArguments::to_int(L"X");

	if ( false == CommandLineArguments::has(L"Y") )
		return false;

	m_y = CommandLineArguments::to_int(L"Y");

	if ( false == CommandLineArguments::has(L"WIDTH") )
		return false;

	m_width = CommandLineArguments::to_int(L"WIDTH");

	if ( false == CommandLineArguments::has(L"HEIGHT") )
		return false;

	m_height = CommandLineArguments::to_int(L"HEIGHT");

	if ( false == CommandLineArguments::has(L"CUT_WIDTH") )
		return false;

	m_CutWidth = CommandLineArguments::to_int(L"CUT_WIDTH");

	if ( false == CommandLineArguments::has(L"CUT_HEIGHT") )
		return false;

	m_CutHeight = CommandLineArguments::to_int(L"CUT_HEIGHT");

	if ( false == CommandLineArguments::has(L"CUT_HEIGHT") )
		return false;

	m_frames = CommandLineArguments::to_int(L"FRAME");

	if ( false == CommandLineArguments::has(L"DIRECTION") )
		return false;

	m_direction = CommandLineArguments::to_str(L"DIRECTION");
	return true;
}

void Crop::HorizontalSlice()
{
	int CurLayer = 0;
	for(int y = (m_y / m_CutHeight); y < m_height / m_CutHeight; ++y, ++CurLayer)
	{
		int CurFrame = 0;
		for(int x = (m_x / m_CutWidth); x < m_width / m_CutWidth; ++x)
		{
			light::RECT_INT rect(0, 0, m_CutWidth, m_CutHeight);
			light::RECT_INT cut(x * m_CutWidth, y * m_CutHeight, (x + 1) * m_CutWidth, (y + 1) * m_CutHeight );

			m_crop_infos[CurLayer].push_back(CROP_INFO(CurFrame, rect, cut));
			CurFrame += m_frames;
		}
	}
}

void Crop::VerticalSlice()
{
	int CurLayer = 0;
	for(int x = (m_x / m_CutWidth); x < m_width / m_CutWidth; ++x, ++CurLayer)
	{
		int CurFrame = 0;
		for(int y = (m_y / m_CutHeight); y < m_height / m_CutHeight; ++y)
		{
			light::RECT_INT rect(0, 0, m_CutWidth, m_CutHeight);
			light::RECT_INT cut(x * m_CutWidth, y * m_CutHeight, (x + 1) * m_CutWidth, (y + 1) * m_CutHeight );

			m_crop_infos[CurLayer].push_back(CROP_INFO(CurFrame, rect, cut));
			CurFrame += m_frames;
		}		
	}
}

bool Crop::Write(const std::wstring& strTag, const std::wstring& strLayerTag)
{
	light::XMLWriter generator;

	if( false == light::Directory::create_directory(light::get_path_without_file_name(m_strFile)) )
	{
		return false;
	}

	if(false == generator.open(m_strFile, strTag))
		return false;

	for each(const MAP_CROP_INFO::value_type& val in m_crop_infos)
	{
		int layer = val.first;	
		generator.into_with_attrib(strLayerTag);
		generator.attribute(L"Layer", layer);
		generator.attribute(L"TextureName", m_textures[0]); // 0으로 지정한 이유는 Cropper는 파일당 Texture를 한개만 지원하기 때문.

		for each( const CROP_INFO& info in val.second )
		{
			const light::RECT_INT& rect = info.Rect;
			const light::RECT_INT& cut = info.Cut;
			const view::Colors<float> color = info.Color;

			generator.child_elem(L"FrameInfo");
			generator.child_attribute(L"Frame", info.Frame);

			generator.child_attribute(L"CutLeft", cut.Left());
			generator.child_attribute(L"CutTop", cut.Top());
			generator.child_attribute(L"CutRight", cut.Right());
			generator.child_attribute(L"CutBottom", cut.Bottom());

			generator.child_attribute(L"Left", rect.Left());
			generator.child_attribute(L"Top", rect.Top());
			generator.child_attribute(L"Right", rect.Right());
			generator.child_attribute(L"Bottom", rect.Bottom());

			generator.child_attribute(L"R", (int)color.R());
			generator.child_attribute(L"G", (int)color.G());
			generator.child_attribute(L"B", (int)color.B());
			generator.child_attribute(L"A", (int)color.A());
			generator.child_attribute(L"Rotate", (int)info.Rotate);

			generator.child_elem_end();
		}
	}

	generator.out();
	return true;
}

bool Crop::WriteSpr()
{
	return Write(L"Sprite", L"SpriteInfo");
}

bool Crop::WriteAni()
{
	return Write(L"Animation", L"LayerInfo");
}

} // namespace view

} // namespace view
