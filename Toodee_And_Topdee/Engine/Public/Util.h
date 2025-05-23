
#pragma once

#include "Base.h"


BEGIN(Engine)

namespace Picking
{
	struct ENGINE_DLL Ray
	{
		D3DXVECTOR3 origin;
		D3DXVECTOR3 dir;
	};

	ENGINE_DLL Ray GetRayFromMouse(const unsigned int WinX, const unsigned int WinY, HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device);
	ENGINE_DLL HRESULT RayIntersectsAABB(const Ray& ray, const D3DXVECTOR3& min, const D3DXVECTOR3& max, float* pDist = nullptr);
	ENGINE_DLL HRESULT RayIntersectsTriangle(const Ray& ray, const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, float* pDist = nullptr);
};

END
