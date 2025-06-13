#pragma once
#include "Client_Defines.h"

#include "BlendObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackCloud : public CBlendObject
{
private:
	CBackCloud(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackCloud(const CBackCloud& Prototype);
	virtual ~CBackCloud() = default;
public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;

private: 
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };

	HRESULT				Ready_Components();
	HRESULT				Begin_RenderState();
	HRESULT				End_RenderState();

public:
	static CBackCloud*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CBackCloud*		Clone(void* pArg) override;
	virtual void			Free() override;

};

END