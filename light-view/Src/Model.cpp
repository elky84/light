#include "light-view.h"
#include "Model.h"

#include "DirectX.h"

namespace light
{

namespace view
{

Model::Model(light::weak_raw_ptr<DirectX> directX)
: m_direct_x(directX)
, m_framesRoot(NULL)		
, m_BoneMatrices(NULL)
, m_vecCenter(D3DXVECTOR3(0.0f,0.0f,0.0f))
, m_fRadius(0.0f)
, m_dwCurrentAnimation(0xFFFFFFFF)
, m_dwAnimationSetCount(0)
, m_uMaxBones(0)
, m_AnimController(NULL)
, m_FirstMesh(NULL)
{
}

Model::~Model()
{
	//Delete Animation Controller
	SAFE_RELEASE(m_AnimController)
	
	//if there is a frame hierarchyo
	if(m_framesRoot)
	{
		//Allocation class
		AllocateHierarchy Alloc;
		D3DXFrameDestroy(m_framesRoot, &Alloc);
		m_framesRoot = NULL;
	}

	//Delete the bones
	SAFE_DELETE_ARRAY(m_BoneMatrices)
}

bool Model::Load(const std::wstring& file_name)
{
	AllocateHierarchy Alloc;
	if( FAILED(D3DXLoadMeshHierarchyFromX(file_name.c_str(), D3DXMESH_MANAGED, m_direct_x->DxDevice(), &Alloc, NULL, &m_framesRoot, &m_AnimController)) )
	{
		LOG_ERROR(L"Model Load Error [%s]", file_name.c_str());
		return false;
	}

	if(m_AnimController == NULL)
	{
		LOG_ERROR(L"m_AnimController == NULL [%s]", file_name.c_str());
		return false;
	}

	m_dwAnimationSetCount = m_AnimController->GetMaxNumAnimationSets();

	if(m_framesRoot == NULL)
	{
		LOG_ERROR(L"m_framesRoot == NULL [%s]", file_name.c_str());
		return false;
	}

	//Set the bones up
	SetupBoneMatrices((LPFRAME)m_framesRoot, NULL);

	//Setup the bone matrices array 
	m_BoneMatrices  = new D3DXMATRIX[m_uMaxBones];
	ZeroMemory(m_BoneMatrices, sizeof(D3DXMATRIX)*m_uMaxBones);

	//Calculate the Bounding Sphere
	D3DXFrameCalculateBoundingSphere(m_framesRoot, &m_vecCenter, &m_fRadius);
	return true;
}

void Model::SetupBoneMatrices(LPFRAME pFrame, LPD3DXMATRIX pParentMatrix)
{
	LPMESHCONTAINER pMesh = (LPMESHCONTAINER)pFrame->pMeshContainer;

	//Set up the bones on the mesh
	if(pMesh)
	{
		if(!m_FirstMesh)
			m_FirstMesh = pMesh;
		
		// if there is a skinmesh, then setup the bone matrices
		if(pMesh->pSkinInfo)
		{
			//Create a copy of the mesh
			pMesh->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMesh->MeshData.pMesh->GetFVF(), m_direct_x->DxDevice(), &pMesh->pSkinMesh);

			if(m_uMaxBones < pMesh->pSkinInfo->GetNumBones())
			{
				//Get the number of bones
				m_uMaxBones = pMesh->pSkinInfo->GetNumBones();
			}

			LPFRAME pTempFrame = NULL;

			//For each bone 
			for (UINT i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++)
			{   
				// Find the frame
				pTempFrame = (LPFRAME)D3DXFrameFind(m_framesRoot, pMesh->pSkinInfo->GetBoneName(i));

				//set the bone part
				pMesh->ppFrameMatrices[i] = &pTempFrame->matCombined;
			}
		}
	}

	//Check your Sister
	if(pFrame->pFrameSibling)
		SetupBoneMatrices((LPFRAME)pFrame->pFrameSibling, pParentMatrix);

