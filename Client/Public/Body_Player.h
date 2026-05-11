#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "StateMachine.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class Obb;
class VIBuffer_Cube;
NS_END

NS_BEGIN(Client)

class CBody_Player final : public CPartObject
{
public:
	enum PLAYER_STATE {
		IDLE = 0x00000001,
		RUN = 0x00000002,
		ATTACK = 0x00000004,
		END = 0xffffffff
	};
	typedef struct tagBodyPlayerDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState = { nullptr };
	}BODY_PLAYER_DESC;

private:
	CBody_Player(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBody_Player(const CBody_Player& Prototype);
public:
	virtual ~CBody_Player();

public:
	const _float4x4* Get_SocketMatrixPtr(const _char* pSocketName);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	shared_ptr<CModel> Get_Model() { return m_pModelCom; }
	StateMachine<CBody_Player>* Get_StateMachine() { return m_pStateMachine.get(); }
private:
	shared_ptr<CShader>				m_pShaderCom = { nullptr };
	shared_ptr<CModel>				m_pModelCom = { nullptr };
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Cube> m_pObbBfCom = { nullptr };
private:
	const uint32_t* m_pParentState = { nullptr };
	unique_ptr<StateMachine<CBody_Player>> m_pStateMachine = nullptr;
private:
	HRESULT Ready_Components();
	void ExpandCollider();

public:
	static unique_ptr<CBody_Player> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END