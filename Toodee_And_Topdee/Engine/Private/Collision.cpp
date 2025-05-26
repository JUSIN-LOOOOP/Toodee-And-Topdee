#include "Collision.h"


const _float  CCollision::CubeEdgeDirX[8] = { 1,  1, -1, -1 ,  1,  1,  -1, -1};
const _float  CCollision::CubeEdgeDirY[8] = { 1,  1,  1,  1 , -1, -1, - 1, -1};
const _float  CCollision::CubeEdgeDirZ[8] = { 1, -1,  1, -1 ,  1, -1,   1, -1};

CCollision::CCollision()
{
}

_bool CCollision::Collision_Rect_Rect(const OBB& RectA, const OBB& RectB)
{
	_float3 centerA = RectA.center;
	_float3 centerB = RectB.center;

	_float3 halfSizeA = RectA.halfSize;
	_float3 halfSizeB = RectB.halfSize;

	_float3 CubeA_Edge[4] = {};
	_float3 CubeB_Edge[4] = {};

	for (size_t i = 0; i < 4; ++i)
		CubeA_Edge[i] = {
		centerA.x + RectA.axis[0].x * halfSizeA.x * CubeEdgeDirX[i],
		RectA.axis[1].y ,
		centerA.z + RectA.axis[2].z * halfSizeA.z * CubeEdgeDirZ[i] };

	for (size_t i = 0; i < 4; ++i)
		CubeB_Edge[i] = {
		centerB.x + RectB.axis[0].x * halfSizeB.x * CubeEdgeDirX[i],
		RectB.axis[1].y,
		centerB.z + RectB.axis[2].z * halfSizeB.z * CubeEdgeDirZ[i] };


	//a,b�� x,z �� �� �˻� 
	_float3 axes[4] = {
		   RectA.axis[0], RectA.axis[2],
		   RectB.axis[0], RectB.axis[2]
	};

	for (int i = 0; i < 4; ++i) {
		if (!OverlapOnAxis(4,4, CubeA_Edge, CubeB_Edge, axes[i]))
			return false;
	}
	return true;

}

_bool CCollision::Collision_Rect_Cube(const OBB& RectA, const OBB& CubeB)
{
	_float3 centerA = RectA.center;
	_float3 halfSizeA = RectA.halfSize;

	_float3 centerB = CubeB.center;
	_float3 halfSizeB = CubeB.halfSize;

	_float3 CubeA_Edge[4];
	_float3 CubeB_Edge[8] = {};

	for (size_t i = 0; i < 4; ++i)
		CubeA_Edge[i] = centerA + RectA.axis[0] * (halfSizeA.x * CubeEdgeDirX[i]) + RectA.axis[2] * (halfSizeA.z * CubeEdgeDirZ[i]);

	for (size_t i = 0; i < 8; ++i)
	CubeB_Edge[i] = centerB 
		+ CubeB.axis[0] * (halfSizeB.x * CubeEdgeDirX[i]) 
		+ CubeB.axis[1] * (halfSizeB.y * CubeEdgeDirY[i]) 
		+ CubeB.axis[2] * (halfSizeB.z * CubeEdgeDirZ[i]);

	_float3 axes[11];
	_uint count = 0;

	// a�� 2��, b�� 3��
	axes[count++] = RectA.axis[0];
	axes[count++] = RectA.axis[2];
	for (_uint i = 0; i < 3; ++i) axes[count++] = CubeB.axis[i];

	// ���� �� 6��
	for (_uint i = 0; i < 3; i += 2) {
		for (_uint j = 0; j < 3; ++j) {
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &RectA.axis[i], &CubeB.axis[j]);

			//_float length = D3DXVec3Length(&cross);
			//if (length > 0.0001f) {
			//	D3DXVec3Normalize(&cross, &cross);
				axes[count++] = cross;
			//}
		}
	}

		for (_uint i = 0; i < count; ++i) {
			if (!OverlapOnAxis(4, 8, CubeA_Edge, CubeB_Edge, axes[i]))
				return false;
		}
		return true;
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
		if (!OverlapOnAxis(8,8, CubeA_Edge, CubeB_Edge, axes[i]))
			return false;
	}
	return true;
}

// �࿡ ������ [min, max] ���� ���
void CCollision::GetProjection(const size_t& iLength ,const _float3* vEdge,  const _float3& vAxis, _float& fMin, _float& fMax) {

	fMin = fMax = D3DXVec3Dot(&vAxis, &vEdge[0]);

	for (size_t i = 1; i < iLength ; ++i) {
		_float proj = D3DXVec3Dot(&vAxis, &vEdge[i]);
		if (proj < fMin) fMin = proj;
		if (proj > fMax) fMax = proj;
	}
}

// �˻��ҷ��� �� �߽����� �� ���� ������ ��ġ����
_bool CCollision::OverlapOnAxis(const size_t& iLengthA, const size_t& iLengthB, const _float3* vEdgeA, const _float3* vEdgeB, const _float3& vAxis) {

	_float fMinA, fMaxA, fMinB, fMaxB;

	GetProjection(iLengthA, vEdgeA, vAxis, fMinA, fMaxA);
	GetProjection(iLengthB, vEdgeB, vAxis, fMinB, fMaxB);

	return !(fMaxA < fMinB || fMaxB < fMinA); // �ϳ��� �� ��ġ�� �浹 X
}


void CCollision::Free()
{
    __super::Free();
}
