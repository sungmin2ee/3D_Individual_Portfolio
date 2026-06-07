#pragma once

#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle_Point final : public CVIBuffer_Instance
{
public:
	typedef struct tagParticleInstanceDesc final : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_float2			vSpeed = {};
		_float2			vLifeTime = {};
		_bool			isLoop = { false };
		_float3			vPivot = {};
	}PARTICLE_INSTANCE_DESC;
protected:
	CVIBuffer_Particle_Point(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CVIBuffer_Particle_Point();

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Resources() override;
	virtual HRESULT Render() override;
	void Set_Center(_float3 vNewCenter);
public:
	void Drop(_float fTimeDelta);
	void Rise(_float fTimeDelta);
	void Spread(_float fTimeDelta);
	void Blood(_float fTimeDelta);

	void Set_Loop(_bool flag) { m_isLoop = flag; }
	void Reset() { m_Reset = true; }

private:
	shared_ptr<VTXINSTANCE_PARTICLE[]>			m_pInstanceData = { nullptr };
	shared_ptr<_float[]>						m_pSpeeds = { nullptr };
	_bool										m_isLoop = { false };
	_float3										m_vPivot = {};
	D3D11_BUFFER_DESC							m_InstanceBufferDesc = {};
	_bool										m_Reset = { false };

public:
	static unique_ptr<CVIBuffer_Particle_Point> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext, void* pArg);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END