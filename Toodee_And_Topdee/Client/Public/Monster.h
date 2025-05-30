#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END


BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc
	{
		CTransform* TargetTransformCom{};

	}MONSTER_DESC;

protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	CTransform* m_pTargetTransformCom = { nullptr };

	_float	m_fMoveX{}, m_fMoveZ{}, m_fMoveY{};
	_float3	m_vFocusTargetPos{};

protected:
	void Move_To_Target(CTransform* pTransform, _float fTimeDelta);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() ;

};

END