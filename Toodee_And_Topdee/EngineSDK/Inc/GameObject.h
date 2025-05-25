#pragma once
#include "Base.h"

/* ���ӿ�����Ʈ���� �θ� �Ǵ� Ŭ���� */

/*	�ֿ� ���
			����ڰ� ������ ����������Ŭ�� ���� �Լ��� �߻�ȭ��.
			���� ������Ʈ ��ü�� ���� �纻 ������Ʈ�� ���� �� ����
*/

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	class CComponent* Get_Component(const _wstring& strComponentTag);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	map<const _wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

	_wstring name = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END