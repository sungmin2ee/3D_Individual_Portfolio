#include "GameInstance.h"
#include "Body_Zombie.h"
#include "VIBuffer_Collider.h"
#include "Obb.h"
#include "Layer.h"
#include "Player.h"


#include "Zombie.h"
#include "Zombie_Idle.h"
#include "Zombie_Walk.h"
#include "Stair_Collider.h"

CBody_Zombie::CBody_Zombie(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_Zombie::CBody_Zombie(const CBody_Zombie& Prototype)
	: CPartObject{ Prototype }
{
	m_pStateMachine = nullptr;
	bodyAngle = 0.f;
	m_bDirChanged = false;
	m_bIsRotating = false;
	m_bIsDamaged = false;
	m_bPlayerInRange = false;
	m_bPlayerDetected = false;
	m_fAttackTime = 0.f;
	m_iHp = 40;

	pCollidedDoor = nullptr;
	pStairCollider = nullptr;
	m_bDoorCollided = false;
	m_bUsingStairs = false;
}

CBody_Zombie::~CBody_Zombie()
{
}

const _float4x4* CBody_Zombie::Get_SocketMatrixPtr(const _char* pSocketName)
{
	return m_pModelCom->Get_BoneMatrixPtr(pSocketName);

}

HRESULT CBody_Zombie::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Zombie::Initialize(void* pArg)
{
	auto	pDesc = static_cast<BODY_ZOMBIE_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);

	m_pModelCom->Calculate_Box(ETOUI(MODEL::ANIM));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CBody_Zombie));
	ExpandCollider();
	//m_pTransformCom->Set_State(STATE::POSITION, pDesc->pos);
	m_eCurState = pDesc->State;
	m_eCurDir = pDesc->Direction;
	m_ePrevDir = m_eCurDir;
	m_pZombie = pDesc->zombie;

	if (pDesc->firstState == ZOMBIE_FIRSTSTATE::IDLE) {
		m_pStateMachine = StateMachine<CBody_Zombie>::Create(this, CZombie_Idle::Create());
	}
	else {
		m_pStateMachine = StateMachine<CBody_Zombie>::Create(this, CZombie_Walk::Create());
	}
	return S_OK;
}

void CBody_Zombie::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (m_eCurState != ZOMBIE_STATE::DOORHIT) {
		CheckDoorCollide();
		CheckStairCollide();
	}
	CalculateVolume();
	
}

void CBody_Zombie::Update(_float fTimeDelta)
{
	m_pStateMachine->Update(fTimeDelta);

	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
	__super::Update(fTimeDelta);

	if (m_eCurState != ZOMBIE_STATE::DOORHIT) {
		if (m_bExecuting || m_bStealthDeath) {
			CheckColliding();

			return;
		}
		ExpandCollider();
		if (!m_bPlayerDetected) {
			DetectPlayer();
		}
		if (m_bPlayerDetected) {
			if (pCollidedDoor != nullptr) {
				m_bPlayerDetected = false;
			}
			CheckColliding();
		}

		if (m_bIsDamaged) {
			m_fAttackTime += fTimeDelta;
			if (m_fAttackTime >= 0.5f) {
				m_bIsDamaged = false;
				m_fAttackTime = 0.f;
			}
		}

		if (m_bPlayerDetected && !m_bUsingStairs) {

			if (m_eCurState != ZOMBIE_STATE::KNOCKDOWN)
				FocusPlayer();
			if (m_ePrevDir != m_eCurDir) {
				if (!m_bIsRotating) {
					m_bDirChanged = true;
					bodyAngle = 0.f;
				}

			}
		}

		if (pCollidedDoor != nullptr && !m_bDoorCollided) {
			m_pZombie.lock()->Get_Transform()->Go_Backward(fTimeDelta * 20.f);
			m_bDirChanged = true;
			m_bDoorCollided = true;

		}
		if (m_bDoorCollided && !m_bPlayerDetected) {
			if (!m_bIsRotating) {
				m_bDirChanged = true;
				m_bDoorCollided = false;
			}
		}
		if (m_bDirChanged) {
			Turn(fTimeDelta);
		}
	}

}

void CBody_Zombie::Late_Update(_float fTimeDelta)
{
	Make_CombinedWorldMatrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));
	m_pHeadMat = m_pModelCom->Get_BoneMatrixPtr("fringeHair");

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CBody_Zombie));

	__super::Late_Update(fTimeDelta);
}

