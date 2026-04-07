#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"
NS_BEGIN(Engine)
class Model;
class CGameObject;
class Shader;
class Obb;
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
public:
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<Model> m_pModelCom = { nullptr };
	shared_ptr<Shader> m_pShaderCom = { nullptr };
	shared_ptr<Obb> m_pObbCom = { nullptr };
	uint32_t			m_iData = {};
	MatrixBuffer cb;
	ComPtr<ID3D11Buffer>							m_pConstantBuffer = { nullptr };
	ComPtr<ID3D11Buffer>							m_pBoneBuffer = { nullptr };
	ComPtr<ID3D11VertexShader>						m_pVS = { nullptr };
	ComPtr<ID3D11PixelShader>						m_pPS = { nullptr };
	ComPtr<ID3D11InputLayout>						m_pLayout = { nullptr };
	ComPtr<ID3D11SamplerState>						m_pSamplerState = { nullptr };
	ComPtr<ID3D11Device>							m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>						m_pContext = { nullptr };

	ImGuizmo::OPERATION m_CurrentGizmoOperation;
public:
	static unique_ptr<CPlayer> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END