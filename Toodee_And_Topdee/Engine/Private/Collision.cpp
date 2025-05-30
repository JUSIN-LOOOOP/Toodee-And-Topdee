#include "Collision.h"

const _float3 CCollision::CubeCornerSigns[8] = {
    { -1.f, -1.f, -1.f },
    {  1.f, -1.f, -1.f },
    { -1.f,  1.f, -1.f },
    {  1.f,  1.f, -1.f },
    { -1.f, -1.f,  1.f },
    {  1.f, -1.f,  1.f },
    { -1.f,  1.f,  1.f },
    {  1.f,  1.f,  1.f }
};

const _float3 CCollision::RectCornerSigns[4] = {
    { -1.f, 0.f, -1.f }, {  1.f, 0.f, -1.f },  { -1.f, 0.f,  1.f }, {  1.f, 0.f,  1.f }
};

CCollision::CCollision()
{
}

CCollision::AABB CCollision::Create_AABB(const OBB& info)
{
    CCollision::AABB box;
    box.vMin = info.center - info.halfSize;
    box.vMax = info.center + info.halfSize;

    return box;
}

_bool CCollision::Collision_AABB(const AABB& a, const AABB& b)
{
    return(a.vMin.x <= b.vMax.x && a.vMax.x >= b.vMin.x) &&
        (a.vMin.y <= b.vMax.y && a.vMax.y >= b.vMin.y) &&
        (a.vMin.z <= b.vMax.z && a.vMax.z >= b.vMin.z);
}

// X, Z축만 검사 
_bool CCollision::Collision_Rect_Rect(const OBB& RectA, const OBB& RectB)
{
    if (!Collision_AABB(Create_AABB(RectA), Create_AABB(RectB)))return false;

    _float3 CubeA_Edge[4] = { };
    _float3 CubeB_Edge[4] = { };

    for (_uint i = 0; i < 4; ++i) {
        CubeA_Edge[i] = RectA.center
            + RectA.axis[0] * (RectA.halfSize.x * RectCornerSigns[i].x)
            + RectA.axis[2] * (RectA.halfSize.z * RectCornerSigns[i].z);
    }

    for (_uint i = 0; i < 4; ++i) {
        CubeB_Edge[i] = RectB.center
            + RectB.axis[0] * (RectB.halfSize.x * RectCornerSigns[i].x)
            + RectB.axis[2] * (RectB.halfSize.z * RectCornerSigns[i].z);
    }


    _float3 axes[4] = {
        RectA.axis[0], RectA.axis[2],
        RectB.axis[0], RectB.axis[2]
    };

    for (_uint i = 0; i < 4; ++i) {
        if (!OverlapOnAxis(4, 4, CubeA_Edge, CubeB_Edge, axes[i]))
            return false;
    }
    return true;
}

