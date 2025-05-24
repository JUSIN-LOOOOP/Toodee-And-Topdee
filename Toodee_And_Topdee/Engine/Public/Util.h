
#pragma once

#include "Base.h"


BEGIN(Engine)

BEGIN(Picking)

struct ENGINE_DLL Ray
{
	_float3 origin;
	_float3 dir;
};

ENGINE_DLL Ray GetRayFromMouse(const unsigned int WinX, const unsigned int WinY, HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device);
ENGINE_DLL HRESULT RayIntersectsAABB(const Ray& ray, const _float3& min, const _float3& max, _float* pDist = nullptr);
ENGINE_DLL HRESULT RayIntersectsTriangle(const Ray& ray, const _float3& v0, const _float3& v1, const _float3& v2, _float* pDist = nullptr);

END
END
