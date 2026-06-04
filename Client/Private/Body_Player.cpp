#include "Body_Player.h"
#include "Player.h"
#include "VIBuffer_Collider.h"
#include "Obb.h"
#include "Player_Idle.h"
#include "Player_Stair.h"
#include "Player_Execute.h"
#include "GameInstance.h"
#include "Stair_Collider.h"
#include "Player_StealthAttack.h"
#include "Zombie.h"
#include "Layer.h"
//#include "Door.h"
#include "Blocker.h"
#include "Search_Collider.h"
#include "UIObject.h"
#include "SearchBox.h"
#include "Inventory.h"
#include "Overlay.h"

CBody_Player::CBody_Player(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
	: CPartObject{ Prototype }
{
}

CBody_Player::~CBody_Player()
{
}

const _float4x4* CBody_Player::Get_SocketMatrixPtr(const _char* pSocketName)
{
	return m_pModelCom->Get_BoneMatrixPtr(pSocketName);

}

HRESULT CBody_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
	auto	pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);

	m_pParentState = pDesc->pParentState;
	m_pParentMatrix = pDesc->pParentMatrix;
	pDesc->fRotationPerSec = 720.f;
	m_pPlayer = pDesc->player;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	m_pStateMachine = StateMachine<CBody_Player>::Create(this, CPlayer_Idle::Create());
	m_pModelCom->Calculate_Box(ETOUI(MODEL::ANIM));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CBody_Player));
	ExpandCollider();
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-1.5f, 0, 0, 1));
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0, 0, 1));
	auto layer = CGameInstance::Get().Find_Layer(ETOUI(pDesc->nextLevel), TEXT("Layer_Inventory"));
	m_pInven = static_pointer_cast<CInventory>(layer->GetObjectFirst());
	return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
	
	__super::Priority_Update(fTimeDelta);
	if (!m_pInven->Get_Render()) {
		if (m_bIsRotating) {
			if (m_bDirChanged && m_eCurState != PLAYER_STATE::ATTACK) {
				if (m_eCurDir == CBody_Player::PLAYER_DIR::RIGHT) {
					bodyAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
					//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);
					m_pPlayer.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);

					if (bodyAngle >= 180.f) {
						//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 270.f);
						m_pPlayer.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 270.f);
						m_bDirChanged = false;
						m_bIsRotating = false;
						bodyAngle = 180.f;
						m_eCurDir = PLAYER_DIR::LEFT;
						return;

					}
				}
				else if (m_eCurDir == CBody_Player::PLAYER_DIR::LEFT) {
					bodyAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
					//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);
					m_pPlayer.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);


					if (bodyAngle >= 360.f) {
						//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
						m_pPlayer.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);

						m_bDirChanged = false;
						m_bIsRotating = false;
						bodyAngle = 0.f;
						m_eCurDir = PLAYER_DIR::RIGHT;
						return;

					}
				}
			}
		}
		else {
			if (!m_bStairMove) {
				if (m_eCurDir == PLAYER_DIR::LEFT) {
					if (CGameInstance::Get().Key_Down(DIK_D)) {
						m_bDirChanged = true;
						m_bIsRotating = true;
						return;
					}
				}
				else if (m_eCurDir == PLAYER_DIR::RIGHT) {
					if (CGameInstance::Get().Key_Down(DIK_A)) {
						m_bDirChanged = true;
						m_bIsRotating = true;
						return;

					}
				}
			}

		}
	}


	
	
}

void CBody_Player::Update(_float fTimeDelta)
{
	if (m_bOnHit)
	{
		m_fOnHitTime += fTimeDelta;
		if (m_fOnHitTime >= 1.f) {
			m_bOnHit = false;
			m_fOnHitTime = 0.f;
		}
	}
	m_pStateMachine->Update(fTimeDelta);

	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
	__super::Update(fTimeDelta);
	ExpandCollider();
	CheckDoorCollide();
	CheckStairCollide();
	CheckSearchCollide();
}

void CBody_Player::Late_Update(_float fTimeDelta)
{

	Make_CombinedWorldMatrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));

	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CBody_Player));

	__super::Late_Update(fTimeDelta);
}

HRESULT CBody_Player::Render()
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

