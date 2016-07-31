#pragma once

#include <unordered_map>
#include <set>

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "weak_raw_ptr.hpp"
#include "Thread.h"

#include "FileUtil.h"

namespace light
{

namespace view
{

class DirectX;

class Texture
{
public:
	static const int DEFAULT_WIDTH = 128;
	static const int DEFAULT_HEIGHT = 128;

public:
	Texture(LPDIRECT3DTEXTURE9 pTexture, int nWIdth, int nHeight, DWORD dwColorKey);

	Texture(LPDIRECT3DTEXTURE9 pTexture, int nWIdth, int nHeight);

	~Texture();

	int GetWidth() const;

	int GetHeight() const;

	void Release();

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);

	LPDIRECT3DTEXTURE9 GetTexture() const;

	DWORD GetColorKey();

	bool operator<(const Texture& texture) const;

private:
	LPDIRECT3DTEXTURE9	m_texture;

	DWORD m_dwColorKey;

	int m_width;

	int m_height;
};

class TextureManager
{
private:
	typedef std::unordered_map<std::wstring, Texture> HASHMAP_TEXTURE;
	typedef std::set<Texture> SET_TEXTURE;

public:
	TextureManager(light::weak_raw_ptr<DirectX> directX);
	virtual ~TextureManager();

	Texture* Create(int nWidth, int nHeight);

	Texture* Get(const std::wstring& strName);
	Texture* Load(const std::wstring& strName, DWORD dwColorKey = D3DCOLOR_XRGB(255, 0, 255));

	bool Free(const std::wstring& strName);

	void ReleaseAll(); 
	void Reset();

	Texture* NullTexture();

	void AsyncLoad();

    void stop();

protected:
	virtual void begin();

	///반복적으로 불려지는 메소드. 상속 받은 클래스에서 재정의해서 사용해야함.
	virtual void on_update();

private:
	/// 텍스쳐의 해쉬 맵
	HASHMAP_TEXTURE m_hashmap_Texture;

	// 텍스쳐의 셋
	SET_TEXTURE m_set_Texture;

	// Texture의 NULL 오브젝트
	light::weak_raw_ptr<Texture> m_NullTexture; 

	light::weak_raw_ptr<DirectX> m_direct_x;

	light::FileUtil::LIST_FILENAME m_list_FileName;

	boost::recursive_mutex m_mutex;

    Thread m_thread;
};

} // namespace view

} // namespace view
