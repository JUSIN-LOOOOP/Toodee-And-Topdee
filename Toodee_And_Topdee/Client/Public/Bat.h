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
		_float	fSpeedPerSec{};
	}BAT_DESC;

public:
	enum class FLYSTATE { FLY_DOWN, FLY_UP, FLY_NON };
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
	_bool		m_bLeft{};
	_float		m_fMoveX{}, m_fSpeedPerSec{};
	_float3		m_vToodeePos{};
	

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();
	void	Render_Parts();
	void	Move_To_Target(_float fTimeDelta);
	
	void	Move_Patrol(_float fTimeDelta);


	_float3 absfloat3(const _float3& vec3) { return _float3(fabsf(vec3.x), fabsf(vec3.y), fabsf(vec3.z)); } // 벡터 절대값 만드는거


private : // Test용
	_float	MoveHeight(_float fStart, _float fEnd, _float fSecond);
	void	Key_Input(_float fTimeDelta);

	FLYSTATE	m_eState{};
	_float		m_fMaxDistance{};
	_float3		m_vTargetDir{};

	

public:
	static CBat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END