#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CSpikes final : public CGameObject
{
private:
	CSpikes(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpikes(const CSpikes& Prototype);
	virtual ~CSpikes() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	//Dimension Change
	DIMENSION					m_eRenderDimension = {};
	_bool						m_bChange_Dimension = {};
	_float						m_fAnimDelay = {};
	_float						m_fCurrentAnimTime = {};
	_uint						m_iAnimCount = {};
	_uint						m_iMaxAnimCount = {};
	_uint						m_iIncreaseAnimCount = {};
	_uint						m_iCount = {};

private:
	void Check_Dimension();
	void Update_AnimCount(_float fTimeDelta);


	HRESULT Ready_Components();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


public:
	static CSpikes* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END