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

}


#endif // Engine_Struct_h__
