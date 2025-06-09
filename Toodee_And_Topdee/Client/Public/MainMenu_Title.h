#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CMainMenu_Title final : public CGameObject
{
private:
	CMainMenu_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_Title(const CMainMenu_Title& Prototype);
	virtual ~CMainMenu_Title() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float						m_fMotionSize = { 0.f };
	_float						m_bMotionSize = { false };
	_float						m_fMotionPosition = { 0.f };
	_float						m_bMotionPosition = { false };
	_float						m_fMotionIntervalTime = { 0.1f };
	_float						m_fAccumulateMotionTime = { 0.f };

// �⺻ ��������
	//���� �ؽ��� �ȼ���ġ 
	_float				m_fTextureHeight_InPixel = { 64.f }; //�ؽ��� ��ü ����
	_float				m_fTextTopMargin_InPixel = { 17.f };
	_float				m_fTextBottomPos_InPixel = { 45.f };
	_float				m_fShadowTopMargin_InPixel = { 28.f };
	_float				m_fShadowBottomPos_InPixel = { 34.f };

	_float				fTextHeight = { 10.f };      // �ؽ�Ʈ ����
	_float				fTextWidth = { 30.f };;      // �ؽ�Ʈ �ʺ�
	_float				fAndSize = { 16.f };         // AND �ؽ�Ʈ ũ��
	_float				fSpacing = { 0.f };         // �ؽ�Ʈ �� ����

	_float				fToodeeZ = { 15.f };
	_float				fAndZ = { 11.f };
	_float				fTopdeeZ = { 10.f };

	_float				fBaseY = { 2.f };



private:
	_float3				RotationEndPoint(_float fRadian, _float fDistance, _float3 vPostion);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				Change_Motion(_float fTimeDelta);


public:
	static CMainMenu_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END