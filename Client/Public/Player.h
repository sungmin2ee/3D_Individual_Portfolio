#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "Body_Player.h"
NS_BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	typedef struct tagPlayerDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{

	}PLAYER_DESC;



private:
	CPlayer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CPlayer(const CPlayer& Prototype);
public:
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CBody_Player* Get_Body() { return body.get(); }
private:
	uint32_t			m_iState = {};

	shared_ptr<CBody_Player> body = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static unique_ptr<CPlayer> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END