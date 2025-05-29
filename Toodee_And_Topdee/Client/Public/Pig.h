#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CParts;
END

BEGIN(Client)

class CPig : public CGameObject
{
protected:
	CPig(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPig(const CPig& Prototype);
	virtual ~CPig() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

private:
	map<const _wstring, CParts*>  m_vParts;
	_float m_fWidth{}, m_fHeight{};		//X,Z축 이동에 대한 값


private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	void Render_Parts();

private : // Test용
	void Test_KeyInput(_float fTimeDelta);
	void Move_Patrol(_float fTimeDelta);
	_float m_fMaxPat{}, m_fPatrol{};


public:
	static CPig* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END