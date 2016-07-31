#include "light-view.h"
#include "FreeType2.h"
#include "DirectX.h"
#include "TextureManager.h"

#pragma comment(lib, "freetype.lib")

namespace light
{

namespace view
{

FreeType2::FreeType2(light::weak_raw_ptr<DirectX> directX, LPD3DXSPRITE& Sprite) 
: m_direct_x(directX)
, m_d3d_sprite(Sprite)
, m_Library(NULL)
, m_Face(NULL)
{

}

FreeType2::~FreeType2()
{

}

void FreeType2::Init()
{
}

void FreeType2::Release()
{
	m_FontCache.clear();
}

/**
@brief LoadFont
지정한 글꼴을 불러옵니다.
*/
bool FreeType2::LoadFont(const std::wstring& str)
{
    if (m_Library == NULL)
    {
        //프리타입을 초기화 시킨다.
        if (FT_Init_FreeType(&m_Library))
        {
            LOG_ERROR(L"%s, FT_Init_FreeType() Failed %s", __FUNCTIONW__, str.c_str());
            return false;
        }
    }

	// 경로에 해당하는 폰트파일을 불러온다.
	if(FT_New_Face(m_Library, WCHAR_TO_CHAR(str.c_str()), 0, &m_Face) == FT_Err_Unknown_File_Format)
	{
		return LoadFontWindowsDir(str);
	}
	else if (m_Face == NULL)
	{
		LOG_ERROR(L"%s, m_Face is NULL %s", __FUNCTIONW__, str.c_str());
		return false;
	}
	return true;
}

bool FreeType2::LoadFontWindowsDir(const std::wstring& str)
{
    WCHAR szPath[MAX_PATH] = L"";
    GetWindowsDirectoryW(szPath, MAX_PATH);
    std::wstring font_path = std::wstring(szPath) + L"/fonts/" + str;
    return LoadFont(font_path);
}

bool FreeType2::Render(const std::wstring& str, const light::POSITION_INT& Pos, const int Size, const DWORD dwColor)
{
	if( true == str.empty() )
		return false;

	m_d3d_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	light::POSITION_INT Position = Pos;

	for( size_t i = 0; i < str.size(); ++i)
	{
		Texture* pTexture = Get(str[i], Size);

		D3DXVECTOR3 vector = D3DXVECTOR3((float)Position.X(), (float)Position.Y(), 0);
		m_d3d_sprite->Draw(pTexture->GetTexture(), 0, 0, &vector, dwColor);

		Position.X(Position.X() + pTexture->GetWidth() + 1); // 1은 여백
	}

	m_d3d_sprite->End();
	return true;
}

bool FreeType2::Parsing(const std::wstring wstr, const light::RECT_INT& Rect, const int Size, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString)
{
	vectorWidth.resize(1);
	vectorString.resize(1);

	light::POSITION_INT Position;
	long line = 0;
	for( size_t i = 0; i < wstr.size(); ++i)
	{
		Texture* pTexture = Get(wstr[i], Size);

		long DrawingX = Position.X() + pTexture->GetWidth();
		if( Rect.Height() < Position.Y() + pTexture->GetHeight() )
			return false;

		if( wstr[i] == L'\n' )
		{
			NextLine(line, DrawingX, pTexture, Position, vectorWidth, vectorString);
		}
		else if( Rect.Width() <= DrawingX )
		{
			NextLine(line, DrawingX, pTexture, Position, vectorWidth, vectorString);
			if( Rect.Height() < Position.Y() + pTexture->GetHeight() )
				return false;

			NextCharacter(line, wstr[i], pTexture, Position, vectorWidth, vectorString);
		}
		else
		{
			NextCharacter(line, wstr[i], pTexture, Position, vectorWidth, vectorString);
		}
	}

	return true;
}

void FreeType2::NextLine(long& line, const long DrawingX, const Texture* pTexture, light::POSITION_INT& Position, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString)
{
	++line;
	vectorString.resize(line + 1);
	vectorWidth.resize(line + 1);
	vectorWidth[line] = DrawingX;

	Position.Y(Position.Y() + pTexture->GetHeight() + (INT)ceil(pTexture->GetHeight() * 0.2f));
	Position.X(0);

}

void FreeType2::NextCharacter(const long line, const WCHAR ch, const Texture* pTexture, light::POSITION_INT& Position, std::vector<long>& vectorWidth, std::vector<std::wstring>& vectorString)
{
	Position.X(Position.X() + pTexture->GetWidth());
	vectorString[line] += ch;
	vectorWidth[line] = Position.X();
}

bool FreeType2::Render(const std::wstring& text, const light::POSITION_INT& Pos, const light::RECT_INT& Rect, const int Size, const DWORD dwColor, const Align::Type align)
{
	if( true == text.empty() )
		return false;

	m_d3d_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	light::POSITION_INT Position = Pos;

	switch(align)
	{
	case Align::LEFT:
		{
			std::vector<long> vectorWidth;
			std::vector<std::wstring> vectorString;
			Parsing(text, Rect, Size, vectorWidth, vectorString);

			for each( auto& str in vectorString)
			{
				Position.X(Pos.X());
				for( size_t i = 0; i < str.size(); ++i)
				{
					Texture* pTexture = Get(str[i], Size);

					D3DXVECTOR3 vector = D3DXVECTOR3((float)Position.X(), (float)Position.Y(), 0);
					m_d3d_sprite->Draw(pTexture->GetTexture(), 0, 0, &vector, dwColor);

					Position.X(Position.X() + pTexture->GetWidth());

					if( str.size() -1 == i )
					{
						Position.Y(Position.Y() + pTexture->GetHeight() + (INT)ceil(pTexture->GetHeight() * 0.2f));
					}
				}
			}
		}
		break;
	case Align::CENTER:
		{
			std::vector<long> vectorWidth;
			std::vector<std::wstring> vectorString;
			Parsing(text, Rect, Size, vectorWidth, vectorString);

			long line = 0;
			
			Position = Pos;
			for each( auto& str in vectorString)
			{
				Position.X(Pos.X() + (Rect.Width() / 2 - vectorWidth[line++] / 2));
				for( size_t i = 0; i < str.size(); ++i)
				{
					Texture* pTexture = Get(str[i], Size);

					D3DXVECTOR3 vector = D3DXVECTOR3((float)Position.X(), (float)Position.Y(), 0);
					m_d3d_sprite->Draw(pTexture->GetTexture(), 0, 0, &vector, dwColor);

					Position.X(Position.X() + pTexture->GetWidth());

					if( str.size() -1 == i )
					{
						Position.Y(Position.Y() + pTexture->GetHeight() + (INT)ceil(pTexture->GetHeight() * 0.2f));
					}
				}
			}
		}
		break;
	case Align::RIGHT:
		{
			std::vector<long> vectorWidth;
			std::vector<std::wstring> vectorString;
			Parsing(text, Rect, Size, vectorWidth, vectorString);

			long line = 0;
			Position = Pos;
			for each( auto& str in vectorString)
			{
				Position.X(Pos.X() + Rect.Width() - vectorWidth[line++]);
				for( size_t i = 0; i < str.size(); ++i)
				{
					Texture* pTexture = Get(str[i], Size);

					D3DXVECTOR3 vector = D3DXVECTOR3((float)Position.X(), (float)Position.Y(), 0);
					m_d3d_sprite->Draw(pTexture->GetTexture(), 0, 0, &vector, dwColor);

					Position.X(Position.X() + pTexture->GetWidth());

					if( str.size() -1 == i )
					{
						Position.Y(Position.Y() + pTexture->GetHeight() + (INT)ceil(pTexture->GetHeight() * 0.2f));
					}
				}
			}
		}
		break;
	}

	m_d3d_sprite->End();
	return true;
}

Texture* FreeType2::Get(const WCHAR ch, int Size)
{
	MAP_FONTTEXTURE::iterator it = m_FontCache.find(KEY(ch, Size));
	if( it != m_FontCache.end())
	{
		return it->second;
	}

	Register(ch, Size);
	return Get(ch, Size);
}

void FreeType2::Register(const WCHAR ch, int Size)
{
	FT_GlyphSlot		slot = m_Face->glyph;
	FT_Bitmap			*pBitmap = &(slot->bitmap);
	FT_Glyph_Metrics	*pMetrics = 0;

	// 한글자가 들어갈 사이즈를 설정한다.
	FT_Set_Pixel_Sizes(m_Face, Size, Size);

	// 문자를 페이스로 불러들인다.
	FT_Load_Char(m_Face, ch, FT_LOAD_RENDER);

	// 비트맵과 매트릭스에 슬롯에 있는 데이터를 꺼내어 대입.
	pBitmap = &(slot->bitmap);
	pMetrics = &(slot->metrics);

	int nHeight = pMetrics->height >> 6;

	// 문자의 상단으로 부터의 높이를 구한다.
	int TextHeight = ((pMetrics->vertAdvance - pMetrics->horiBearingY) - (pMetrics->vertAdvance / 6)) >> 6;

	int CharHeight = nHeight + TextHeight + 1; // 1은 여백

	int CharWidth = (pMetrics->horiAdvance >> 6);

	// 얻어온 문자열의 가로,세로 크기로 텍스쳐를 생성한다.
	Texture* pTexture = m_direct_x->TextureMgr()->Create(CharWidth, CharHeight);
	if( pTexture == NULL)
	{
		LOG_ERROR(L"Failed create font texture!");
		return;
	}

	// 폰트를 그릴 위치를 텍스쳐로 잡는다.
	LPDIRECT3DSURFACE9	pSurface;
	if( FAILED(pTexture->GetTexture()->GetSurfaceLevel(0, &pSurface)))
	{
		LOG_ERROR(L"Failed create font surface!");
		return;
	}

	/// 실제 그리기
	D3DLOCKED_RECT FontTextureRect;
	pSurface->LockRect(&FontTextureRect, NULL, D3DLOCK_DISCARD);

	FT_Pos LineStartPositionX = 0;
	FT_Pos DrawingPositionX = LineStartPositionX; 
	FT_Pos DrawingPositionY = 0, BearingX = 0, BearingY = 0;

	BYTE *pDestData = (BYTE*)FontTextureRect.pBits;

	int nTextWidth = pMetrics->width >> 6;
	int nTextHeight = pMetrics->height >> 6;

	BearingX = pMetrics->horiBearingX >> 6;
	BearingY = ((pMetrics->vertAdvance - pMetrics->horiBearingY) - (pMetrics->vertAdvance / 6)) >> 6;

	for(int x = 0; x < nTextHeight; x++)
	{
		DWORD* pData = (DWORD*)(&pDestData[(DrawingPositionY + BearingY + x) * FontTextureRect.Pitch]);
		for(int y = 0; y < nTextWidth; y++)
		{
			if(pBitmap->buffer[x * pBitmap->width + y])
			{	  
				pData[DrawingPositionX + BearingX + y] = ((pBitmap->buffer[x * pBitmap->width + y]) << 24) + RGB(255,255,255);
			}
		}
	}

	// 락을 풀어준다.
	pSurface->UnlockRect();

	// STL맵에 문자열과 텍스쳐를 등록해 준다.
	m_FontCache.insert(MAP_FONTTEXTURE::value_type( KEY(ch, Size), pTexture));
}

bool FreeType2::InputCondition(const std::wstring& Message, const light::RECT_INT& Rect, const int Size)
{
	std::vector<long> vectorWidth;
	std::vector<std::wstring> vectorString;
	return Parsing(Message, Rect, Size, vectorWidth, vectorString);
}

} // namespace view

} // namespace view
