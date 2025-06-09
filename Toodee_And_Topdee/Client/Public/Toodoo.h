#pragma once
#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CToodoo : public CGameObject
{
private:
	CToodoo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CToodoo(const CToodoo& Prototype);
	virtual ~CToodoo() = default;
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
	_bool				offsetFlag = false;
	_float				m_fDeltaTime = {};
	_float				m_fOffset = {};

	HRESULT				Ready_Components();

public:
	static CToodoo*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CToodoo*		Clone(void* pArg) override;
	virtual void			Free() override;

};

END