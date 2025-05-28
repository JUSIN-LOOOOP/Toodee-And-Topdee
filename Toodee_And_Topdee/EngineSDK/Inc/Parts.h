#pragma once

#include "Component.h"

// ���� ������Ʈ���� ����(����)�� �θ�Ŭ����

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
	virtual void	Update(class CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth);
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

	// ������ ��ġ�� ���� �� �浵(�¿�)/����(���Ʒ�) �� body������ ����
	_float		m_fTheta{}, m_fPhi{};
	_float3		m_vBodyScale{};


protected:
	// �̰� ���� transform ������Ʈ�� �ű���� �ؾ߰���
	// ������ ��ü�� �߽����� ����(�˵�����)
	// fPartsScale���� �⺻ �� 1 �������� ũ�⺯�� (����Scale * �Է°� �⺻�� 1.f)
	void RevolveAround(class CTransform* pTransform, _float fWidth, _float fHeight, _float fPartsScale = 1.f);
	// ��ü �̵��� ���� ȸ���� ���� ����(����ó�� �ڵ尡 �����ѰŰ��Ƽ� �Լ�ȭ��ų��)
	void If_Revolved(_float& fWidth, _float& fHeight, _float fInputWidth, _float fInputHeight);


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END