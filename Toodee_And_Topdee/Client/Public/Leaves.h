#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CLeaves final : public CPSystem
{
private:
	CLeaves(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLeaves(const CLeaves& Prototype);
	virtual ~CLeaves() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;

public:
	static CLeaves* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END