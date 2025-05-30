#pragma once
#include "Client_Defines.h"
#include "Block.h"

BEGIN(Client)

class CBlock_Wall final : public CBlock
{
private:
	CBlock_Wall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Wall(const CBlock_Wall& Prototype);
	virtual ~CBlock_Wall() = default;

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
	_uint	tmpIdx = {};
	_uint	key[2] = {};
public:
	static CBlock_Wall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

