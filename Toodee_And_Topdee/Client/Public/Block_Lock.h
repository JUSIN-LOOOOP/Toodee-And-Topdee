#pragma once
#include "Client_Defines.h"
#include "Block.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTransform;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CBlock_Lock final : public CBlock
{
private:
	CBlock_Lock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Lock(const CBlock_Lock& Prototype);
	virtual ~CBlock_Lock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private: 
	HRESULT Ready_Components();
	void	SetUp_RenderState();
	void	Reset_RenderState();


public:
	static CBlock_Lock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