_bool CCollision::Collision_Rect_Cube(const OBB& RectA, const OBB& CubeB)
{
    if (!Collision_AABB(Create_AABB(RectA), Create_AABB(CubeB)))return false;
    _float3 CubeA_Edge[4] = { };
    _float3 CubeB_Edge[8] = { };

    for (_uint i = 0; i < 4; ++i) {
        CubeA_Edge[i] = RectA.center
            + RectA.axis[0] * (RectA.halfSize.x * RectCornerSigns[i].x)
            + RectA.axis[2] * (RectA.halfSize.z * RectCornerSigns[i].z);
    }

    for (_uint i = 0; i < 8; ++i) {
        CubeB_Edge[i] = CubeB.center
            + CubeB.axis[0] * (CubeB.halfSize.x * CubeCornerSigns[i].x)
            + CubeB.axis[1] * (CubeB.halfSize.y * CubeCornerSigns[i].y)
            + CubeB.axis[2] * (CubeB.halfSize.z * CubeCornerSigns[i].z);
    }

    _float3 axes[11];
    _uint count = 0;

    // Rect의 축 2개 (X, Z)
    axes[count++] = RectA.axis[0];
    axes[count++] = RectA.axis[2];

    // Cube의 축 3개
    for (_uint i = 0; i < 3; ++i)
        axes[count++] = CubeB.axis[i];

    // 외적 축들  ( Rect의 X,Z축과 Cube의 3축 )
    for (_uint i = 0; i < 2; ++i) { // Rect는 X,Z만
        _uint rectAxisIdx = (i == 0) ? 0 : 2;
        for (_uint j = 0; j < 3; ++j) {
            _float3 cross;
            D3DXVec3Cross(&cross, &RectA.axis[rectAxisIdx], &CubeB.axis[j]);

            _float length = D3DXVec3Length(&cross);
            if (length > 0.0001f) {
                D3DXVec3Normalize(&cross, &cross);
                axes[count++] = cross;
            }
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
    if (!Collision_AABB(Create_AABB(CubeA), Create_AABB(CubeB)))return false;

    _float3 CubeA_Edge[8] = {};
    _float3 CubeB_Edge[8] = {};

    for (_uint i = 0; i < 8; ++i) {
        CubeA_Edge[i] = CubeA.center
            + CubeA.axis[0] * (CubeA.halfSize.x * CubeCornerSigns[i].x)
            + CubeA.axis[1] * (CubeA.halfSize.y * CubeCornerSigns[i].y)
            + CubeA.axis[2] * (CubeA.halfSize.z * CubeCornerSigns[i].z);
    }

    for (_uint i = 0; i < 8; ++i) {
        CubeB_Edge[i] = CubeB.center
            + CubeB.axis[0] * (CubeB.halfSize.x * CubeCornerSigns[i].x)
            + CubeB.axis[1] * (CubeB.halfSize.y * CubeCornerSigns[i].y)
            + CubeB.axis[2] * (CubeB.halfSize.z * CubeCornerSigns[i].z);
    }

    _float3 axes[15];
    _uint count = 0;

    // 각 Cube의 3개 축
    for (_uint i = 0; i < 3; ++i) axes[count++] = CubeA.axis[i];
    for (_uint i = 0; i < 3; ++i) axes[count++] = CubeB.axis[i];

    // 외적 축들 (9개)
    for (_uint i = 0; i < 3; ++i) {
        for (_uint j = 0; j < 3; ++j) {
            D3DXVECTOR3 cross;
            D3DXVec3Cross(&cross, &CubeA.axis[i], &CubeB.axis[j]);

            _float length = D3DXVec3Length(&cross);
            if (length > 0.0001f) {
                D3DXVec3Normalize(&cross, &cross);
                axes[count++] = cross;
            }
        }
    }

    for (_uint i = 0; i < count; ++i) {
        if (!OverlapOnAxis(8, 8, CubeA_Edge, CubeB_Edge, axes[i]))
            return false;
    }
    return true;
}

void CCollision::GetProjection(const _uint& iLength, const _float3* vEdge, const _float3& vAxis, _float& fMin, _float& fMax) {
    fMin = fMax = D3DXVec3Dot(&vAxis, &vEdge[0]);

    for (_uint i = 1; i < iLength; ++i) {
        _float proj = D3DXVec3Dot(&vAxis, &vEdge[i]);
        if (proj < fMin) fMin = proj;
        if (proj > fMax) fMax = proj;
    }
}

_bool CCollision::OverlapOnAxis(const _uint& iLengthA, const _uint& iLengthB, const _float3* vEdgeA, const _float3* vEdgeB, const _float3& vAxis) {
    _float fMinA = {}, fMaxA = {}, fMinB = {}, fMaxB = {};

    GetProjection(iLengthA, vEdgeA, vAxis, fMinA, fMaxA);
    GetProjection(iLengthB, vEdgeB, vAxis, fMinB, fMaxB);

    return !(fMaxA < fMinB || fMaxB < fMinA);
}

void CCollision::Free()
{
    __super::Free();
}