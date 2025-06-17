#pragma once
#include "Client_Defines.h"
#include "Block.h"
#include "Event.h"

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

	void Get_Key(const GETKEYEVENT& Event);
private:
	_uint	m_iPlayLevel = {};
	_uint	m_iTotalKeyCount = {};
	_uint	m_iGetKeyCount = {};
	_bool	m_bOpenLock = {};
private: 

	HRESULT Ready_Components();
	HRESULT Ready_SubscribeEvent(_uint iPlayLevel);
	void	SetUp_RenderState();
	void	Reset_RenderState();


public:
	static CBlock_Lock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

