#include "Body_Player.h"
#include "Player.h"
#include "VIBuffer_Cube.h"
#include "Obb.h"
#include "Player_Idle.h"
#include "Player_Stair.h"

#include "GameInstance.h"
#include "Stair_Collider.h"

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
	pDesc->fRotationPerSec = 720.f;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	m_pStateMachine = StateMachine<CBody_Player>::Create(this, CPlayer_Idle::Create());
	m_pModelCom->Calculate_Box(ETOUI(MODEL::ANIM));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(this);
	ExpandCollider();

	return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


	if (m_bIsRotating) {
		if (m_bDirChanged && m_eCurState != PLAYER_STATE::ATTACK) {
			if (m_eCurDir == CBody_Player::PLAYER_DIR::RIGHT) {
				bodyAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);
				if (bodyAngle >= 180.f) {
					m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 270.f);
					m_bDirChanged = false;
					m_bIsRotating = false;
					bodyAngle = 180.f;
					m_eCurDir = PLAYER_DIR::LEFT;
					return;

				}
			}
			else if (m_eCurDir == CBody_Player::PLAYER_DIR::LEFT) {
				bodyAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), bodyAngle);
				if (bodyAngle >= 360.f) {
					m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
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

HRESULT CBody_Player::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STAGE2), TEXT("Prototype_Model_Joe")));
	if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh")));
	if (FAILED(__super::Add_Component(TEXT("Com_Shader"), m_pShaderCom)))
		return E_FAIL;

	m_pObbBfCom = static_pointer_cast<VIBuffer_Cube>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), L"Prototype_Cube_Buffer"));
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

	vector<const _float4x4*> bones =
	{
		boneMat,
		boneMat1,
		boneMat2,
		boneMat3,
		boneMat4,
		boneMat5
	};

	_float3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	_float3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	_matrix world = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr());

	for (auto bone : bones)
	{
		if (!bone)
			continue;

		_matrix boneMatrix = XMLoadFloat4x4(bone);

		XMVECTOR pos = boneMatrix.r[3];

		pos = XMVector3TransformCoord(pos, world);

		_float3 p;
		XMStoreFloat3(&p, pos);

		vMin.x = min(vMin.x, p.x);
		vMin.y = min(vMin.y, p.y);
		vMin.z = min(vMin.z, p.z);

		vMax.x = max(vMax.x, p.x);
		vMax.y = max(vMax.y, p.y);
		vMax.z = max(vMax.z, p.z);
	}

	_float3 scale = m_pTransformCom->Get_Scaled();


	// 1. Center 계산: 로컬 중심점(mid)을 월드 행렬로 변환
	XMVECTOR mid;
	_float fLeftOffset = 0.01f;
	mid = (XMLoadFloat3(&vMax) + XMLoadFloat3(&vMin)) * 0.5f;
	//if (m_eCurState == PLAYER_STATE::ATTACK) {
	//	if (m_eCurDir == PLAYER_DIR::LEFT) {
	//		mid = XMVectorSetX(mid, XMVectorGetX(mid) - fLeftOffset);
	//	}
	//	else {
	//		mid = XMVectorSetX(mid, XMVectorGetX(mid) + fLeftOffset);
	//	}
	//	XMStoreFloat3(&m_pObbCom->myOBB.Center, mid);
	//}
	//else {
	//	XMStoreFloat3(&m_pObbCom->myOBB.Center, mid);
	//}
	XMStoreFloat3(&m_pObbCom->myOBB.Center, mid);

	// 2. Extents 계산: (모델 크기 * 트랜스폼 스케일)의 절반
	// myOBB 자체가 월드에서 클릭되어야 하므로 여기서 스케일을 미리 곱해야 합니다.
	m_pObbCom->myOBB.Extents.x = (vMax.x - vMin.x) * 0.5f;// * scale.x;
	m_pObbCom->myOBB.Extents.y = (vMax.y - vMin.y) * 0.5f;// * scale.y;
	m_pObbCom->myOBB.Extents.z = (vMax.z - vMin.z) * 0.9f;// * scale.z;

	// 3. Orientation 추출: 월드 행렬에서 회전값만 가져옴
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, world);
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 4. 렌더링용 월드 행렬 (m_WorldMatrix) 갱신
	// VIBuffer_Cube는 -0.5 ~ 0.5 (크기 1)이므로, Extents * 2를 하면 딱 맞습니다.
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x * 2.f,
		m_pObbCom->myOBB.Extents.y * 2.f,
		m_pObbCom->myOBB.Extents.z * 2.f);
	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(mid);
	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}

