#pragma once
#include "Client_Defines.h"

#include "GameObject.h"
#include "Tile.h"

BEGIN(Engine)

class CBasicTile : public CTile
{
private:
	CBasicTile(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBasicTile(const CBasicTile& Prototype);
	virtual ~CBasicTile() = default;
public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;

private:

private: 
	HRESULT				Ready_Components();
	

public:
	static CBasicTile*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CBasicTile*		Clone(void* pArg) override;
	virtual void		Free() override;

};

END