#pragma once
#include "Client_Defines.h"
#include "Hole.h"


BEGIN(Client)

class CSpikeHole final : public CHole
{
private:
	CSpikeHole(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpikeHole(const CSpikeHole& Prototype);
	virtual ~CSpikeHole() = default;

public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;

private:
	HRESULT				Ready_Components();
	HRESULT				Begin_RenderState();
	HRESULT				End_RenderState();

public:
	static CSpikeHole* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END