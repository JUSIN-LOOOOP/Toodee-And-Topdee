#pragma once
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Water final : public CShader
{
public:
	typedef struct tagShaderWater_Desc {
		_float4x4	WorldMatrix;
		_float4x4	ViewMatrix;
		_float4x4	ProjMatrix;
		_float		fFillHeight;
		_float3		vSinkPosition;
		_float		fSinkPower;
		_float		fTimeDelta;
		_float		fSpeed;
	}SHADERWATER_DESC;

private:
	CShader_Water(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader_Water(const CShader_Water& Prototype);
	virtual ~CShader_Water() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Shader(void* pArg);


public:
	static CShader_Water* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;;
	virtual void Free() override;
};

END