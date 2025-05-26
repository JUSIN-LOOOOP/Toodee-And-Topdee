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
	virtual void	Update(CTransform* pTransform, _float fTheta, _float fPhi);
	virtual HRESULT Render() override;

private:
	_int   m_iTextureIndex = {};
	_float m_fWidth = {};
	_float m_fHeight = {};

public:
	static CPart_Nose* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END