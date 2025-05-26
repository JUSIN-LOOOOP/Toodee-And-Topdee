#pragma once

#include "Component.h"

// ���� ������Ʈ���� ����(����)�� �θ�Ŭ����

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	typedef struct tagPart_Desc
	{
		_float					fTheta{}, fPhi{},fTextureMaxIndex{}, fParentRotation = 0;// ������ rotation�� ��ü������ ��ü ȸ�� ����Ǹ� �굵 ���ּ�
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

protected: // �� �������� ���������� �ʿ��� ������Ʈ��
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

	// ������ ������ ���� enum
	PARTSTATE	m_eState = {};
	// �ؽ��� �ε带 ���� ���� �ؽ��� ������Ʈ�� �̸� �� ����
	_wstring	m_strTexTag = {};
	_uint		m_iTexLevelIndex = {};

	// ������ ��ġ�� ���� �� �浵(�¿�)/����(���Ʒ�) ����
	_float m_fTheta{} , m_fPhi{};


protected:
	// ������ ��ü�� �߽����� ����(�˵�����)
	// fRadius���� �⺻ �� 0 �������� �߽ɿ��� ����߸��� ���� ��ŭ �߰��Ͽ�
	void RevolveAround(class CTransform* pTransform, _float fWidth, _float fDepth, _float fRadius = 0.f);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END