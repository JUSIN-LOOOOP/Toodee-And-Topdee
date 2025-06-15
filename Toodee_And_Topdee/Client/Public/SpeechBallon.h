#pragma once

#include "UIObject.h"
#include "Client_Defines.h"
#include "Event.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
END

BEGIN(Client)

class CSpeechBallon final : public CUIObject
{
public:
	typedef struct tagSpeechDesc{
		const _tchar*	strText;
		RECT			tRect;
		D3DCOLOR		dwColor;
		_uint			iSpeechNumber;
	}SPEECH_DESC;

private:
	CSpeechBallon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpeechBallon(const CSpeechBallon& Prototype);
	virtual ~CSpeechBallon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	

	void SpeechNumber(const SPEECHNUMBER_EVENT& Event);

private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	RECT		m_tRect = {};
	const _tchar* m_strText = {};
	_tchar*		m_pText = {};
	_uint		m_iTextLength = {};
	_uint		m_iCurrentTextLength = {};
	_uint		m_iSpeechNumber = {};
	_uint		m_iCurrentSpeechNumber = {};
	D3DCOLOR	m_dwColor = {};

	_float		m_fTime = {};


private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
public:
	static CSpeechBallon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END