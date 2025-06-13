
#pragma once

#include "Base.h"


BEGIN(Engine)

BEGIN(Picking)

struct ENGINE_DLL Ray
{
	_float3 origin;
	_float3 dir;
};

ENGINE_DLL Ray GetRayFromMouse(HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device);
ENGINE_DLL HRESULT RayIntersectsAABB(const Ray& ray, const _float3& min, const _float3& max, float* pDist = nullptr);
ENGINE_DLL HRESULT RayIntersectsTriangle(const Ray& ray, const _float3& v0, const _float3& v1, const _float3& v2, _float* pDist = nullptr);

END

BEGIN(d3d)
inline DWORD FtoDw(float f)
{
    return *reinterpret_cast<DWORD*>(&f);
}

inline float GetRandomFloat(float lowBound, float hightBound)
{
	if (lowBound >= hightBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;
	return (f * (hightBound - lowBound)) + lowBound;
}

inline void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}
END

END
