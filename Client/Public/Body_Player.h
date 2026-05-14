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
	enum class PLAYER_STATE {
		STAND,SIT,STAIR_UP, STAIR_DOWN,END
	};
	enum class PLAYER_DIR {
		LEFT, RIGHT, END
	};
	enum class PLAYER_WEAPON {
		HAND, AXE, END
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
	PLAYER_STATE Get_CurState() { return m_eCurState; }
	PLAYER_DIR Get_CurDir() { return m_eCurDir; }
	PLAYER_WEAPON Get_Weapon() { return m_eCurWeapon; }
	_float Get_BodyAngle() { return bodyAngle; }
	_bool Get_Rotating() { return m_bIsRotating; }
	void Set_CurState(PLAYER_STATE state) { m_eCurState = state; }
	void Set_CurDir(PLAYER_DIR state) { m_eCurDir = state; }
	void Set_BodyAngle(_float angle) { bodyAngle = angle; }
	void Set_DirChanged() { m_bDirChanged = true; }
private:
	shared_ptr<CShader>				m_pShaderCom = { nullptr };
	shared_ptr<CModel>				m_pModelCom = { nullptr };
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Cube> m_pObbBfCom = { nullptr };
private:
	const uint32_t* m_pParentState = { nullptr };
	unique_ptr<StateMachine<CBody_Player>> m_pStateMachine = nullptr;
	PLAYER_STATE m_eCurState = PLAYER_STATE::END;
	PLAYER_DIR   m_eCurDir = PLAYER_DIR::RIGHT;
	PLAYER_WEAPON   m_eCurWeapon = PLAYER_WEAPON::HAND;
	_float bodyAngle = 0.f;
	_bool  m_bDirChanged = false;
	_bool  m_bIsRotating = false;
private:
	HRESULT Ready_Components();
	void ExpandCollider();

public:
	static unique_ptr<CBody_Player> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;


};

NS_END