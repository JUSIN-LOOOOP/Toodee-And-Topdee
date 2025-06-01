#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Nose final : public Engine::CParts
{
private:
	CPart_Nose(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Nose(const CPart_Nose& Prototype);
	virtual ~CPart_Nose() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render(void* pArg) override;

private:
	_int	m_iTextureIndex = {};
	_int	m_iDeltaAngleX = {};
	_int	m_iDeltaAngleY = {};

public:
	static CPart_Nose* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END