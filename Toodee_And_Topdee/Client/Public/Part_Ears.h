#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Ears final : public Engine::CParts
{
private:
	CPart_Ears(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Ears(const CPart_Ears& Prototype);
	virtual ~CPart_Ears() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render(void* pArg) override;

private:
	_float  m_fDeltaAngleX = {};

	
public:
	static CPart_Ears* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END