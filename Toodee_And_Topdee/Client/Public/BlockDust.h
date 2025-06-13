#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CBlockDust final : public CPSystem
{
private:
	CBlockDust(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlockDust(const CBlockDust& Prototype);
	virtual ~CBlockDust() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;
	CTransform* m_pPlayer = { nullptr };

public:
	static CBlockDust* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END