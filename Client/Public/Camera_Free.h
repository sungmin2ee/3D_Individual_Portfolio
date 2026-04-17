#pragma once

#include "Client_Defines.h"
#include "ECamera.h"

NS_BEGIN(Engine)

NS_END


NS_BEGIN(Client)

class CCamera_Free final : public ECamera
{
public:
	typedef struct tagCameraFreeDesc : public ECamera::CAMERA_DESC
	{
		_float		fMouseSensor;
	}CAMERA_FREE_DESC;

private:
	CCamera_Free(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CCamera_Free(const CCamera_Free& Prototype);
public:
	virtual ~CCamera_Free();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float		m_fMouseSensor = {};

private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CCamera_Free> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END