void CBody_Player::Execute()
{
	auto zombieLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Zombie"));

	if (zombieLayer == nullptr)
		return;
	auto zombies = zombieLayer->GetObjects();
	auto iter = zombies.begin();
	for (iter; iter != zombies.end(); iter++) {
		if (static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_HP() != 0) {
			if (m_pObbCom->myOBB.Intersects(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_Obb()->myOBB)) {
				if (static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_HP() <= 21) {
					if (m_eCurWeapon == PLAYER_WEAPON::HAND) {
						if (CGameInstance::Get().Key_Down(DIK_F)) {
							m_pStateMachine->ChangeState(CPlayer_Execute::Create());
							static_pointer_cast<CZombie>(*iter)->Get_Body()->Set_Executing(true);
							return;
						}
					}
				
				}
				auto dir = static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_CurDir();
				if (ETOUI(static_pointer_cast<CZombie>(*iter)->Get_Body()->Get_CurDir()) == ETOUI(m_eCurDir)) {
					if (CGameInstance::Get().Key_Down(DIK_F)) {
						static_pointer_cast<CZombie>(*iter)->Get_Body()->Set_Stealth_Death();
						m_pStateMachine->ChangeState(CPlayer_StealthAttack::Create());
						return;
					}
				}
			}
		}
		
	}
}
void CBody_Player::CheckDoorCollide()
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
			
			XMStoreFloat4(&myPos, m_pPlayer.lock()->Get_Transform()->Get_State(STATE::POSITION));
			//XMStoreFloat3(&doorPos, pDoor->Get_Obb()->myOBB.Center);
			if (fabs(myPos.x - pDoor->Get_Obb()->myOBB.Center.x) < 0.03f) {
				pCollidedDoor = pDoor.get(); // 충돌한 문의 주소값 저장
				break; // 찾았으니 다른 문은 더 돌 필요 없이 루프 탈출!

			}
		}
	}

	if (pCollidedDoor != nullptr)
	{
		// 충돌한 문이 있는 경우 처리
		auto myPos = m_pPlayer.lock()->Get_Transform()->Get_State(STATE::POSITION);
		auto doorPos = pCollidedDoor->Get_Transform()->Get_State(STATE::POSITION);

		_float4 deltaPos;
		XMStoreFloat4(&deltaPos, (myPos - doorPos));

		if (deltaPos.x > 0)
		{
			if (pCollidedDoor->Get_LeftBlocker() != nullptr) {
				pCollidedDoor->Get_LeftBlocker()->Set_IsScanning(true);
				pCollidedDoor->Get_LeftBlocker()->SetScanningFromLeft(false);
				pCollidedDoor->Get_LeftBlocker()->Set_WasScanning(true);
			}
			if (pCollidedDoor->Get_RightBlocker() != nullptr) {
				pCollidedDoor->Get_RightBlocker()->Set_IsScanning(false);
			}
		}
		else
		{
			if (pCollidedDoor->Get_RightBlocker() != nullptr) {
				pCollidedDoor->Get_RightBlocker()->Set_IsScanning(true);
				pCollidedDoor->Get_RightBlocker()->SetScanningFromLeft(true);
				pCollidedDoor->Get_RightBlocker()->Set_WasScanning(true);
			}
			if (pCollidedDoor->Get_LeftBlocker() != nullptr) {
				pCollidedDoor->Get_LeftBlocker()->Set_IsScanning(false);
			}
		}
		
	}
	else
	{
		// 어떤 문과도 충돌하지 않았다면 모든 문의 블로커를 안전하게 꺼줍니다.
		for (auto& pDoorObj : doors)
		{
			auto pDoor = static_pointer_cast<CDoor>(pDoorObj);
			if (pDoor == nullptr) continue;
		/*	if (pDoor->Get_DoorOpened()) {
				return;
			}*/

			if (pDoor->Get_LeftBlocker() != nullptr)
				pDoor->Get_LeftBlocker()->Set_IsScanning(false);
			if (pDoor->Get_RightBlocker() != nullptr)
				pDoor->Get_RightBlocker()->Set_IsScanning(false);
		}
	}

	
}

void CBody_Player::CheckStairCollide()
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
			XMStoreFloat4(&stairColliderPos,pStair->Get_Transform()->Get_State(STATE::POSITION));

			break; // 찾았으니 다른 문은 더 돌 필요 없이 루프 탈출!
		}
	}

}

void CBody_Player::CheckSearchCollide()
{
	auto searchLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Search_Collider"));
	if (searchLayer == nullptr)
		return;

	auto boxes = searchLayer->GetObjects();

	for (auto& box : boxes)
	{
		auto pBox = static_pointer_cast<CSearch_Collider>(box);
		if (pBox == nullptr) continue;
		auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_SearchBox"));
		auto searchBox = layer->GetObjectFirst();
		if (m_pObbCom->myOBB.Intersects(pBox->Get_Obb()->myOBB))
		{
			pBox->Set_Render(true);
			if (CGameInstance::Get().Key_Down(DIK_F)) {
				auto layer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Inventory"));
				auto inven = layer->GetObjectFirst();
				static_pointer_cast<CInventory>(inven)->Set_Render(true);

				static_pointer_cast<CSearchBox>(searchBox)->Set_Render(true);
				static_pointer_cast<CSearchBox>(searchBox)->Refresh();
				auto overlaylayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), TEXT("Layer_Overlay"));
				auto overlay = overlaylayer->GetObjectFirst();
				static_pointer_cast<COverlay>(overlay)->Set_Render(true);
				pBox->Set_Dead();
			}
			break; 
		}
		else {
			pBox->Set_Render(false);
			
			//static_pointer_cast<CSearchBox>(searchBox)->Set_Render(false);
		}
	}
}

HRESULT CBody_Player::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Joe")));
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


unique_ptr<CBody_Player> CBody_Player::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CBody_Player>(new CBody_Player(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Player");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CBody_Player::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CBody_Player(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Player");
		return nullptr;
	}

	return pInstance;
}

void CBody_Player::ExpandCollider()
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