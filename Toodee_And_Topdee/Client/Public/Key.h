#pragma once
#include "Client_Defines.h"
#include "Gameobject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CKey final : public CGameObject
{
private:
	CKey(LPDIRECT3DDEVICE9 pGraphic_Device);
	CKey(const CKey& Prototype);
	virtual ~CKey() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Get_Key();

private: 

	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };

	_bool	m_bTextureChangeDirection = { false };
	_uint	m_iTextureIdx = {};
	_float	m_iIntervalTime = {0.07f};
	_float	m_iAccumulateTime = { 0.f };
	_float	m_iTwinkleDelayTime = { 0.f };


private:
	HRESULT Ready_Components();
	void	Change_Motion(_float fTimeDelta);

public:
	static CKey* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

