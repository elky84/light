#pragma once

//Derived from the default mesh container
typedef struct _D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	//Mesh variables
	LPDIRECT3DTEXTURE9*  ppTextures;		// Textures of the mesh
	D3DMATERIAL9*		 pMaterials9;		// Use the DirectX 9 Material type

	//Skinned mesh variables
	LPD3DXMESH           pSkinMesh;			// The skin mesh
	LPD3DXMATRIX		 pBoneOffsets;		// The bone matrix Offsets
	LPD3DXMATRIX*		 ppFrameMatrices;	// Pointer to the Frame Matrix

	// Attribute table stuff
	LPD3DXATTRIBUTERANGE pAttributeTable;	// The attribute table
	DWORD                NumAttributeGroups;// The number of attribute groups

}MESHCONTAINER, *LPMESHCONTAINER;

//Derived frame struct so it looks like a heirarchy
typedef struct _D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIX matCombined;	//Combined Transformation Matrix

}FRAME, *LPFRAME;

namespace light
{

namespace view
{

class AllocateHierarchy: public ID3DXAllocateHierarchy
{
public:

	// Create a frame
    //1. The name of the frame
	//2. The output new frame
	STDMETHOD(CreateFrame)(THIS_ 
		LPCSTR Name, LPD3DXFRAME *ppNewFrame);
    
	// Create a Mesh Container
	//1. Name of the Mesh
	//2. The mesh Data
	//3. that materials of the mesh
	//4. the effects on the mesh
	//5. the number of meterials in the mesh
	//6. the adjacency array for the mesh
	//7. the skin information for the mesh
	//8. the output mesh container
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
    
	// Destroy a frame
	//1. The frame to delete
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    
	// Destroy a mesh container
	//1. The container to destroy
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};

} // namespace view

} // namespace view
