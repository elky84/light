#include "light-view.h"
#include "TextureManager.h"
#include "DirectX.h"

#include "Profiler.h"
#include "DirectoryUtil.h"

#pragma warning ( push )
#pragma warning(disable:4100) 
#include <boost/thread.hpp>
#pragma warning ( pop )

namespace light
{

namespace view
{

Texture::Texture(LPDIRECT3DTEXTURE9 pTexture, int nWIdth, int nHeight, DWORD dwColorKey)
	: m_texture(pTexture)
	, m_dwColorKey(dwColorKey)
	, m_width(nWIdth)
	, m_height(nHeight)
{
}

Texture::Texture(LPDIRECT3DTEXTURE9 pTexture, int nWIdth, int nHeight) 
	: m_texture(pTexture) 
	, m_width(nWIdth)
	, m_height(nHeight)
{
}

Texture::~Texture()
{
}

int Texture::GetWidth() const 
{
	return m_width;
}

int Texture::GetHeight() const 
{
	return m_height;
}

void Texture::Release()
{
	SAFE_RELEASE(m_texture);
}

void Texture::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_texture = pTexture;
}

LPDIRECT3DTEXTURE9 Texture::GetTexture() const 
{
	return m_texture;
}

DWORD Texture::GetColorKey()
{
	return m_dwColorKey;
}

bool Texture::operator<(const Texture& texture) const
{
	return &texture < this;
}

TextureManager::TextureManager(light::weak_raw_ptr<DirectX> directX)
	: m_direct_x(directX)
    , m_thread(boost::bind(&TextureManager::on_update, this), boost::bind(&TextureManager::begin, this))
{
	boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
	std::pair<SET_TEXTURE::iterator, bool> pair_result = m_set_Texture.insert(Texture(NULL, Texture::DEFAULT_WIDTH, Texture::DEFAULT_HEIGHT));
	m_NullTexture = &(*pair_result.first);
}

TextureManager::~TextureManager()
{
	ReleaseAll();

	{
		boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
		m_hashmap_Texture.clear();
	}
}

Texture* TextureManager::Create(int nWidth, int nHeight)
{
	LPDIRECT3DTEXTURE9 pTexture = m_direct_x->CreateTexture(nWidth, nHeight);
	if(pTexture == NULL)
		return NULL;

	{
		boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
		std::pair<SET_TEXTURE::iterator, bool> pair_result = m_set_Texture.insert(Texture(pTexture, nWidth, nHeight));
		return const_cast<Texture*>(&(*pair_result.first));
	}
}

Texture* TextureManager::Load(const std::wstring& strName, DWORD dwColorKey)
{
	Texture* pTexture = Get(strName);
	if(pTexture)
	{
		return pTexture;
	}

	light::ScopeProfiler profiler(__FUNCTIONW__, light::Profiler::DEFAULT_BASELINE, L"%s", strName.c_str());

	D3DXIMAGE_INFO imageinfo;
	LPDIRECT3DTEXTURE9 lpTexture = m_direct_x->LoadTexture(strName, dwColorKey, &imageinfo);
	if( lpTexture == NULL )
	{
		/// NULL 객체를 반환한다.
		LOG_INFO(L"%s [%s] Not Find. Return NullTexture", __FUNCTIONW__, strName.c_str());
		return NullTexture();
	}

	{
		boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
		if( m_hashmap_Texture.find(strName) == m_hashmap_Texture.end() )
			m_hashmap_Texture.insert(HASHMAP_TEXTURE::value_type(strName, Texture(lpTexture, imageinfo.Width, imageinfo.Height, dwColorKey)));
	}
	return Get(strName);
}

bool TextureManager::Free(const std::wstring& strName)
{
	Texture* pTexture = Get(strName);
	if(pTexture == NULL)
		return false;

	pTexture->Release();
	return true;
}

void TextureManager::AsyncLoad()
{
    m_thread.start(); // 비동기 스레드 시작
	LOG_INFO(L"%s", __FUNCTIONW__);
}

void TextureManager::begin()
{
	light::FileUtil::search(m_list_FileName, light::Directory::get_current_directory(), L"*.png");
	LOG_INFO(L"%s. Count[%u]", __FUNCTIONW__, m_list_FileName.size());
}

void TextureManager::on_update()
{
	if( true == m_list_FileName.empty() )
	{
        m_thread.stop();
		return;
	}

	auto val = m_list_FileName.front();
	if ( m_NullTexture.get() != Load(val))
	{
		m_list_FileName.pop_front();
	}
}

Texture* TextureManager::NullTexture()
{
	return m_NullTexture.get();
}

Texture* TextureManager::Get(const std::wstring& strName)
{
	boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
	HASHMAP_TEXTURE::iterator it = m_hashmap_Texture.find(strName);
	if(it == m_hashmap_Texture.end())
		return NULL;

	return &it->second;
}

void TextureManager::ReleaseAll()
{
	boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
	for(HASHMAP_TEXTURE::iterator it = m_hashmap_Texture.begin(); it != m_hashmap_Texture.end(); ++it)
	{
		Texture& texture = it->second;;
		texture.Release();
	}

	for(SET_TEXTURE::iterator it = m_set_Texture.begin(); it != m_set_Texture.end(); ++it)
	{
		Texture& texture = const_cast<Texture&>(*it);
		texture.Release();
	}
}

void TextureManager::Reset()
{
	boost::lock_guard<boost::recursive_mutex> guard(m_mutex);
	for(HASHMAP_TEXTURE::iterator it = m_hashmap_Texture.begin(); it != m_hashmap_Texture.end(); ++it)
	{
		Texture& texture = it->second;;
		texture.Release();
		texture.SetTexture(m_direct_x->LoadTexture(it->first, texture.GetColorKey()));
	}

	for(SET_TEXTURE::iterator it = m_set_Texture.begin(); it != m_set_Texture.end(); ++it)
	{
		Texture& texture = const_cast<Texture&>(*it);
		texture.Release();
		texture.SetTexture(m_direct_x->CreateTexture(texture.GetWidth(), texture.GetHeight()));
	}

	m_NullTexture = Create(Texture::DEFAULT_WIDTH, Texture::DEFAULT_HEIGHT);
}

void TextureManager::stop()
{
    m_thread.stop();
}

} // namespace view

} // namespace view
