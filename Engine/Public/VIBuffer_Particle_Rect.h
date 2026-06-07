#pragma once

#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle_Rect final : public CVIBuffer_Instance
{
public:
	typedef struct tagParticleInstanceDesc final : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_float2			vSpeed = {};
		_float2			vLifeTime = {};
		_bool			isLoop = { false };
	}PARTICLE_INSTANCE_DESC;
protected:
	CVIBuffer_Particle_Rect(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CVIBuffer_Particle_Rect();

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Drop(_float fTimeDelta);


private:
	shared_ptr<VTXINSTANCE_PARTICLE[]>			m_pInstanceData = { nullptr };
	shared_ptr<_float[]>						m_pSpeeds = { nullptr };
	_bool										m_isLoop = { false };

public:
	static unique_ptr<CVIBuffer_Particle_Rect> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext, void* pArg);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END