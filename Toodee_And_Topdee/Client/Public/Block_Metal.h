#pragma once
#include "Client_Defines.h"
#include "Block.h"

BEGIN(Client)

class CBlock_Metal final : public CBlock
{
private:
	CBlock_Metal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Metal(const CBlock_Metal& Prototype);
	virtual ~CBlock_Metal() = default;

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
	static CBlock_Metal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

