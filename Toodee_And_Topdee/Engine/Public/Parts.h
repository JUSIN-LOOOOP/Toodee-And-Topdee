#pragma once

#include "Component.h"

// ���� ������Ʈ���� ����(����)�� �θ�Ŭ����

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	enum class PARTSTATE { PARTS_FRONT, PARTS_BACK, PARTS_LEFT, PARTS_RIGHT, PARTS_UP, PS_DOWN, PARTS_CENTER };

	typedef struct tagPart_Desc
	{
		class CVIBuffer_Rect*	pVIBufferCom;
		class CTexture*			pTextureCom;

		_float					fAngleX{}, fAngleY{};	// ��ü���� ������ġ��
		_float					fFrame{}, fMaxFrame{};	// 1���� �̹����� �ƴѰ��
		_wstring				strOtherName{};			// �������� �б��ϱ�����
		PARTSTATE				eState{};				// ��ü���� ��ġ���� (�¿츸 �����ص��Ǳ��ؼ� �ʿ������ PARTSTATE���� ���Ͽ���)
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

	// ������ ������ ���� enum
	PARTSTATE	m_eState = {};				// ������ ��ġ����
	_wstring	m_strOtherName{};				// ���ΰ�ü�� �̸� �б⹮ �������� ��ɼ���������
	_float		m_fAngleX{}, m_fAngleY{};	// ��ġ�� ����
	_float		m_fFrame{}, m_fMaxFrame{};		// �����̹����� �ƴҰ�� ���


protected:
	// ������ ��ü�� �߽����� ����(�˵�����)
	void	RevolveAround(class CTransform* pTransform, _float fAngleX, _float fAngleY, _float fRightLength = 0.f, _float fUpLength = 0.f);
	void	Look_At_degree(_float* pAngleXOut, _float* pAngleYOut, CTransform* pTransform, _float3 vTargetPos);
	void	Check_To_FocusDelta(_float* pOutX, _float* pOutY, _float3 vFocusPos, _float3 vMyPos);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END