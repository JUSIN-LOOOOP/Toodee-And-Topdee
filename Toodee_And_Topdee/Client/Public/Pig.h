#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CParts;
class CCollider;
END

BEGIN(Client)

class CPig final : public CMonster
{
public:
	typedef struct tagPigDesc : public MONSTER_DESC
	{
		_float3 vPosSet{};
	}PIG_DESC;

private:
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
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

private:
	map<const _wstring, CParts*>  m_vParts;
	_bool		m_bLeft{};
	_bool		m_bMotion{};

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	void	Render_Parts();
	void	Parts_Update(_float fTimeDelta);

private : // Test¿ë
	void Move_Patrol(_float fTimeDelta);
	_float m_fMaxPat{}, m_fPatrol{};
	_float3 m_vScale{};

public:
	static CPig* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END