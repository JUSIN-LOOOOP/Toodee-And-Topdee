#include "Collision.h"


const _float  CCollision::CubeEdgeDirX[8] = { 1,  1, -1, -1,  1,  1,  -1, -1 };
const _float  CCollision::CubeEdgeDirY[8] = { 1,  1,  1,  1, -1, -1, - 1, -1 };
const _float  CCollision::CubeEdgeDirZ[8] = { 1, -1,  1, -1,  1, -1,   1, -1 };

CCollision::CCollision()
{
}

_bool CCollision::Collision_Rect_Rect(const OBB& RectA, const OBB& RectB)
{
	_float3 centerA = RectA.center;
	_float3 centerB = RectB.center;

	_float3 halfSizeA = RectA.halfSize;
	_float3 halfSizeB = RectB.halfSize;

	_float3 CubeA_Edge[8] = {};
	_float3 CubeB_Edge[8] = {};

	for (size_t i = 0; i < 8; ++i)
		CubeA_Edge[i] = {
		centerA.x + RectA.axis[0].x * halfSizeA.x * CubeEdgeDirX[i],
		centerA.y + RectA.axis[1].y * halfSizeA.y * CubeEdgeDirY[i],
		centerA.z + RectA.axis[2].z * halfSizeA.z * CubeEdgeDirZ[i] };

	for (size_t i = 0; i < 8; ++i)
		CubeB_Edge[i] = {
		centerB.x + RectB.axis[0].x * halfSizeB.x * CubeEdgeDirX[i],
		centerB.y + RectB.axis[1].y * halfSizeB.y * CubeEdgeDirY[i],
		centerB.z + RectB.axis[2].z * halfSizeB.z * CubeEdgeDirZ[i] };


	//a,b�� x,z �� �� �˻� 
	_float3 axes[4] = {
		   RectA.axis[0], RectA.axis[2],
		   RectB.axis[0], RectB.axis[2]
	};

	for (int i = 0; i < 4; ++i) {
		if (!OverlapOnAxis(4, CubeA_Edge, CubeB_Edge, axes[i]))
			return false;
	}
	return true;

}

_bool CCollision::Collision_Rect_Cube(const OBB& RectA, const OBB& CubeB)
{
	_float3 centerA = RectA.center;
	
	_float3 centerB = CubeB.center;
	_float3 halfSizeB = CubeB.halfSize;

	_float3 CubeB_Edge[8] = {};
	for (size_t i = 0; i < 8; ++i)
		CubeB_Edge[i] = {
		centerB.x + CubeB.axis[0].x * halfSizeB.x * CubeEdgeDirX[i],
		centerB.y + CubeB.axis[1].y * halfSizeB.y * CubeEdgeDirY[i],
		centerB.z + CubeB.axis[2].z * halfSizeB.z * CubeEdgeDirZ[i] };

	/* ��鿡  �����ϴ� ���� ������ ������ ����� ���� */
	_float	fUpAndDown = {}; 
	_int fUp = { 0 }, fDown= { 0 };

	for (size_t i = 0; i < 8; ++i)
	{
		_float3 temp = CubeB_Edge[i] - centerA;
		fUpAndDown = D3DXVec3Dot(&RectA.axis[1], &temp);

		if (fUpAndDown > 0)
			++fUp;
		else
			++fDown;;
	}

	return ( fUp > 0 && fDown > 0);
}

_bool CCollision::Collision_Cube_Cube(const OBB& CubeA, const OBB& CubeB)
{
	_float3 centerA = CubeA.center;
	_float3 centerB = CubeB.center;

	_float3 halfSizeA = CubeA.halfSize;
	_float3 halfSizeB = CubeB.halfSize;

	_float3 CubeA_Edge[8] = {};
	_float3 CubeB_Edge[8] = {};

	for (size_t i = 0; i < 8; ++i)
		CubeA_Edge[i] = {
		centerA.x + CubeA.axis[0].x * halfSizeA.x * CubeEdgeDirX[i],
		centerA.y + CubeA.axis[1].y * halfSizeA.y * CubeEdgeDirY[i],
		centerA.z + CubeA.axis[2].z * halfSizeA.z * CubeEdgeDirZ[i] };

	for (size_t i = 0; i < 8; ++i)
		CubeB_Edge[i] = {
		centerB.x + CubeB.axis[0].x * halfSizeB.x * CubeEdgeDirX[i],
		centerB.y + CubeB.axis[1].y * halfSizeB.y * CubeEdgeDirY[i],
		centerB.z + CubeB.axis[2].z * halfSizeB.z * CubeEdgeDirZ[i] };


	_float3 axes[15];
	_uint count = 0;

	// a�� 3��, b�� 3��
	for (_uint i = 0; i < 3; ++i) axes[count++] = CubeA.axis[i];
	for (_uint i = 0; i < 3; ++i) axes[count++] = CubeB.axis[i];

	// ���� �� 9��
	for (_uint i = 0; i < 3; ++i) {
		for (_uint j = 0; j < 3; ++j) {
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &CubeA.axis[i], &CubeB.axis[j]);
			axes[count++] = cross;
		}
	}


	for (_uint i = 0; i < count; ++i) {
		if (!OverlapOnAxis(8, CubeA_Edge, CubeB_Edge, axes[i]))
			return false;
	}
	return true;
}

// �࿡ ������ [min, max] ���� ���
void CCollision::GetProjection(const size_t& iLength, const _float3* vEdge,  const _float3& vAxis, _float& fMin, _float& fMax) {

	fMin = fMax = D3DXVec3Dot(&vAxis, &vEdge[0]);

	for (size_t i = 1; i < iLength ; ++i) {
		_float proj = D3DXVec3Dot(&vAxis, &vEdge[i]);
		if (proj < fMin) fMin = proj;
		if (proj > fMax) fMax = proj;
	}
}

// �˻��ҷ��� �� �߽����� �� ���� ������ ��ġ����
_bool CCollision::OverlapOnAxis(const size_t& iLength, const _float3* vEdgeA, const _float3* vEdgeB, const _float3& vAxis) {

	_float fMinA, fMaxA, fMinB, fMaxB;

	GetProjection(iLength, vEdgeA, vAxis, fMinA, fMaxA);
	GetProjection(iLength, vEdgeB, vAxis, fMinB, fMaxB);

	return !(fMaxA < fMinB || fMaxB < fMinA); // �ϳ��� �� ��ġ�� �浹 X
}


void CCollision::Free()
{
    __super::Free();
}