	//Check your Son
	if(pFrame->pFrameFirstChild)
		SetupBoneMatrices((LPFRAME)pFrame->pFrameFirstChild, &pFrame->matCombined);
}

bool Model::SetCurrentAnimation(DWORD dwAnimationFlag)
{
	// If the animation is not one that we are already using
	//	and the passed in flag is not bigger than the number of animations
	if(dwAnimationFlag != m_dwCurrentAnimation && dwAnimationFlag < m_dwAnimationSetCount) 
	{ 
		LPD3DXANIMATIONSET AnimSet = NULL;
		if(FAILED(m_AnimController->GetAnimationSet(dwAnimationFlag, &AnimSet)))
		{
			return false;
		}

		m_dwCurrentAnimation = dwAnimationFlag;
		m_AnimController->SetTrackAnimationSet(0, AnimSet);
		SAFE_RELEASE(AnimSet);
	}

	return true;
}

void Model::Draw(const D3DXMATRIX& mat)
{
	m_direct_x->DxDevice()->SetTransform(D3DTS_WORLD, &mat);

	LPMESHCONTAINER pMesh = m_FirstMesh;

	//While there is a mesh try to draw it
	while(pMesh)
	{
		//Select the mesh to draw
		LPD3DXMESH pDrawMesh = (pMesh->pSkinInfo) ? pMesh->pSkinMesh: pMesh->MeshData.pMesh;
		
		//Draw each mesh subset with correct materials and texture
		for (DWORD i = 0; i < pMesh->NumMaterials; ++i)
		{
			m_direct_x->DxDevice()->SetMaterial(&pMesh->pMaterials9[i]);
			m_direct_x->DxDevice()->SetTexture(0, pMesh->ppTextures[i]);
			pDrawMesh->DrawSubset(i);
		}

		//Go to the next one
		pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
	}
}

void Model::DrawFrame(LPFRAME pFrame)
{
	LPMESHCONTAINER pMesh = (LPMESHCONTAINER)pFrame->pMeshContainer;

	//While there is a mesh try to draw it
	while(pMesh)
	{
		//Select the mesh to draw
		LPD3DXMESH pDrawMesh = (pMesh->pSkinInfo) ? pMesh->pSkinMesh: pMesh->MeshData.pMesh;
		
		//Draw each mesh subset with correct materials and texture
		for (DWORD i = 0; i < pMesh->NumMaterials; ++i)
		{
			m_direct_x->DxDevice()->SetMaterial(&pMesh->pMaterials9[i]);
			m_direct_x->DxDevice()->SetTexture(0, pMesh->ppTextures[i]);
			pDrawMesh->DrawSubset(i);
		}

		//Go to the next one
		pMesh = (LPMESHCONTAINER)pMesh->pNextMeshContainer;
	}

	//Check your Sister
	if(pFrame->pFrameSibling)
		DrawFrame((LPFRAME)pFrame->pFrameSibling);

	//Check your Son
	if(pFrame->pFrameFirstChild)
		DrawFrame((LPFRAME)pFrame->pFrameFirstChild);
}

bool Model::Update(double dElapsedTime)
{
	//Set the time for animation
	if( m_AnimController && m_dwCurrentAnimation != -1 )
	{
		if(FAILED(m_AnimController->AdvanceTime(dElapsedTime, NULL)))
		{
			return false;
		}
	}

	//Update the frame hierarchy
	if(m_framesRoot == NULL)
		return false;

	UpdateFrameMatrices((LPFRAME)m_framesRoot, NULL);

	LPMESHCONTAINER pMesh = m_FirstMesh;
	if(pMesh)
	{
		if(pMesh->pSkinInfo)
		{
			UINT Bones = pMesh->pSkinInfo->GetNumBones();
			for (UINT i = 0; i < Bones; ++i)
			{	
				D3DXMatrixMultiply
				(
					&m_BoneMatrices[i],//out
					&pMesh->pBoneOffsets[i], 
					pMesh->ppFrameMatrices[i]
				);
			}

			// Lock the meshes' vertex buffers
			void *SrcPtr, *DestPtr;
			pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&SrcPtr);
			pMesh->pSkinMesh->LockVertexBuffer(0, (void**)&DestPtr);

			// Update the skinned mesh using provided transformations
			pMesh->pSkinInfo->UpdateSkinnedMesh(m_BoneMatrices, NULL, SrcPtr, DestPtr);

			// Unlock the meshes vertex buffers
			pMesh->pSkinMesh->UnlockVertexBuffer();
			pMesh->MeshData.pMesh->UnlockVertexBuffer();
		}
	}
	return true;
}

void Model::UpdateFrameMatrices(LPFRAME pFrame, LPD3DXMATRIX pParentMatrix)
{	
	//Parent check
	if (pParentMatrix)
	{
		D3DXMatrixMultiply(&pFrame->matCombined, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->matCombined = pFrame->TransformationMatrix;
	}

	//Do the kid too
	if (pFrame->pFrameSibling)
	{
		UpdateFrameMatrices((LPFRAME)pFrame->pFrameSibling, pParentMatrix);
	}

	//make sure you get the first kid
	if (pFrame->pFrameFirstChild)
	{
		UpdateFrameMatrices((LPFRAME)pFrame->pFrameFirstChild, &pFrame->matCombined);
	}
}

} // namespace view

} // namespace view
