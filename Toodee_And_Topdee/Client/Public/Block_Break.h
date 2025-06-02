#pragma once
#include "Client_Defines.h"
#include "SubjectObject.h"
#include "Block.h"

BEGIN(Client)

class CBlock_Break final : public CBlock
{
private:
	CBlock_Break(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Break(const CBlock_Break& Prototype);
	virtual ~CBlock_Break() = default;

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
	static CBlock_Break* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

