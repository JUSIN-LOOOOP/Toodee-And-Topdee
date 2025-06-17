#pragma once

#include "Component.h"

// 각종 오브젝트들의 파츠(부위)의 부모클래스

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	enum class PARTSTATE { PARTS_FRONT, PARTS_BACK, PARTS_LEFT, PARTS_RIGHT, PARTS_UP, PS_DOWN, PARTS_CENTER };

	typedef struct tagPart_Desc
	{
		class CVIBuffer_Rect*	pVIBufferCom;
		class CTexture*			pTextureCom;

		_float					fAngleX{}, fAngleY{};	// 객체기준 파츠위치값
		_float					fFrame{}, fMaxFrame{};	// 1장의 이미지가 아닌경우
		_wstring				strOtherName{};			// 파츠에서 분기하기위함
		PARTSTATE				eState{};				// 객체기준 위치구분 (좌우만 구분해도되긴해서 필요없으면 PARTSTATE갯수 줄일예정)
	}PART_DESC;

protected:
	CParts(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParts(const CParts& Prototype);
	virtual ~CParts() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(class CTransform* pTransform, _float fTimeDelta, _float3	vFocusPos = _float3(0.f, 0.f, 0.f));
	virtual HRESULT Render();

protected:
	class CVIBuffer_Rect*	m_pVIBufferCom	= { nullptr };
	class CTexture*			m_pTextureCom	= { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };

	// 파츠의 구분을 위한 enum
	PARTSTATE	m_eState = {};				// 파츠의 위치구분
	_wstring	m_strOtherName{};				// 주인객체의 이름 분기문 구분으로 기능수행을위함
	_float		m_fAngleX{}, m_fAngleY{};	// 위치값 세팅
	_float		m_fFrame{}, m_fMaxFrame{};		// 고정이미지가 아닐경우 사용


protected:
	// 파츠가 객체를 중심으로 공전(궤도공전)
	void	RevolveAround(class CTransform* pTransform, _float fAngleX, _float fAngleY, _float fRightLength = 0.f, _float fUpLength = 0.f);
	void	Look_At_degree(_float* pAngleXOut, _float* pAngleYOut, CTransform* pTransform, _float3 vTargetPos);
	void	Check_To_FocusDelta(_float* pOutX, _float* pOutY, _float3 vFocusPos, _float3 vMyPos);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END