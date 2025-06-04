#pragma once
#include "Client_Defines.h"
#include "StageBoss_limb.h"

BEGIN (Client)

class CStageBoss_Body : public CStageBoss_limb
{
private:
	enum class PARTS_TYPE {EYE_L, EYE_R, MOUTH, HORN1, HORN2, HORN3, PARTS_END};

	typedef struct PARTS_DESC
	{
		CVIBuffer_Rect* pVIPartsBufferCom = { nullptr };
		_float3			fOffset = {};
		_float3			fScale = {};
		_float			fRot = {};
		_uint			TextureIdx = {};
	}PARTS_DESC;


private:
	CStageBoss_Body(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStageBoss_Body(const CStageBoss_Body& Prototype);
	virtual ~CStageBoss_Body() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();
	HRESULT	Create_Fire();

	CTexture* m_pPartsTextureCom = {  };

private:
	PARTS_DESC m_sParts[ENUM_CLASS(PARTS_TYPE::PARTS_END)];

public:
	static CStageBoss_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END