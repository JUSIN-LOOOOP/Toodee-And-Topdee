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
		_float					fAngleX{}, fAngleY{}, fFrame{};
		_float3					vBodyScale{};
		_wstring				strTexTag{};
		_uint					iTexLevelIndex{}, iTextureIndex{}, iTextureMaxIndex{};	//텍스쳐 로드시 필요한 정보들
		PARTSTATE				eState{};
	}PART_DESC;

protected:
	CParts(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParts(const CParts& Prototype);
	virtual ~CParts() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(class CTransform* pTransform, _float fTimeDelta, _float3	vFocusPos = _float3(0.f,0.f,0.f));
	virtual HRESULT Render(void* pArg = 0);

protected: // 각 파츠에서 공통적으로 필요한 컴포넌트들
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

	// 파츠의 구분을 위한 enum
	PARTSTATE	m_eState = {};

	// 텍스쳐 로드를 위한 원형 텍스쳐 컴포넌트의 이름 및 레벨
	_wstring	m_strTexTag{};
	_uint		m_iTexLevelIndex {},m_iTextureIndex{}, m_iTextureMaxIndex{};

	// 파츠의 위치를 결정 할 좌우, 상하 및 body스케일 설정
	_float		m_fAngleX{}, m_fAngleY{};
	_float3		m_vBodyScale{};


protected:
	// 파츠가 객체를 중심으로 공전(궤도공전)
	// fPartsScale값은 기본 값 1 파츠별로 크기변경 (몸통Scale)
	void RevolveAround(class CTransform* pTransform, _int iAngleX, _int iAngleY, _float fRadius = 0.f);
	void Check_To_FocusDelta(_int* pOutX, _int* pOutY, _float3 vFocusPos, _float3 vMyPos);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END