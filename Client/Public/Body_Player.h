#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Door.h"
#include "Stair_Collider.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CBody_Player final : public CPartObject
{
public:
	enum class PLAYER_STATE {
		STAND,SIT,STAIR_UP, STAIR_DOWN,ATTACK,END
	};
	enum class PLAYER_ANIM {
		AXE_STEALTH1, AXE_STEALTH2, AXE_ATTACK1, AXE_ATTACK2, DOOR_OPEN_STAND,
		DOOR_OPEN_SNEAK, DOOR_PEEK_SNEAK_IDLE, DOOR_CLOSE_PEEK, DOOR_OPEN_PEEK,
		DOOR_PULL_CLOSE_SNEAK, DOOR_PULL_CLOSE_STAND,HIT_REACT_BACK,HIT_REACT_FRONT, IDLE, IDLE_TO_RUN, STAIR_BOTTOM_ENTER,
		STAIR_BOTTOM_EXIT, STAIR_TOP_ENTER, STAIR_TOP_EXIT, RUN_MED, RUN_STOP, SNEAK_IDLE,
		SNEAK_IDLE_TRANSITION, SNEAK_WALK, SNEAK_WALK_START, STAIR_IDLE_DOWN,
		STAIR_IDLE_UP, STAIR_RUN_DOWN, STAIR_RUN_UP, STAIR_SNEAK_WALKDOWN,
		STAIR_SNEAK_WALKUP, UNARMED_ATTACK1, UNARMED_ATTACK2, GROUND_EXECUTE1, GROUND_EXECUTE2,
		WALK, WALKSTOP, STEALTH_KILL, END
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
		shared_ptr<class CPlayer> player;
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
	_float Get_HP() { return m_fHp; }
	_bool Get_Rotating() { return m_bIsRotating; }
	_bool Is_MakingSound() { return m_bIsMakingSound; }
	_bool Get_OnHit() { return m_bOnHit; }
	shared_ptr<Obb> Get_Obb() { return m_pObbCom; };
	CDoor* Get_CollidedDoor() { return pCollidedDoor; };
	CStair_Collider* Get_CollidedStair() { return pStairCollider; }
	const _float4& Get_StairPos() { return stairColliderPos; }
	weak_ptr<class CPlayer> Get_Player() { return m_pPlayer.lock(); }

	void Set_CurState(PLAYER_STATE state) { m_eCurState = state; }
	void Set_Rotating(_bool flag) { m_bIsRotating = flag; }
	void Set_CurDir(PLAYER_DIR state) { m_eCurDir = state; }
	void Set_BodyAngle(_float angle) { bodyAngle = angle; }
	void Set_DirChanged() { m_bDirChanged = true; }
	void Set_MakingSound(_bool flag) { m_bIsMakingSound = flag; }
	void Set_OnHit(_bool flag) { m_bOnHit= flag; }
	void Set_StairMove(_bool flag) { m_bStairMove = flag; }
	void Set_Hp(_float delta) { m_fHp += delta; }

private:
	shared_ptr<CShader>				m_pShaderCom = { nullptr };
	shared_ptr<CModel>				m_pModelCom = { nullptr };
	shared_ptr<Obb>					m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>		m_pObbBfCom = { nullptr };
private:
	const uint32_t*								m_pParentState = { nullptr };
	unique_ptr<StateMachine<CBody_Player>>		m_pStateMachine = nullptr;
	PLAYER_STATE								m_eCurState = PLAYER_STATE::STAND;
	PLAYER_DIR									m_eCurDir = PLAYER_DIR::RIGHT;
	PLAYER_WEAPON								m_eCurWeapon = PLAYER_WEAPON::AXE;
	_float										bodyAngle = 0.f;
	_bool										m_bDirChanged = false;
	_bool										m_bIsRotating = false;
	_bool										m_bIsMakingSound = false;
	_bool										m_bOnHit = false;
	_bool										m_bStairMove = false;
	_float										m_fHp = 100.f;
	_float										m_fOnHitTime = 0.f;
	 CDoor*										pCollidedDoor = nullptr;
	 CStair_Collider*							pStairCollider = nullptr;
	_float4										stairColliderPos = {};
	weak_ptr<class CPlayer>							m_pPlayer;
public:
	void Execute();
	void CheckDoorCollide();
	void CheckStairCollide();
private:
	HRESULT Ready_Components();
	void ExpandCollider();


public:
	static unique_ptr<CBody_Player> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;


};

NS_END