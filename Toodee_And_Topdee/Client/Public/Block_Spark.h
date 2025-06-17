#pragma once
#include "Client_Defines.h"
#include "InteractionBlock.h"

BEGIN(Client)

class CBlock_Spark final : public CInteractionBlock
{
private:
	CBlock_Spark(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Spark(const CBlock_Spark& Prototype);
	virtual ~CBlock_Spark() = default;

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
	static CBlock_Spark* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

