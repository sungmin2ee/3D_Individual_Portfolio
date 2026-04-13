#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"
NS_BEGIN(Engine)
class Model;
class CGameObject;
class CShader;
class Obb;
class VIBuffer_Cube;
NS_END

NS_BEGIN(Client)

class Test final : public CGameObject
{
public:
	typedef struct tagPlayerDesc : public CGameObject::GAMEOBJECT_DESC
	{
		uint32_t		iData;
	}PLAYER_DESC;

private:
	Test(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~Test();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	shared_ptr<Model> m_pModelCom = { nullptr };
	shared_ptr<CShader> m_pShaderCom = { nullptr };
	MatrixBuffer cb = {};
public:
	static unique_ptr<Test> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END