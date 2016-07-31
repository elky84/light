#include "light-view.h"
#include "Polygon2D.h"
#include "DirectX.h"
#include "TextureManager.h"

namespace light
{

namespace view
{

Polygon2D::Polygon2D()
: m_width(0)
, m_height(0)
{
}


/**
@brief 생성자
*/
Polygon2D::Polygon2D(light::weak_raw_ptr<DirectX> directX)
: m_direct_x(directX)
, m_width(0)
, m_height(0)
{
}

/**
@brief 복사 생성자
@param polygon 폴리곤 객체
*/
Polygon2D::Polygon2D(const Polygon2D& polygon)
: m_width(polygon.m_width)
, m_height(polygon.m_height)
, m_texture(polygon.m_texture)
, m_direct_x(polygon.m_direct_x)
{
	for(int i = 0; i < _countof(m_vertex_data); i++)
		m_vertex_data[i] = polygon.m_vertex_data[i];
}

/**
@brief 소멸자
*/
Polygon2D::~Polygon2D()
{
	Release();
}


/**
@brief 대입 연산자.
*/
const Polygon2D& Polygon2D::operator = (const Polygon2D& polygon)
{
	UNREFERENCED_PARAMETER(polygon);
	return *this;
}

/**
@brief 텍스쳐 로드 함수. 
@param strName 텍스쳐 파일 명
@param DWORD dwColorKey 컬러키
*/
bool Polygon2D::LoadTexture(const std::wstring& strName, DWORD dwColorKey)
{
	if ( true == strName.empty() )
	{
		//Just create with out texture.
		m_width = 32;
		m_height = 32;
	}
	else
	{
		
		m_texture = m_direct_x->TextureMgr()->Load(strName, dwColorKey);
		if( NULL == m_texture.get())
		{
			LOG_ERROR(L"%s [%s]", __FUNCTIONW__, strName.c_str());
			return false;
		}

		if( m_texture.get() == m_direct_x->TextureMgr()->NullTexture() )
		{
			LOG_ERROR(L"%s [%s] Not Load. this is NullTexture.", __FUNCTIONW__, strName.c_str());
		}

		// Getting texture size information.3
		m_width = m_texture->GetWidth();
		m_height = m_texture->GetHeight();
	}

	reset();
	return true;
}


void Polygon2D::SetDirectX(light::weak_raw_ptr<DirectX> directX)
{
	m_direct_x = directX;
}

/**
@brief 초기 값 리셋 메소드
*/
void Polygon2D::reset()
{
	for ( int i = 0;i < _countof(m_vertex_data);i++ )
	{
		m_vertex_data[i].color = D3DCOLOR_RGBA(255,255,255,255);
	}

	// Vertex A1 puts position data.
	m_vertex_data[0].x	= 0;
	m_vertex_data[0].y	= 0;
	m_vertex_data[0].z	= 0.5f;
	m_vertex_data[0].rhw	= 1.0f;
	m_vertex_data[0].tu	= 0.0f;
	m_vertex_data[0].tv	= 0.0f;
	// Vertex A2 puts position data.
	m_vertex_data[1].x	= (float)m_texture->GetWidth();
	m_vertex_data[1].y	= 0;
	m_vertex_data[1].z	= 0.5f;
	m_vertex_data[1].rhw	= 1.0f;
	m_vertex_data[1].tu	= 1.0f;
	m_vertex_data[1].tv	= 0.0f;
	// Vertex A3 puts position data.
	m_vertex_data[2].x	= (float)m_texture->GetWidth();
	m_vertex_data[2].y	= (float)m_texture->GetHeight();
	m_vertex_data[2].z	= 0.5f;
	m_vertex_data[2].rhw	= 1.0f;
	m_vertex_data[2].tu	= 1.0f;
	m_vertex_data[2].tv	= 1.0f;
	// Vertex A4 puts position data.
	m_vertex_data[3].x	= 0;
	m_vertex_data[3].y	= (float)m_texture->GetHeight();
	m_vertex_data[3].z	= 0.5f;
	m_vertex_data[3].rhw	= 1.0f;
	m_vertex_data[3].tu	= 0.0f;
	m_vertex_data[3].tv	= 1.0f;
}

/**
@brief 화면에 텍스쳐를 그려주는 메소드. 언제나 변환이 필요하다. you can do everything transform!
@param Rect Rect
@param Uv 텍스쳐 내의 UV 좌표
@param Mirror 좌우 반전 여부
@param fRot 회전 값
*/
void Polygon2D::Blt(const light::RECT_FLOAT& Rect, const light::RECT_FLOAT& Uv, const bool Mirror, float fRot /* = 0.0f */)
{	
	light::RECT_FLOAT fTempRt;

	// 픽셀계열에서 퍼센테이지 계열로 변환
	fTempRt.Left((float)(1*Uv.Left()/m_width));
	fTempRt.Top((float)(1*Uv.Top()/m_height));
	fTempRt.Right((float)(1*Uv.Right()/m_width));
	fTempRt.Bottom((float)(1*Uv.Bottom()/m_height));

	// 중심축을 구한다	
	float wid = Rect.Right() / 2.0f;
	float hei = Rect.Bottom() / 2.0f;
	if (fRot != 0.0f)
	{
		light::POSITION_FLOAT fRadian[4];
		for(int i = 0; i < 4; i++)
		{
			static const float fnumX[4] = {-1.0f, 1.0f, 1.0f, -1.0f};
			static const float fnumY[4] = {-1.0f, -1.0f, 1.0f, 1.0f};

			double foundangle = 0;
			// 정점의 위치를 취득 --, +-, ++, -+ 순으로
			fRadian[i].X(Rect.Left() + (wid * fnumX[i]));					
			fRadian[i].Y(Rect.Top() + (hei * fnumY[i]));

			// atan 값을 맞춰주기 위해 위치에 따라서 각도조정
			if(fRadian[i].Y() < Rect.Top())	
				foundangle = 180.0f;				
			else							
				foundangle = 0.0f;		

			// 위의 값을 보면 Rectition이 10, 10으로 초기값이 되어 있다 
			// 그 Rectition의 중심축을 (0, 0)으로 맞추기 위해
			fRadian[i] -= Rect.LeftTop();											

			// 거리 계산
			float v = (float)sqrt(fRadian[i].X() * fRadian[i].X() + fRadian[i].Y() * fRadian[i].Y());

			// 각도 계산
			double temp_angle = (float)atan((float)fRadian[i].X() / fRadian[i].Y()) * 180.0f / 3.14159;

			// Radian의 계산 
			float r = (float)(((temp_angle - fRot + foundangle)  / 180.0f) * 3.14159);

			// 라디안 값을 통해서 좌표를 취득
			fRadian[i].X(v * sin(r));
			fRadian[i].Y(v * cos(r));

			// 본래의 위치를 맞춰 주기 위해 본래의 좌표 값과 폭 높이를 더한다.
			fRadian[i].X( fRadian[i].X() + Rect.Left() + wid);
			fRadian[i].Y( fRadian[i].Y() + Rect.Top() + hei);
		}

		m_vertex_data[0].x = fRadian[0].X() + 0.5f;
		m_vertex_data[0].y = fRadian[0].Y() + 0.5f;
		m_vertex_data[1].x = fRadian[1].X();
		m_vertex_data[1].y = fRadian[1].Y();
		m_vertex_data[2].x = fRadian[2].X();
		m_vertex_data[2].y = fRadian[2].Y();
		m_vertex_data[3].x = fRadian[3].X();
		m_vertex_data[3].y = fRadian[3].Y();
	}
	else
	{
		m_vertex_data[0].x =	Rect.Left() - 0.5f;
		m_vertex_data[0].y =	Rect.Top() - 0.5f;
		m_vertex_data[1].x =	m_vertex_data[0].x + Rect.Right();
		m_vertex_data[1].y =	m_vertex_data[0].y;
		m_vertex_data[2].x =	m_vertex_data[0].x + Rect.Right();
		m_vertex_data[2].y =	m_vertex_data[0].y + Rect.Bottom();
		m_vertex_data[3].x =	m_vertex_data[0].x;
		m_vertex_data[3].y =	m_vertex_data[0].y + Rect.Bottom();
	}

	if(Mirror)
	{
		m_vertex_data[0].tu = fTempRt.Right();
		m_vertex_data[0].tv = fTempRt.Top();
		m_vertex_data[1].tu = fTempRt.Left();
		m_vertex_data[1].tv = fTempRt.Top();
		m_vertex_data[2].tu = fTempRt.Left();
		m_vertex_data[2].tv = fTempRt.Bottom();
		m_vertex_data[3].tu = fTempRt.Right();
		m_vertex_data[3].tv = fTempRt.Bottom();
	}
	else
	{
		m_vertex_data[0].tu = fTempRt.Left();
		m_vertex_data[0].tv = fTempRt.Top();
		m_vertex_data[1].tu = fTempRt.Right();
		m_vertex_data[1].tv = fTempRt.Top();
		m_vertex_data[2].tu = fTempRt.Right();
		m_vertex_data[2].tv = fTempRt.Bottom();
		m_vertex_data[3].tu = fTempRt.Left();
		m_vertex_data[3].tv = fTempRt.Bottom();
	}

	if ( fRot != 0.0f || 
		(Rect.Right() != m_width || Rect.Bottom() != m_height) || 
		(Uv.Right() != m_width || Uv.Bottom() != m_height) ) // 사이즈가 바꼈으면
	{
		DrawTexture(true);
	}
	else
	{
		DrawTexture(false);
	}
}

/**
@brief 화면에 그려주는 메소드
@param bBlur 블러 메소드
*/
void Polygon2D::DrawTexture(bool bBlur /* = false */)
{
	/// 알파 블렌드 켬
	m_direct_x->DxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true );

	// turn on rander when use the screen effect.
	m_direct_x->DxDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_direct_x->DxDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// in case of broken texture should be turn on the blur effect.
	if ( false == bBlur )
	{
		m_direct_x->DxDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		m_direct_x->DxDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	}
	else
	{
		m_direct_x->DxDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	// Set this texture to D3Ddev.
	m_direct_x->DxDevice()->SetTexture(0, m_texture->GetTexture());

	// Draw up the screen. DrawPrimitiveUP() - 정점 버퍼를 사용하지 않고 정점 Rendering  
	m_direct_x->DxDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_vertex_data, sizeof(Vertex2D));

	m_direct_x->DxDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

/**
@brief 텍스쳐의 알파와 RGB 컬러를 설정한다.
*/
void Polygon2D::SetTransformColor(const COLORS_FLOAT& color)
{
	for (int i = 0;i < _countof(m_vertex_data); i++)
	{
		m_vertex_data[i].color = color.RGBA();
	}
}

/**
@brief Clean-up all polygon and texture data.
*/
void Polygon2D::Release()
{
	ReleaseTexture();
}

/**
@brief Clean-up all Texture data.
*/
void Polygon2D::ReleaseTexture()
{
	m_texture.release();
}

} // namespace view

} // namespace view