HRESULT CBody_Zombie::Render()
{
	if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", CGameInstance::Get().Get_Transform(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", CGameInstance::Get().Get_Transform(D3DTS::PROJ))))
		return E_FAIL;



	uint32_t	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		m_pModelCom->Render(i);
	}

	m_pObbCom->Render();
	return S_OK;
}

HRESULT CBody_Zombie::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Zombie1")));
	if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pObbBfCom = static_pointer_cast<VIBuffer_Collider>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Collider_Buffer"));
	if (nullptr == m_pObbBfCom)
	{
		MSG_BOX("OBB 버퍼 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	m_pObbCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_OBB", &m_pObbBfCom));
	if (nullptr == m_pObbCom)
	{
		MSG_BOX("OBB 컴포넌트 클론 실패!");
		return E_FAIL;
	}
	return S_OK;
}


unique_ptr<CBody_Zombie> CBody_Zombie::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBody_Zombie>(new CBody_Zombie(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Zombie");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBody_Zombie::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBody_Zombie(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Zombie");
		return nullptr;
	}

	return pInstance;
}

void CBody_Zombie::ExpandCollider()
{
	const _float4x4* boneMat = m_pModelCom->Get_BoneMatrixPtr("backpack2");
	const _float4x4* boneMat1 = m_pModelCom->Get_BoneMatrixPtr("rightPinky1");
	const _float4x4* boneMat2 = m_pModelCom->Get_BoneMatrixPtr("leftPinky1");
	const _float4x4* boneMat3 = m_pModelCom->Get_BoneMatrixPtr("fringeHair");
	const _float4x4* boneMat4 = m_pModelCom->Get_BoneMatrixPtr("toes_R");
	const _float4x4* boneMat5 = m_pModelCom->Get_BoneMatrixPtr("toes_L");

	vector<const _float4x4*> bones = { boneMat, boneMat1, boneMat2, boneMat3, boneMat4, boneMat5 };

	// 로컬(모델) 공간에서의 최소/최대 값을 구합니다.
	_float3 vLocalMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	_float3 vLocalMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (auto bone : bones)
	{
		if (!bone) continue;

		_matrix boneMatrix = XMLoadFloat4x4(bone);
		XMVECTOR localPos = boneMatrix.r[3];

		_float3 p;
		XMStoreFloat3(&p, localPos);

		vLocalMin.x = min(vLocalMin.x, p.x);
		vLocalMin.y = min(vLocalMin.y, p.y);
		vLocalMin.z = min(vLocalMin.z, p.z);

		vLocalMax.x = max(vLocalMax.x, p.x);
		vLocalMax.y = max(vLocalMax.y, p.y);
		vLocalMax.z = max(vLocalMax.z, p.z);
	}

	// 1. 자신의 월드 행렬 계산
	_matrix myWorld = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr());

	// 2. 부모의 행렬이 존재한다면 결합 (핵심 추가 변경 사항)
	_matrix combinedWorld = myWorld;
	if (m_pParentMatrix != nullptr)
	{
		_matrix parentWorld = XMLoadFloat4x4(m_pParentMatrix);
		combinedWorld = myWorld * parentWorld; // 행렬 곱셈 순서는 엔진 구조(행우선/열우선)에 맞춰 확인 필요
	}

	// 3. 결합된 최종 월드 행렬을 분해하여 최종 스케일, 회전, 위치를 추출
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, combinedWorld);

	_float3 finalScale;
	XMStoreFloat3(&finalScale, vScale);

	// 4. Center 계산: 로컬 중심점을 구한 뒤, '최종 결합 행렬'로 월드 변환
	XMVECTOR localMid = (XMLoadFloat3(&vLocalMax) + XMLoadFloat3(&vLocalMin)) * 0.5f;
	XMVECTOR centerWorld = XMVector3TransformCoord(localMid, combinedWorld);
	XMStoreFloat3(&m_pObbCom->myOBB.Center, centerWorld);

	// 5. Extents 계산: 로컬 크기에 부모까지 반영된 '최종 스케일'을 곱해줍니다.
	m_pObbCom->myOBB.Extents.x = (vLocalMax.x - vLocalMin.x) * 0.6f * finalScale.x;
	m_pObbCom->myOBB.Extents.y = (vLocalMax.y - vLocalMin.y) * 0.6f * finalScale.y;
	m_pObbCom->myOBB.Extents.z = (vLocalMax.z - vLocalMin.z) * 0.6f * finalScale.z * 0.9f;

	// 6. Orientation 설정
	vRot = XMQuaternionNormalize(vRot);
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 7. 렌더링용 OBB 월드 행렬 생성
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x * 2.f,
		m_pObbCom->myOBB.Extents.y * 2.f,
		m_pObbCom->myOBB.Extents.z * 2.f);

	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(centerWorld);

	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}

