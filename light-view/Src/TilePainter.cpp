#include "light-view.h"
#include "TilePainter.h"

#include "XMLParser.h"
#include "DirectoryUtil.h"

namespace light
{

namespace view
{

TilePainter::TilePainter(const light::RECT_INT& Rect)
	: m_rect(Rect)
	, m_width(0)
	, m_height(0)
{
}

TilePainter::~TilePainter()
{
}

bool TilePainter::Load(light::weak_raw_ptr<DirectX> directX, const std::wstring& MapFile)
{
	light::XMLParser parser;
	if ( false == parser.read_file(MapFile) ) 
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	if ( false == parser.execute( L"/Map" ) ) 
	{
		LOG_ERROR(L"%s. %s execute failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	if ( false == parser.bind_elem( L"MapInfo" ) ) 
	{
		LOG_ERROR(L"%s. %s bind_elem failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	if ( false == parser.bind_attrib(L"Width", m_width) ) 
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	if ( false == parser.bind_attrib(L"Height", m_height) )
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	std::wstring strSprite;
	if ( false == parser.bind_attrib(L"SpriteName", strSprite) )
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	if ( false == parser.next() )
	{
		LOG_ERROR(L"%s. %s next() failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	const std::wstring strPath = light::get_path_without_file_name(MapFile);

	if ( false == m_sprite.Load(directX, strPath + strSprite) )
	{
		LOG_ERROR(L"%s. %s file open failed.", __FUNCTIONW__, strSprite.c_str());
		return false;
	}

	m_Tile.reset(new Tile(m_sprite));

	if ( false == parser.bind_elem(L"Line") ) 
	{
		LOG_ERROR(L"%s. %s bind_elem failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	std::wstring strProperty;
	if ( false == parser.bind_attrib(L"Property", strProperty ) )
	{
		LOG_ERROR(L"%s. %s bind_attrib failed.", __FUNCTIONW__, MapFile.c_str());
		return false;
	}

	while(parser.next())
	{
		light::str::VECTOR_STRING vectorRow = light::str::split(strProperty, L"|");
		for each(auto& strLine in vectorRow)
		{
			light::str::VECTOR_STRING vectorStr = light::str::split(strLine, L",");
			if( vectorStr.size() != 4 )
			{
				LOG_ERROR(L"%s. %s Invalid Property count. Property[%s] Count[%d]", __FUNCTIONW__, MapFile.c_str(), strProperty.c_str(), vectorStr.size());
				return false;
			}

			TileProperty property;
			property.X = _wtoi(vectorStr[0].c_str());
			property.Y = _wtoi(vectorStr[1].c_str());
			property.Type = _wtoi(vectorStr[2].c_str());
			property.Value = _wtoi(vectorStr[3].c_str());

			m_vectorTileProperty.push_back(property);
		}
	}

	return true;
}

void TilePainter::Render()
{
	for(int x = 0; x < m_width; x++)
	{
		for(int y = 0; y < m_height; y++)
		{
			if( CheckDraw(x * m_Tile->Width(), y * m_Tile->Height()) )
				continue;

			light::POSITION_INT position(m_rect.Left() + x* m_Tile->Width(), m_rect.Top() + y * m_Tile->Height());
			AdjustPosition(position);

			const TileProperty& property = m_vectorTileProperty[m_height * y + x];
			m_Tile->Draw((Layer2D::NO)property.X, position, property.Y);
		}
	}
}

bool TilePainter::CheckDraw(const int X, const int Y)
{
	return m_rect.Width() < X || m_rect.Height() < Y;
}

void TilePainter::AdjustPosition(light::POSITION_INT& Position)
{
	UNREFERENCED_PARAMETER(Position);
}

TileProperty& TilePainter::Property(const int X, const int Y)
{
	return m_vectorTileProperty[m_height * Y + X];
}

TilePainterScroll::TilePainterScroll(const int& ScrollX, const int& ScrollY, const light::RECT_INT& Rect)
	: TilePainter(Rect)
	, m_ScrollX(ScrollX)
	, m_ScrollY(ScrollY)
{
}

bool TilePainterScroll::CheckDraw(const int X, const int Y)
{
	return m_rect.Width() < X - m_ScrollX || m_rect.Height() < Y - m_ScrollY;
}

void TilePainterScroll::AdjustPosition(light::POSITION_INT& Position)
{
	Position.X(Position.X() - m_ScrollX);
	Position.Y(Position.Y() - m_ScrollY);
}

} // namespace view

} // namespace view
