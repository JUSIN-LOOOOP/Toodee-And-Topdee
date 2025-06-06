#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Body final : public Engine::CParts
{
private:
	CPart_Body(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Body(const CPart_Body& Prototype);
	virtual ~CPart_Body() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos) override;
	virtual HRESULT Render() override;

private:
	_float m_fOldFrame{};


private:
	void Pos_Set(class CTransform* pTransform);
public:
	static CPart_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END