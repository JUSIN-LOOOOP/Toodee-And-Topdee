#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollision  final: public CBase
{
public:
	typedef struct ColliderTransformInfoOBB {
		_float3 center;
		_float3 axis[3]; // 정규화된 x,y,z 방향 벡터
		_float3 halfSize;

	}OBB;

	typedef struct ColliderTransformInfoAABB {
		_float3 vMin;  // 좌하단-뒤쪽 모서리
		_float3 vMax;  // 우상단-앞쪽 모서리
	}AABB;

private:
	CCollision();
	virtual ~CCollision() = default;

public:
	static AABB				Create_AABB(const OBB& info);
	static _bool			Collision_AABB(const AABB& A, const AABB& B);
	static _bool			Collision_Rect_Rect(const OBB& RectA, const OBB& RectB);
	static _bool			Collision_Rect_Cube(const OBB& RectA, const OBB& CubeB);
	static _bool			Collision_Cube_Cube(const OBB& CubeA, const OBB& CubeB);

	static void				GetProjection(const _uint& iLength ,const _float3* vEdge, const _float3& vAxis, _float& fMin, _float& fMax);
	static _bool			OverlapOnAxis(const _uint& iLengthA, const _uint& iLengthB, const _float3* vEdgeA, const _float3* vEdgeB, const _float3& vAxis);

public:
	static const _float3	CubeCornerSigns[8];
	static const _float3	RectCornerSigns[4];

public:
	virtual void Free() override;
};

END