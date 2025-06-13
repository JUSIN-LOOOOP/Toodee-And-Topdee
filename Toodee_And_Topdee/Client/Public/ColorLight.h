#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CColorLight final : public CPSystem
{
private:
	CColorLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	CColorLight(const CColorLight& Prototype);
	virtual ~CColorLight() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;
	_uint	m_iLightCount = 0;

public:
	static CColorLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END