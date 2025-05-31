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
		_float					fAngleX{}, fAngleY{}, fFrame{};
		_float3					vBodyScale{};
		_wstring				strTexTag{};
		_uint					iTexLevelIndex{}, iTextureIndex{}, iTextureMaxIndex{};	//�ؽ��� �ε�� �ʿ��� ������
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

protected: // �� �������� ���������� �ʿ��� ������Ʈ��
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

	// ������ ������ ���� enum
	PARTSTATE	m_eState = {};

	// �ؽ��� �ε带 ���� ���� �ؽ��� ������Ʈ�� �̸� �� ����
	_wstring	m_strTexTag{};
	_uint		m_iTexLevelIndex {},m_iTextureIndex{}, m_iTextureMaxIndex{};

	// ������ ��ġ�� ���� �� �¿�, ���� �� body������ ����
	_float		m_fAngleX{}, m_fAngleY{};
	_float3		m_vBodyScale{};


protected:
	// ������ ��ü�� �߽����� ����(�˵�����)
	// fPartsScale���� �⺻ �� 1 �������� ũ�⺯�� (����Scale)
	void RevolveAround(class CTransform* pTransform, _int iAngleX, _int iAngleY, _float fRadius = 0.f);
	void Check_To_FocusDelta(_int* pOutX, _int* pOutY, _float3 vFocusPos, _float3 vMyPos);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END