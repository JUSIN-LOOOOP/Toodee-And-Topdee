#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Eyes final : public Engine::CParts
{
private:
	CPart_Eyes(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Eyes(const CPart_Eyes& Prototype);
	virtual ~CPart_Eyes() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render() override;

private:
	_float	m_fDeltaAngleX = {}, m_fDeltaAngleY = {};

public:
	static CPart_Eyes* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END