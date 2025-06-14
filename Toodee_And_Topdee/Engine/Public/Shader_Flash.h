#pragma once
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Flash final : public CShader
{
public:
	typedef struct tagShaderFlash_Desc {
		_float4x4		WorldMatrix;
		_float4x4		ViewMatrix;
		_float4x4		ProjMatrix;
		_float	TotalTime;
	}SHADERFLASH_DESC;

private:
	CShader_Flash(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader_Flash(const CShader_Flash& Prototype);
	virtual ~CShader_Flash() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Shader(void* pArg);


public:
	static CShader_Flash* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;;
	virtual void Free() override;
};

END