#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "Body_Player.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class Obb;
class VIBuffer_Cube;
NS_END

NS_BEGIN(Client)

class CBody_Zombie final : public CPartObject
{
public:
	enum class ZOMBIE_STATE {
		AXE_STEALTH1,AXE_STEALTH2,AGGRO_IDLE1,AGGRO_IDLE2,AGGRO_TRANS1,AGGRO_TRANS2,
		LAYDOWN,ATTACK1,ATTACK2,ATTACK3,CRAWL_IDLE,CRAWL_TRANS,DEATH,DOORHIT,PATROL_IDLE,
		SEARCH_WALK,GET_UP,HIT_REACT1,HIT_REACT2,IDLE,KNOCKDOWN,PATROL_IDLE,PATROL_STOP,
		PATROL_TURN,PATROL_WALK,STAIR_BOTTOM_ENTER,STAIR_BOTTON_EXIT,STAIR_TOP_ENTER,
		STAIR_TOP_EXIT,RUN,STAIR_WALK_DOWN,STAIR_WALK_UP,STOP_WALK,UNARMED_GROUND_EXECUTE,
		UNARMED_STEALTH,WALK_FAST,END
	};
	enum class ZOMBIE_DIR {
		LEFT, RIGHT, END
	};

	typedef struct tagBodyZombieDesc : public CPartObject::PARTOBJECT_DESC
	{
		const uint32_t* pParentState = { nullptr };
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
	_bool Get_PlayerDetected() { return m_bPlayerDetected; }
	uint32_t Get_HP() { return m_iHp; }


	void Set_CurState(ZOMBIE_STATE state) { m_eCurState = state; }
	void Set_CurDir(ZOMBIE_DIR state) { m_eCurDir = state; }
	void Set_BodyAngle(_float angle) { bodyAngle = angle; }
	void Set_DirChanged() { m_bDirChanged = true; }
	void Set_Damaged() { m_bIsDamaged = true; }
	void Set_Detected(_bool flag) { m_bPlayerDetected = flag; }
	void Set_HP(uint32_t reduce) { m_iHp -= reduce; }

private:
	shared_ptr<CShader>				m_pShaderCom = { nullptr };
	shared_ptr<CModel>				m_pModelCom = { nullptr };
	shared_ptr<Obb> m_pObbCom = { nullptr };
	shared_ptr<VIBuffer_Cube> m_pObbBfCom = { nullptr };
private:
	const uint32_t* m_pParentState = { nullptr };
	unique_ptr<StateMachine<CBody_Zombie>> m_pStateMachine = nullptr;
	ZOMBIE_STATE m_eCurState = ZOMBIE_STATE::END;
	ZOMBIE_DIR   m_eCurDir = ZOMBIE_DIR::RIGHT;
	_float bodyAngle = 0.f;
	_bool  m_bDirChanged = false;
	_bool  m_bIsRotating = false;
	_bool  m_bIsDamaged  = false;
	_bool  m_bPlayerInRange = false;
	_bool  m_bPlayerDetected = false;
	_float m_fAttackTime = 0.f;
	uint32_t m_iHp = 0;
private:
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