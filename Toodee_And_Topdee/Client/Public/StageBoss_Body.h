#pragma once
#include "Client_Defines.h"
#include "StageBoss_limb.h"

BEGIN(Client)

class CStageBoss_Body : public CStageBoss_limb
{
private:
	enum class PARTS_TYPE { EYE_L, EYE_R, MOUTH, HORN1, HORN2, HORN3, Pupil1, Pupil2, CORN1, CORN2, PARTS_END };

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
	virtual HRESULT Initialize_Prototype(void* pArg);
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Parts();

private:
	virtual HRESULT Ready_Components();
	void			Ready_PartsData();
	HRESULT			Create_Fire();
	void			Calculate_Pupil_Pos();

	CTexture*		m_pPartsTextureCom = {  };
	_float			m_fAngle = {};
	_float			m_fIdleTime = {};
	_float			m_DamagedTime = {};

private:
	PARTS_DESC		m_sParts[ENUM_CLASS(PARTS_TYPE::PARTS_END)];
	_float			IdleTime = {};

private:
	virtual  HRESULT Render_Shadow() override;

public:
	static CStageBoss_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END