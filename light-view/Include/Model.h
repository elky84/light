#pragma once


#include <d3dx9.h>
#include <d3dx9anim.h>
#include "AllocateHierarchy.h"
#include "weak_raw_ptr.hpp"

#define SHADOWFVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE //The FVF for the shadow vertex

namespace light
{

namespace view
{

class DirectX;

class Model  
{
public:
	Model(light::weak_raw_ptr<DirectX> directX);
	virtual ~Model();

	Model &operator=(const Model&)
	{
	}

	Model(const Model&)
	{
	}

	inline LPD3DXVECTOR3 GetBoundingSphereCenter()
	{ 
		return &m_vecCenter; 
	}

	inline float GetBoundingSphereRadius()
	{ 
		return m_fRadius; 
	}

	inline DWORD GetCurrentAnimation()	
	{
		return m_dwCurrentAnimation; 
	}

	bool SetCurrentAnimation(DWORD dwAnimationFlag);

	void Draw(const D3DXMATRIX& mat);

	bool Load(const std::wstring& file_name);

	bool Update(double dElapsedTime);

private:
	void DrawFrame(LPFRAME pFrame);

	void SetupBoneMatrices(LPFRAME pFrame, LPD3DXMATRIX pParentMatrix);

	void UpdateFrameMatrices(LPFRAME pFrame, LPD3DXMATRIX pParentMatrix);

private:
	//Model
	LPMESHCONTAINER		m_FirstMesh;			// The first mesh in the hierarchy
	LPD3DXFRAME			m_framesRoot;			// Frame hierarchy of the model
	LPD3DXMATRIX		m_BoneMatrices;		// Used when calculating the bone position
	D3DXVECTOR3			m_vecCenter;			// Center of bounding sphere of object
	float               m_fRadius;				// Radius of bounding sphere of object
	UINT				m_uMaxBones;			// The Max number of bones for the model

	//Animation
	DWORD				m_dwCurrentAnimation;	// The current animation
	DWORD				m_dwAnimationSetCount;	// Number of animation sets

	LPD3DXANIMATIONCONTROLLER   m_AnimController;// Controller for the animations

	/// DirectX
	light::weak_raw_ptr<DirectX> m_direct_x;
};
	 
} // namespace view

} // namespace view
