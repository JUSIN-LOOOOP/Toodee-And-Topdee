#pragma once

#include "Component.h"

/* 텍스쳐 파일을 읽어서 장치에 할당되는 단계(바인딩) 클래스 */

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	HRESULT Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Initialize(void* pArg);
	HRESULT Bind_Texture(_uint iTextureIndex = 0);

private:
	_uint									m_iNumTextures = {};
	vector<IDirect3DBaseTexture9*>			m_Textures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END