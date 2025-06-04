#pragma once

#include "Monster.h"

BEGIN(Engine)
class CParts;
END
BEGIN(Client)

class CBat final : public CMonster
{
public:
	typedef struct tagBatDesc : public MONSTER_DESC
	{
		_float3 vPosSet{};
	}BAT_DESC;

private:
	CBat(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBat(const CBat& Prototype);
	virtual ~CBat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;



private:
	map<const _wstring, CParts*>  m_vParts{};
	_bool		m_bLeft{}, m_bMotion{};
	

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	void	Render_Parts();


private : // Test¿ë
	void Move_Patrol(_float fTimeDelta);
	_float3 absfloat3(const _float3& vec3) { return _float3(fabsf(vec3.x), fabsf(vec3.y), fabsf(vec3.z)); }

	void Key_Input(_float fTimeDelta);

	

public:
	static CBat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END