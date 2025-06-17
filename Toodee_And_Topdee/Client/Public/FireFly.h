#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CFireFly final : public CPSystem
{
private:
	CFireFly(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFireFly(const CFireFly& Prototype);
	virtual ~CFireFly() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;

public:
	static CFireFly* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END