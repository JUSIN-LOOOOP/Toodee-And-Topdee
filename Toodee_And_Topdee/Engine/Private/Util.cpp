#include "Util.h"

Picking::Ray Picking::GetRayFromMouse(const unsigned int WinX, const unsigned int WinY, HWND hWnd, LPDIRECT3DDEVICE9 pGraphic_Device)
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	//	[1] 마우스 좌표의 뷰스페이스 좌표 구하기
	//		((2.0f * x) / vp.Width) - 1.0f) <- 여기까지가 NDC좌표
	//		그 다음에 proj._11로 하면 NDC좌표에서 프로젝션 행렬을 곱하기 전으로 가니까 뷰스페이스 좌표!
	D3DVIEWPORT9 vp;
	D3DMATRIX proj;
	pGraphic_Device->GetViewport(&vp);
	pGraphic_Device->GetTransform(D3DTS_PROJECTION, &proj);

	_float px = (((2.0f * ptMouse.x) / vp.Width) - 1.0f) / proj._11;
	_float py = (((-2.0f * ptMouse.y) / vp.Height) + 1.0f) / proj._22;


	//  [2] VIEW 역행렬을 구해서 월드공간의 위치좌표만 뽑아옴
	D3DXMATRIX view, viewInv;
	pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&viewInv, nullptr, &view);
	_float3 origin(viewInv._41, viewInv._42, viewInv._43);
	//D3DXVec3TransformCoord(&origin, &origin, &viewInv);


	//	[3] 뷰공간과 월드공간간의 Ray 방향 구하기
	_float3 dirView(px, py, 1.f);
	_float3 dir;
	D3DXVec3TransformNormal(&dir, &dirView, &viewInv);	//뷰행렬 <-> 월드행렬 광선방향
	D3DXVec3Normalize(&dir, &dir);


	return Picking::Ray{ origin, dir };
}

HRESULT Picking::RayIntersectsAABB(const Ray& ray, const _float3& min, const _float3& max, _float* pDist)
{
	return E_FAIL;
}

HRESULT Picking::RayIntersectsTriangle(const Ray& ray, const _float3& v0, const _float3& v1, const _float3& v2, _float* pDist)
{
	_bool hit = D3DXIntersectTri(&v0, &v1, &v2, &ray.origin, &ray.dir, nullptr, nullptr, nullptr);

	if (hit)
		return S_OK;
	else
		return E_FAIL;
}
