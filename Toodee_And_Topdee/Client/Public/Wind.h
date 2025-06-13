#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CWind final : public CPSystem
{
private:
	CWind(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWind(const CWind& Prototype);
	virtual ~CWind() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;
	_uint	m_iCountingParticle = 0;

public:
	static CWind* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END