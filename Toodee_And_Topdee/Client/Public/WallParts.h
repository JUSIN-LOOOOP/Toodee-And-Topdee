#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CWallParts final : public CPSystem
{
private:
	CWallParts(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWallParts(const CWallParts& Prototype);
	virtual ~CWallParts() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	CTransform* m_pTopdee = { nullptr };
	CTransform* m_pToodee = { nullptr };
	_bool		m_bOutFlag = false;
	_float3		m_BoxCenter = {};

public:
	static CWallParts* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END