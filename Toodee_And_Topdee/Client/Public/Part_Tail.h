#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Tail final : public Engine::CParts
{
private:
	CPart_Tail(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Tail(const CPart_Tail& Prototype);
	virtual ~CPart_Tail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render(void* pArg) override;

private:

	_int	m_iDeltaAngleX = {};
	_int	m_iDeltaAngleY = {};

public:
	static CPart_Tail* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END