void CBody_Zombie::DetectPlayer()
{
	//플레이어가 소리를 냈고 플레이어의 x가 좀비의 x 플마 1? 정도에 있으면 detect는 true
	//m_bPlayerDetected = false;
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return;
	auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
	if (playerBody == nullptr)
		return;
	_vector playerPos = player->Get_Transform()->Get_State(STATE::POSITION);
	_vector deltaPos = m_pZombie.lock()->Get_Transform()->Get_State(STATE::POSITION) - playerPos;
	_float4 fpos;
	XMStoreFloat4(&fpos, deltaPos);
	
	//if (fabs(fpos.x) <= 0.5f && playerBody->Is_MakingSound()) {
	//	if(fpos.y == 0)
	//		m_bPlayerDetected = true;
	//}
	//else {
	//	m_bPlayerDetected = false;
	//}

	if (fabs(fpos.x) <= 0.5f&& fpos.y == 0) {
		if (playerBody->Is_MakingSound()) {
			m_bPlayerDetected = true;
			CGameInstance::Get().PlaySoundOne(L"detect.wav", CHANNELID::SOUND_EFFECT_ZOMBIE, 1.f);

		}
		else if (ETOUI(playerBody->Get_CurDir()) != ETOUI(m_eCurDir) && playerBody->Is_MakingSound()) {
			m_bPlayerDetected = true;

		}
	}

	
}

void CBody_Zombie::CheckColliding()
{
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return;
	auto playerBody = static_pointer_cast<CPlayer>(player)->Get_Body();
	if (playerBody == nullptr)
		return;
	if (m_pObbCom->myOBB.Intersects(playerBody->Get_Obb()->myOBB)) {
		m_bPlayerInRange = true;
		m_pObbCom->SetSelected(true);
		playerBody->Get_Obb()->SetSelected(true);
	}
	else {
		m_bPlayerInRange = false;
		m_pObbCom->SetSelected(false);
		playerBody->Get_Obb()->SetSelected(false);
	}
	
}

