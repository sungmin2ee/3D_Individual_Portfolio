#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CBackGround final : public CGameObject
{
public:
	typedef struct tagBackGroundDesc : public CGameObject::GAMEOBJECT_DESC
	{
		uint32_t		iData;
	}BACKGROUND_DESC;

private:
	CBackGround(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBackGround(const CBackGround& Prototype);
public:
	virtual ~CBackGround();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	uint32_t			m_iData = {};

public:
	static unique_ptr<CBackGround> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END