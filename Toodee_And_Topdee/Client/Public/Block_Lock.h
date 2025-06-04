#pragma once
#include "Client_Defines.h"
#include "SubjectObject.h"
#include "Block.h"

BEGIN(Client)

class CBlock_Lock final : public CBlock, public CSubjectObject
{
private:
	CBlock_Lock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Lock(const CBlock_Lock& Prototype);
	virtual ~CBlock_Lock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void onReport(REPORT eReport, CSubjectObject* pSubject) override;

private: 
	HRESULT Ready_Components();
	HRESULT Ready_Observer();
	void	SetUp_RenderState();
	void	Reset_RenderState();


public:
	static CBlock_Lock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

