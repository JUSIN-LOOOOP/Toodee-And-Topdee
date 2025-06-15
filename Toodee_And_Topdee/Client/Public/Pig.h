#pragma once

#include "Monster.h"

BEGIN(Engine)
class CParts;
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
	map<const _wstring, CParts*>  m_vParts{};
	_bool		m_bLeft{}, m_bGravity{};
	_float3		m_vColliderScale{};
	_uint		m_iPlayLevel = {};
	_float3		m_vOldLook{};

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	void	Render_Parts();

	_bool	Check_Gravity(_float fTimeDelta);
	void	Compute_Collision(_float3 vDir = _float3(0.f,0.f,0.f));

private : // Test용
	void Move_Patrol(_float fTimeDelta);
	
	_float3 absfloat3(const _float3& vec3) { return _float3(fabsf(vec3.x), fabsf(vec3.y), fabsf(vec3.z)); }

	_float3 Move_To_Target(_float fTimeDelta);
	

public:
	static CPig* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END