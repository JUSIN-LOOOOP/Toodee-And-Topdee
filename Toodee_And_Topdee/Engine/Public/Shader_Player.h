#pragma once
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Player final : public CShader
{
public:
	typedef struct tagShaderPlayer_Desc {
		_float4x4	WorldMatrix;
		_float4x4	ViewMatrix;
		_float4x4	ProjMatrix;
	}SHADERPLAYER_DESC;

private:
	CShader_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader_Player(const CShader_Player& Prototype);
	virtual ~CShader_Player() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Shader(void* pArg);


public:
	static CShader_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;;
	virtual void Free() override;
};

END