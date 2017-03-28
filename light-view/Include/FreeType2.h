#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>

#include <map>
#include <vector>

#include "Rectangle.h"
#include "Position.h"

#include "weak_raw_ptr.hpp"

namespace light
{

namespace view
{

class Texture;
class DirectX;

class FreeType2
{
public:
	struct Align
	{
		enum Type
		{
			NONE,
			LEFT,
			CENTER,
			RIGHT,
		};
	};

private:
	typedef std::pair<WCHAR, int> KEY;

	typedef std::map<KEY, Texture* > MAP_FONTTEXTURE;

public:
	FreeType2(light::weak_raw_ptr<DirectX> directX, LPD3DXSPRITE& Sprite);
	~FreeType2();

	void Init();
	
	bool LoadFont(const std::wstring& str);

    bool LoadFontWindowsDir(const std::wstring& str);
	
	bool Render(const std::wstring& str, const light::POSITION_INT& Pos, const int Size, const DWORD dwColor);

	bool Render(const std::wstring& str, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int Size, const DWORD dwColor, const Align::Type align = Align::LEFT);

	void Register(const WCHAR ch, int Size);

	Texture* Get(const WCHAR ch, int Size);

	void Release();

	bool InputCondition(const std::wstring& Message, const light::RECT_INT& Rect, const int Size);

private:
	bool Parsing(const std::wstring wstr, const light::RECT_INT& Rect, const int Size, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString);

	void NextLine(long& line, const long DrawingX, const Texture* pTexture, light::POSITION_INT& Position, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString);

	void NextCharacter(const long line, const WCHAR ch, const Texture* pTexture, light::POSITION_INT& Position, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString);

private:
	FT_Library m_Library;

	FT_Face m_Face;

	LPD3DXSPRITE& m_d3d_sprite;

	MAP_FONTTEXTURE	m_FontCache;

	light::weak_raw_ptr<DirectX> m_direct_x;
};

} // namespace view

} // namespace view
