#include "Door.h"
#include "Layer.h"
#include "Player.h"

CDoor::CDoor(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) : CGameObject{ pDevice, pContext }
{
}

CDoor::CDoor(const CDoor& Prototype) : CGameObject{ Prototype }
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{

	auto		pDesc = static_cast<DOOR_DESC*>(pArg);
	m_sTag = pDesc->pGameObjectTag;
	m_sLeftTag = pDesc->leftTag;
	m_sRightTag = pDesc->rightTag;
	pDesc->fRotationPerSec = 320.f;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//m_pTransformCom->Set_Scale(0.001f, 0.001f, 0.001f);
	m_pModelCom->Calculate_Box(ETOUI(MODEL::NONANIM));
	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CDoor));
	m_pTransformCom->SetWorld(pDesc->worldMat);
	ExpandCollider();

	return S_OK;
}

void CDoor::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	_float x = m_pTransformCom->Get_RotSpeed();
	if (m_bRotating) {
		auto playerLayer = CGameInstance::Get().Find_Layer(CGameInstance::Get().GetCurLevelIndex(), L"Layer_Player");
		if (playerLayer == nullptr) return;

		auto player = playerLayer->GetObjectFirst();

		auto playerPos = static_pointer_cast<CPlayer>(player)->Get_Transform()->Get_State(STATE::POSITION);
		_float4 playerf, myPosf;
	    XMStoreFloat4(&myPosf,m_pTransformCom->Get_State(STATE::POSITION));
	    XMStoreFloat4(&playerf,playerPos);
		
		if (m_bDoorOpened) {
			if (myPosf.x > playerf.x) {
				m_fAngle -= fTimeDelta * m_pTransformCom->Get_RotSpeed();
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
				if (m_fAngle < -230.f) {
					m_bRotating = false;

					m_fAngle = -230.f;
					m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
				}
			}
			else {
				m_fAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
				if (m_fAngle < 30.f) {
					m_bRotating = false;

					m_fAngle = 30.f;
					m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
				}
			}
			
		}
		else {
			if (myPosf.x > playerf.x) {
				m_fAngle += fTimeDelta * m_pTransformCom->Get_RotSpeed();
				
			}else{
				m_fAngle -= fTimeDelta * m_pTransformCom->Get_RotSpeed();

			}
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
			if (m_fAngle > -90.f) {
				m_bRotating = false;
				m_fAngle = -90.f;
				m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
			}
		}
	}
	


}

void CDoor::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pObbCom->GetSelected()) {
		ExpandCollider();
	}
}

void CDoor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CDoor));


}
HRESULT CDoor::Ready_Components()
{
	m_pModelCom = dynamic_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Model_Door")));
	if (FAILED(__super::Add_Component(TEXT("Com_Model"), m_pModelCom)))
		return E_FAIL;

	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh")));
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
HRESULT CDoor::Render()
{
    const _float4x4* view;
    const _float4x4* proj;
    view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
    proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);


    _float4x4 mat = m_pTransformCom->GetWorld();


    _float4x4		IdentityMatrix = {};
    XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &mat)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
        return E_FAIL;

    //if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", CGameInstance::Get().Get_CamPosition(), sizeof(_float4))))
    //    return E_FAIL;

    //if (FAILED(m_pShaderCom->Begin(1)))
    //    return E_FAIL;

    uint32_t	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
            return E_FAIL;
        //if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, aiTextureType_Normals, 0)))
        //	return E_FAIL;
  
        if (FAILED(m_pShaderCom->Begin(1)))
            return E_FAIL;

        m_pModelCom->Render(i);
    }
    m_pObbCom->Render();
	return S_OK;
}


unique_ptr<CDoor> CDoor::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CDoor>(new CDoor(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoor");
		return nullptr;
	}

	return pInstance;
}


shared_ptr<CPrototype> CDoor::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CDoor(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDoor");
		return nullptr;
	}

	return pInstance;
}

void CDoor::ExpandCollider()
{
	_float3 min = m_pModelCom->GetMin();
	_float3 max = m_pModelCom->GetMax();
	_float3 scale = m_pTransformCom->Get_Scaled();

	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);

	// 1. Center 계산: 로컬 중심점(mid)을 월드 행렬로 변환
	XMVECTOR xmMin = XMLoadFloat3(&min);
	XMVECTOR xmMax = XMLoadFloat3(&max);
	XMVECTOR mid = (xmMin + xmMax) * 0.5f;
	XMVECTOR centerWorld = XMVector3TransformCoord(mid, world);
	XMStoreFloat3(&m_pObbCom->myOBB.Center, centerWorld);

	// 2. Extents 계산: (모델 크기 * 트랜스폼 스케일)의 절반
	// myOBB 자체가 월드에서 클릭되어야 하므로 여기서 스케일을 미리 곱해야 합니다.
	m_pObbCom->myOBB.Extents.x = (max.x - min.x) * 0.5f * scale.x;
	m_pObbCom->myOBB.Extents.y = (max.y - min.y) * 0.5f * scale.y;
	m_pObbCom->myOBB.Extents.z = (max.z - min.z) * 0.5f * scale.z;

	// 3. Orientation 추출: 월드 행렬에서 회전값만 가져옴
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, world);
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 4. 렌더링용 월드 행렬 (m_WorldMatrix) 갱신
	// VIBuffer_Collider는 -0.5 ~ 0.5 (크기 1)이므로, Extents * 2를 하면 딱 맞습니다.
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x * 2.f,
		m_pObbCom->myOBB.Extents.y * 2.f,
		m_pObbCom->myOBB.Extents.z * 2.f);
	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(centerWorld);

	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}


