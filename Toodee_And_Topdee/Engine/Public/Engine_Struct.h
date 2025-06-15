#ifndef Engine_Struct_h__
#define Engine_Struct_h__



namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND			hWnd;
		WINMODE			eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;
		unsigned int	iNumLevels;
	}ENGINE_DESC;

	typedef struct tagVertexPosTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;
	}VTXPOSTEX;

	typedef struct tagVertexPosCubeTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexcoord;
	}VTXCUBETEX;

	typedef struct tagVertexNorTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexcoord;
	}VTXNORTEX;

	typedef struct tagMapEdit
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vScale;
		unsigned int	iBlockType;
		unsigned int	iTextureIdx;
		unsigned int	iDir;
		unsigned int	iTileTextureIdx;
	}BLOCK_INFO;

	typedef struct tagVertexDiffuse
	{
		D3DXVECTOR3 vPosition;
		D3DCOLOR    dwColor;
	}VTXCUBEDIFFUSE;

}


#endif // Engine_Struct_h__
