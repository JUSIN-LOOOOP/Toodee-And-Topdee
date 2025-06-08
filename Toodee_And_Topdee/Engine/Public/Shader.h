#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader abstract : public CComponent
{
protected:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:	
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath) PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual HRESULT Bind_Shader(void* pArg);
	HRESULT Reset_Shader();


protected:
	IDirect3DVertexShader9*			m_pVertexShader = { nullptr };
	IDirect3DPixelShader9*			m_pPixelShader = { nullptr };
	ID3DXConstantTable*				m_pVSConst = { nullptr };
	ID3DXConstantTable*				m_pPSConst = { nullptr };
public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END