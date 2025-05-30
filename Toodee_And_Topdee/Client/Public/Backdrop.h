#pragma once
#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackdrop : public CGameObject
{
private:
	CBackdrop(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackdrop(const CBackdrop& Prototype);
	virtual ~CBackdrop() = default;
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
	_uint				m_iThemeIdx;

public:
	static CBackdrop*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CBackdrop*		Clone(void* pArg) override;
	virtual void			Free() override;

};

END