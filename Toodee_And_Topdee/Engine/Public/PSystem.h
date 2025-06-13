#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPSystem abstract : public CBase
{
protected :
	struct BoundingBox
	{
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
		D3DXVECTOR3 _center;


		BoundingBox() : _min(0, 0, 0), _max(0, 0, 0), _center(0, 0, 0) {}
		BoundingBox(const D3DXVECTOR3& minPt, const D3DXVECTOR3& maxPt)
			: _min(minPt), _max(maxPt) {
		}

		bool isPointInside(D3DXVECTOR3& p) {
			if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
				p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
				return true;
			else
				return false;
		}
	};

	typedef struct Attribute
	{
		D3DXVECTOR3 _position;
		D3DXVECTOR3 _velocity;
		D3DXCOLOR	_color;
		BoundingBox	_BoundingBox;
		_float		_FrameTime = 0.f;
		_uint		_TextureIdx = 0.f;
		_bool		_isAlive;
		_float		_size = 1.f;
		_float		_Speed = 1.f;
	}PARTICLE;

protected:
	CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPSystem(const CPSystem& Prototype);
	virtual ~CPSystem() = default;

public:
	virtual HRESULT Initialize();
	virtual void Reset(void* pArg = nullptr);
	virtual void Stop(void* pArg = nullptr);
	virtual void ResetParticle(PARTICLE* attribute, void* pArg = nullptr) = 0;
	virtual void AddParticle();

	virtual void Update(_float fTimeDelta) = 0;
	virtual void PreRender();
	virtual HRESULT Render();
	virtual void postRender();

	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticles();

protected:
	class CGameInstance* m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9					m_pGraphic_Device;
	BoundingBox							m_BoundingBox;
	_float								m_fSize;
	_float								m_fSizeRatio = 1.f;
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	list<PARTICLE>						m_Particles;
	_uint								m_iFVF;
	_uint								m_iChunk = 0;
	_float4x4							worldMatrix;

	IDirect3DBaseTexture9*				m_Texture = { nullptr };
	_uint								m_iNumTextures = 0;
	_float								m_fFrameOffsetTime;

	_uint								m_iNumVertices = { };	/* 정점의 갯수 */
	_uint								m_iVertexStride = { }; /* 정점하나의 크기(Byte) */
	D3DPRIMITIVETYPE					m_ePrimitiveType = {};	/* 어떤 방식으로 그릴 것인지 */
	_uint								m_iNumPrimitive = {};	/* 기본형의 갯수  */

	DWORD vbSize;
	DWORD vbOffset;
	DWORD vbBatchSize;

public:
	virtual void Free() override;

};

END