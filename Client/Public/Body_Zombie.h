#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Body_Player.h"
#include "Door.h"
#include "Stair_Collider.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class Obb;
class VIBuffer_Collider;
NS_END

NS_BEGIN(Client)

class CBody_Zombie final : public CPartObject
{
public:
	enum class ZOMBIE_STATE {
		AXE_STEALTH1,AXE_STEALTH2,AGGRO_IDLE1,AGGRO_IDLE2,AGGRO_TRANS1,AGGRO_TRANS2,
		LAYDOWN,ATTACK1,ATTACK2,ATTACK3,CRAWL_IDLE,CRAWL_TRANS,DEATH,DOORHIT,FAST_PATROL_IDLE,
		SEARCH_WALK,GET_UP,HIT_REACT1,HIT_REACT2,IDLE,KNOCKDOWN,PATROL_IDLE,PATROL_STOP,
		PATROL_TURN,PATROL_WALK,STAIR_BOTTOM_ENTER,STAIR_BOTTOM_EXIT,STAIR_TOP_ENTER,
		STAIR_TOP_EXIT,RUN,STAIR_WALK_DOWN,STAIR_WALK_UP,STOP_WALK,UNARMED_GROUND_EXECUTE,
		UNARMED_STEALTH,WALK_FAST,END
	};
	enum class ZOMBIE_DIR {
		LEFT, RIGHT, FRONT, BACK, END
	};
	enum class ZOMBIE_FIRSTSTATE {
		IDLE, WALK, END
	};
	typedef struct tagBodyZombieDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState = { nullptr };
		CBody_Zombie::ZOMBIE_DIR Direction;
		CBody_Zombie::ZOMBIE_STATE State;
		CBody_Zombie::ZOMBIE_FIRSTSTATE firstState;
		_vector pos;
	}BODY_ZOMBIE_DESC;

private:
	CBody_Zombie(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CBody_Zombie(const CBody_Zombie& Prototype);
public:
	virtual ~CBody_Zombie();

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
	StateMachine<CBody_Zombie>* Get_StateMachine() { return m_pStateMachine.get(); }
	ZOMBIE_STATE Get_CurState() { return m_eCurState; }
	ZOMBIE_DIR Get_CurDir() { return m_eCurDir; }
	_float Get_BodyAngle() { return bodyAngle; }
	_bool Get_Rotating() { return m_bIsRotating; }
	_bool Get_Damaged() { return m_bIsDamaged; }
	_bool Get_PlayerInRange() { return m_bPlayerInRange; }
	_bool Get_PlayerDetected() { return m_bPlayerDetected; }
	_bool Get_DirChanged() { return m_bDirChanged; }
	_bool Is_UsingStairs() { return m_bUsingStairs; }
	_bool Is_Executing() { return m_bExecuting; }
	_bool Stealth_Death() { return m_bStealthDeath; }
	uint32_t Get_HP() { return m_iHp; }
	shared_ptr<Obb> Get_Obb() { return m_pObbCom; };
	CDoor* Get_CollidedDoor() { return pCollidedDoor; };
	CStair_Collider* Get_CollidedStair() { return pStairCollider; }
	const _float4& Get_StairPos() { return stairColliderPos; }


	void Set_CurState(ZOMBIE_STATE state) { m_eCurState = state; }
	void Set_CurDir(ZOMBIE_DIR state) { m_eCurDir = state; }
	void Set_BodyAngle(_float angle) { bodyAngle = angle; }
	void Set_DirChanged() { m_bDirChanged = true; }
	void Set_Damaged() { m_bIsDamaged = true; }
	void Set_Detected(_bool flag) { m_bPlayerDetected = flag; }
	void Set_UsingStair(_bool flag) { m_bUsingStairs = flag; }
	void Set_HP(uint32_t hp) { m_iHp += hp; }
	void Set_HPZero() { m_iHp = 0; }
	void Set_Executing(_bool flag) { m_bExecuting = flag; }
	void Set_Stealth_Death() { m_bStealthDeath = true; }
	void CheckDoorCollide();
	void Turn(_float fTimeDelta);
	void CheckStairCollide();

private:
	shared_ptr<CShader>						m_pShaderCom = { nullptr };
	shared_ptr<CModel>						m_pModelCom = { nullptr };
	shared_ptr<Obb>							m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Collider>			m_pObbBfCom = { nullptr };
private:
	const uint32_t*							m_pParentState = { nullptr };
	unique_ptr<StateMachine<CBody_Zombie>>  m_pStateMachine = nullptr;
	ZOMBIE_STATE							m_eCurState = ZOMBIE_STATE::END;
	ZOMBIE_DIR								m_eCurDir = ZOMBIE_DIR::END;
	ZOMBIE_DIR								m_ePrevDir= ZOMBIE_DIR::END;
	ZOMBIE_FIRSTSTATE						m_eFirstState = ZOMBIE_FIRSTSTATE::END;
	_float									bodyAngle = 0.f;
	_bool									m_bDirChanged = false;
	_bool									m_bIsRotating = false;
	_bool									m_bIsDamaged  = false;
	_bool									m_bPlayerInRange = false;
	_bool									m_bPlayerDetected = false;
	_bool									m_bUsingStairs = false;
	_bool									m_bExecuting = false;
	_bool									m_bStealthDeath = false;
	_bool									m_bDoorCollided = false;
	_float									m_fAttackTime = 0.f;
	uint32_t								m_iHp = 100;
	CDoor*									pCollidedDoor = nullptr;
	CStair_Collider*						pStairCollider = nullptr;
	_float4									stairColliderPos = {};

private:
	void FocusPlayer();
	//shared_ptr<CBody_Player> m_pPlayer = { nullptr };
private:
	HRESULT Ready_Components();
	void ExpandCollider();
	void DetectPlayer();
	void CheckColliding();

public:
	static unique_ptr<CBody_Zombie> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;


};

NS_END