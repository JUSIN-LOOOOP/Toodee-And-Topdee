#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Wing final : public Engine::CParts
{
private:
	CPart_Wing(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Wing(const CPart_Wing& Prototype);
	virtual ~CPart_Wing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render() override;

private:
	_float	m_fOldFrame{}, m_fDeltaAngleX{}, m_fDeltaAngleY{};
	
public:
	static CPart_Wing* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END