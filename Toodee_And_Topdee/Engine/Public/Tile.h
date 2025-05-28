#pragma once

#include "GameObject.h"
#include "Util.h"

BEGIN(Engine)

class ENGINE_DLL CTile abstract : public CGameObject
{
protected:
	CTile(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTile(const CTile& Prototype);
	virtual ~CTile() = default;
public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;
	BLOCK_INFO		Get_BlockInfo();

protected:
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };
	_uint						m_iTileSizeX;
	_uint						m_iTileSizeY;

protected:
	HRESULT						IsTileClicked(Picking::Ray ray);
	void						Set_Type(_int _newBlockType, _int _newTextureType);
	_int						m_iTextureType = 0;
	_int						m_iBlockType = 0;
	_int						m_iTextureDir = 0;

public:
	virtual CTile* Clone(void* pArg) = 0;
	virtual void		Free() override;

};

END