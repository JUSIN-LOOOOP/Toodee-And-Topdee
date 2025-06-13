#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_FinalBoss02 final : public CLevel
{
private:
	enum ATKPATTERN { SEMICLON, FINGER, PATTERN_END };
private:
	CLevel_FinalBoss02(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_FinalBoss02() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private :
	_float	m_fIdleTime = {};
	_float	m_fDelayTime = {};
	_bool	m_bAtkFlag[PATTERN_END] = { false, false };
	_uint	m_iPlayLevel = { };

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_MapObject(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Back(const _wstring& strLayerTag);
	HRESULT Ready_Layer_ColliderMap(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag);
	void	CreateHitBox(_float fTimeDelta);

public:
	static CLevel_FinalBoss02* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END