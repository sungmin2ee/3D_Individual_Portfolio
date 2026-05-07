#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"
NS_BEGIN(Engine)
class CModel;
class CShader;
class Obb;
class VIBuffer_Cube;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	typedef struct tagPlayerDesc : public CGameObject::GAMEOBJECT_DESC
	{
		uint32_t		iData;
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
	//Prototype_Cube_Buffer
private:
	shared_ptr<CModel>			m_pModelCom = { nullptr };
	shared_ptr<CShader>			m_pShaderCom = { nullptr };
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Cube> m_pObbBfCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static unique_ptr<CPlayer> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END