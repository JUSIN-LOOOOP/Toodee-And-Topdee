#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollision  final: public CBase
{
public:
	typedef struct ColliderTransformInfo {
		_float3 center;
		_float3 axis[3]; // 정규화된 x,y,z 방향 벡터
		_float3 halfSize;

	}OBB;

private:
	CCollision();
	virtual ~CCollision() = default;

public:
	static _bool			Collision_Rect_Rect(const OBB& RectA, const OBB& RectB);
	static _bool			Collision_Rect_Cube(const OBB& RectA, const OBB& CubeB);
	static _bool			Collision_Cube_Cube(const OBB& CubeA, const OBB& CubeB);

	static void				GetProjection(const size_t& iLength, const _float3* vEdge, const _float3& vAxis, _float& fMin, _float& fMax);
	static _bool			OverlapOnAxis(const size_t& iLength, const _float3* vEdgeA, const _float3* vEdgeB, const _float3& vAxis);

public:
	static const _float		CubeEdgeDirX[8];
	static const _float		CubeEdgeDirY[8];
	static const _float		CubeEdgeDirZ[8];


public:
	virtual void Free() override;
};

END