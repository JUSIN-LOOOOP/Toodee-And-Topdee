#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Event.h"

BEGIN(Client)

class CLevel_StageBoss final : public CLevel
{
private:
	CLevel_StageBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_StageBoss() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void		ResetBlock(const FIANLBOSSRESET_EVENT& Event);
	HRESULT		Ready_SubscribeEvent(_uint Level);
	
	BLOCK_INFO	m_LockBlockInfo[4];
	BLOCK_INFO	m_KeyInfo[3];
	BLOCK_INFO	m_SpikeInfo[30];

	_uint		m_DeadCount = 0;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_MapObject(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Back(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Cannon(const _wstring& strLayerTag);
	HRESULT Ready_Layer_ColliderMap(const _wstring& strLayerTag);
	HRESULT Ready_Layer_StageBoss(const _wstring& strLayerTag);


public:
	static CLevel_StageBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END