void CBody_Zombie::FocusPlayer() {

	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return;

	auto playerPos = player->Get_Transform()->Get_State(STATE::POSITION);

	_float4 fPlayerPos; 
	XMStoreFloat4(&fPlayerPos,playerPos);
	_float4 myPos;
	XMStoreFloat4(&myPos, m_pZombie.lock()->Get_Transform()->Get_State(STATE::POSITION));
	if (m_eCurDir == ZOMBIE_DIR::FRONT) {
		if (fabs(myPos.z - fPlayerPos.z) < 0.05f) {
			if (myPos.x - fPlayerPos.x > 0) {
				m_eCurDir = ZOMBIE_DIR::LEFT;
				m_bDirChanged = true;
				m_pZombie.lock()->Get_Transform()->Set_State(STATE::POSITION, XMVectorSetZ(m_pZombie.lock()->Get_Transform()->Get_State(STATE::POSITION), 0));
			}
			else if (myPos.x - fPlayerPos.x < 0) {
				m_eCurDir = ZOMBIE_DIR::RIGHT;
				m_bDirChanged = true;
			}
			else {

			}
		}
	}else if (m_eCurDir == ZOMBIE_DIR::BACK) {
		m_eCurDir = ZOMBIE_DIR::FRONT;
		m_bDirChanged = true;

	}
	else {
		if (myPos.x - fPlayerPos.x > 0) {
			m_eCurDir = ZOMBIE_DIR::LEFT;
		}
		else if (myPos.x - fPlayerPos.x < 0) {
			m_eCurDir = ZOMBIE_DIR::RIGHT;
		}
	}

}
void CBody_Zombie::CalculateVolume()
{
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(CGameInstance::Get().GetCurLevelIndex()), TEXT("Layer_Player"));
	if (layer == nullptr)
		return;
	auto player = layer->GetObjectFirst();
	if (player == nullptr)
		return;

	auto playerPos = player->Get_Transform()->Get_State(STATE::POSITION);
	_vector myPos = m_pZombie.lock()->Get_Transform()->Get_State(STATE::POSITION);

	_vector vDiff = playerPos - myPos;
	_float4 fdiff;
	XMStoreFloat4(&fdiff, vDiff);
	if (fabs(fdiff.y) > 0.25f) {
		m_fVolume = 0.f;
		return;
	}
	_float distance = XMVectorGetX(XMVector3Length(vDiff));

	const float maxDistance = 0.8f;

	float volume = 1.0f - (distance / maxDistance);
	m_fVolume = std::clamp(volume, 0.0f, 1.0f);

}
void CBody_Zombie::CheckDoorCollide()
{
	auto DoorLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Door"));
	if (DoorLayer == nullptr)
		return;

	auto doors = DoorLayer->GetObjects();
	pCollidedDoor = nullptr;

	for (auto& pDoorObj : doors)
	{
		auto pDoor = static_pointer_cast<CDoor>(pDoorObj);
		if (pDoor == nullptr) continue;

		if (m_pObbCom->myOBB.Intersects(pDoor->Get_Obb()->myOBB))
		{
			_float4 myPos;
			_float3 doorPos;

			XMStoreFloat4(&myPos, m_pZombie.lock()->Get_Transform()->Get_State(STATE::POSITION));
			//XMStoreFloat3(&doorPos, pDoor->Get_Obb()->myOBB.Center);
			if (fabs(myPos.x - pDoor->Get_Obb()->myOBB.Center.x) < 0.03f) {
				pCollidedDoor = pDoor.get(); // 충돌한 문의 주소값 저장
				break; // 찾았으니 다른 문은 더 돌 필요 없이 루프 탈출!

			}
		}
	}


}
void CBody_Zombie::CheckStairCollide()
{
	auto stairLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Stair_Collider"));
	if (stairLayer == nullptr)
		return;

	auto stairs = stairLayer->GetObjects();
	pStairCollider = nullptr;

	for (auto& pStairObj : stairs)
	{
		auto pStair = static_pointer_cast<CStair_Collider>(pStairObj);
		if (pStair == nullptr) continue;

		if (m_pObbCom->myOBB.Intersects(pStair->Get_Obb()->myOBB))
		{
			pStairCollider = pStair.get();
			XMStoreFloat4(&stairColliderPos, pStair->Get_Transform()->Get_State(STATE::POSITION));

			break; // 찾았으니 다른 문은 더 돌 필요 없이 루프 탈출!
		}
	}


}

void CBody_Zombie::Turn(_float fTimeDelta)
{
	m_bIsRotating = true;
	bodyAngle += fTimeDelta * m_pZombie.lock()->Get_Transform()->Get_RotSpeed();

	if (m_ePrevDir == ZOMBIE_DIR::LEFT) {


		m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), -90.f + bodyAngle);

		if (bodyAngle >= 180.f) {
		
			m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);

			m_bDirChanged = false;
			m_bIsRotating = false;
			bodyAngle = 0.f;
			m_ePrevDir = ZOMBIE_DIR::RIGHT; 
		}
	}
	else if (m_ePrevDir == ZOMBIE_DIR::RIGHT) {
		m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f + bodyAngle);

		if (bodyAngle >= 180.f) {
			m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 270.f);

			m_bDirChanged = false;
			m_bIsRotating = false;
			bodyAngle = 0.f;
			m_ePrevDir = ZOMBIE_DIR::LEFT; 
		}
	}
	else if (m_ePrevDir == ZOMBIE_DIR::FRONT) {
		if (m_eCurDir == ZOMBIE_DIR::LEFT) {
			m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f + bodyAngle);

			if (bodyAngle >= 90.f) {
				m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 270.f);

				m_bDirChanged = false;
				m_bIsRotating = false;
				bodyAngle = 0.f;
				m_ePrevDir = ZOMBIE_DIR::LEFT;
			}
		}else if (m_eCurDir == ZOMBIE_DIR::RIGHT) {
			m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f - bodyAngle);
			if (bodyAngle >= 90.f) {
				m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
				m_bDirChanged = false;
				m_bIsRotating = false;
				bodyAngle = 0.f;
				m_ePrevDir = ZOMBIE_DIR::RIGHT;
			}
		}
	
	}
	else if (m_ePrevDir == ZOMBIE_DIR::BACK) {
		m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);

		if (bodyAngle >= 180.f) {
			m_pZombie.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f);
			m_bDirChanged = false;
			m_bIsRotating = false;
			bodyAngle = 0.f;
			m_ePrevDir = ZOMBIE_DIR::FRONT; 
		}
	}
}