#pragma once
#include "Client_Defines.h"

#include "BlendObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackRock : public CBlendObject
{
public :
	typedef struct tagBackObjectDesc
	{
		_float3 _pos;
		_float3  _size;
		_uint  _textureIdx;
	} BACK_DESC;

private:
	CBackRock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBackRock(const CBackRock& Prototype);
	virtual ~CBackRock() = default;

public:
	virtual					HRESULT Initialize_Prototype() override;
	virtual					HRESULT Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual					HRESULT Render() override;

private: 
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	BACK_DESC				m_Info;

	_float	m_fDir = 1.f;
	_float	m_fMoveTime = 0.f;
	_float	m_fSpeed = 1.f;

	HRESULT					Ready_Components();
	HRESULT					Begin_RenderState();
	HRESULT					End_RenderState();

public:
	static CBackRock*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CBackRock*		Clone(void* pArg) override;
	virtual void			Free() override;

};

END