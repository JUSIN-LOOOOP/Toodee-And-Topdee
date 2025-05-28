#pragma once

#include "Component.h"

// 각종 오브젝트들의 파츠(부위)의 부모클래스

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	enum class PARTSTATE { PARTS_FRONT, PARTS_BACK, PARTS_LEFT, PARTS_RIGHT, PARTS_UP, PS_DOWN, PARTS_CENTER };
	enum class TEXTURESTATE { TEXTURE_ONLY, TEXTURE_ANIMATION };

	typedef struct tagPart_Desc
	{
		_float					fTheta{}, fPhi{}, fFrame{};
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
	virtual void	Update(class CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth);
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

	// 파츠의 위치를 결정 할 경도(좌우)/위도(위아래) 및 body스케일 설정
	_float		m_fTheta{}, m_fPhi{};
	_float3		m_vBodyScale{};


protected:
	// 이건 봐서 transform 컴포넌트로 옮기던가 해야겠음
	// 파츠가 객체를 중심으로 공전(궤도공전)
	// fPartsScale값은 기본 값 1 파츠별로 크기변경 (몸통Scale * 입력값 기본은 1.f)
	void RevolveAround(class CTransform* pTransform, _float fWidth, _float fHeight, _float fPartsScale = 1.f);
	// 객체 이동시 파츠 회전에 대한 조건(예외처리 코드가 난잡한거같아서 함수화시킬게)
	void If_Revolved(_float& fWidth, _float& fHeight, _float fInputWidth, _float fInputHeight);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END