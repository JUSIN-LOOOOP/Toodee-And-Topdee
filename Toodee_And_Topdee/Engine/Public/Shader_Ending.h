#pragma once
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Ending final : public CShader
{
public:
	typedef struct tagShaderEnding_Desc {
		_float4x4		WorldMatrix;
		_float4x4		ViewMatrix;
		_float4x4		ProjMatrix;
		_float	TotalTime;
	}SHADERENDING_DESC;

private:
	CShader_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader_Ending(const CShader_Ending& Prototype);
	virtual ~CShader_Ending() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Shader(void* pArg);


public:
	static CShader_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;;
	virtual void Free() override;
};

END