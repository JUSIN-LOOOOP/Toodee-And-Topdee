#pragma once

#include "Component.h"

// 각종 오브젝트들의 파츠(부위)의 부모클래스

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	typedef struct tagPart_Desc
	{
		_float					fTheta{}, fPhi{},fTextureMaxIndex{}, fParentRotation = 0;// 마지막 rotation은 객체생성시 객체 회전 적용되면 얘도 해주셈
		_wstring				strTexTag;
		_uint					iTexLevelIndex{}, iTextureIndex{};
		PARTSTATE				eState;
	}PART_DESC;

protected:
	CParts(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParts(const CParts& Prototype);
	virtual ~CParts() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update( class CTransform* pTransform, _float fTheta, _float fPhi);
	virtual HRESULT Render();

protected: // 각 파츠에서 공통적으로 필요한 컴포넌트들
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

	// 파츠의 구분을 위한 enum
	PARTSTATE	m_eState = {};
	// 텍스쳐 로드를 위한 원형 텍스쳐 컴포넌트의 이름 및 레벨
	_wstring	m_strTexTag = {};
	_uint		m_iTexLevelIndex = {};

	// 파츠의 위치를 결정 할 경도(좌우)/위도(위아래) 설정
	_float m_fTheta{} , m_fPhi{};


protected:
	// 파츠가 객체를 중심으로 공전(궤도공전)
	// fRadius값은 기본 값 0 파츠별로 중심에서 떨어뜨리고 싶은 만큼 추가하여
	void RevolveAround(class CTransform* pTransform, _float fWidth, _float fDepth, _float fRadius = 0.f);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END