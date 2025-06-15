#pragma once

#include "Client_Defines.h"
#include "PSystem.h"
#include "Util.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPotalEffect final : public CPSystem
{
private:
	CPotalEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPotalEffect(const CPotalEffect& Prototype);
	virtual ~CPotalEffect() = default;

public:
	HRESULT Initialize() override;
	void	ResetParticle(PARTICLE* attribute, void* pArg = nullptr) override;
	void	Update(_float fTimeDelta) override;

private :
	_float	m_fFrameDuration = 0.f;
	_float	m_DurationTime = 0.f;
	CTransform* m_pPlayer = { nullptr };
	_uint	m_Count = 0;

public:
	static CPotalEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END