#pragma once

#include "Component.h"

/* 객체의 모델을 월드스페이스로 변환해주는 월드행렬을 보관한다. */
/* -> 월드행렬의 상태를 바꿔주면 -> 월드상에 그려지는 객체의 상태도 바뀐다. */
/* -> 월드공간에서의 객체의 상태 변환을 위한 기능(월드변환->이동, 회전, 크기변환) 을 보유한다. */

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = {};
		_float		fRotationPerSec = {};
	}TRANSFORM_DESC;

private:
	CTransform(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CTransform();

public:
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4(reinterpret_cast<const _float4*>(&m_WorldMatrix.m[ETOUI(eState)][0]));
	}

	_float3 Get_Scaled();

	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[ETOUI(eState)][0]), vState);
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);


public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);

	void Rotation(_fvector vAxis, _float fAngle);
	void Turn(_fvector vAxis, _float fTimeDelta);

	void LookAt(_fvector vAt);
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);
	void Chase(_fvector vGoal, _float fTimeDelta, _float fLimit);
	_float4x4 GetWorld() { return m_WorldMatrix; }
	XMMATRIX GetWorldMat() { return XMLoadFloat4x4(&m_WorldMatrix); }
	void SetWorld(_float4x4 world) {m_WorldMatrix = world; }

private:
	_float			m_fSpeedPerSec = {};
	_float			m_fRotationPerSec = {};

	_float4x4		m_WorldMatrix = {};

public:
	static shared_ptr<CTransform> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END