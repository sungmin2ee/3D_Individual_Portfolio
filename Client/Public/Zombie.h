#pragma once

#include "Client_Defines.h"
#include "Body_Zombie.h"
#include "ContainerObject.h"
#include "Blood.h"

NS_BEGIN(Client)

class CZombie final : public CContainerObject
{
public:
	typedef struct tagZombieDesc : public CContainerObject::CONTAINEROBJECT_DESC
	{
		CBody_Zombie::ZOMBIE_DIR Direction;
		CBody_Zombie::ZOMBIE_STATE State;
		CBody_Zombie::ZOMBIE_FIRSTSTATE firstState;
		_vector pos;
		LEVEL  nextLevel;

	}ZOMBIE_DESC;



private:
	CZombie(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CZombie(const CZombie& Prototype);
public:
	virtual ~CZombie();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	CBody_Zombie* Get_Body() { return body.get(); }
	CBlood* Get_Effect() { return efftct.get(); }

private:
	shared_ptr<CBody_Zombie> body = nullptr;
	shared_ptr<CBlood>		efftct = nullptr;
	uint32_t			m_iState = {};
	
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects(void* pArg);

public:
	static unique_ptr<CZombie> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END