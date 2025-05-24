#pragma once

#include "Client_Defines.h"
#include "Parts.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CTransform;
END

BEGIN(Client)

class CPart_Eyes final : public Engine::CParts
{
public:
	typedef struct tagEyePart_Desc : public Engine::CParts::tagPart_Desc
	{
		class CTransform* pParentTransform;
		_float m_fTheta = { 0.f };
		_float m_fPhi = { 0.f };
	}EYEPART_DESC;

private:
	CPart_Eyes(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPart_Eyes(const CPart_Eyes& Prototype);
	virtual ~CPart_Eyes() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update() override;
	virtual void	Update(class CTransform* pTransform, _float fTheta, _float fPhi) override;
	virtual void	Late_Update() override;
	virtual HRESULT Render() override;

private:
	_float m_fTheta = {};
	_float m_fPhi	= {};

	
public:
	static CPart_Eyes* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END