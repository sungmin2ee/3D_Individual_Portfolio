#include "ReleaseCollider.h"
#include "Obb.h"
CReleaseCollider::CReleaseCollider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) :CGameObject(pDevice, pContext)
{
}

CReleaseCollider::CReleaseCollider(const CReleaseCollider& Prototype) :
	CGameObject(Prototype)
{
}

CReleaseCollider::~CReleaseCollider()
{
}

HRESULT CReleaseCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CReleaseCollider::Initialize(void* pArg)
{


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CReleaseCollider));
	ExpandCollider();
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.5f, 0.35f, 0.15f, 1));

	return S_OK;
}

void CReleaseCollider::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CReleaseCollider::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	ExpandCollider();
}

void CReleaseCollider::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::NONBLEND, SHARED_THIS(CReleaseCollider));

	__super::Late_Update(fTimeDelta);

}

void CReleaseCollider::ExpandCollider()
{

	_float3 min = { -0.1f,-0.1f,-0.1f };
	_float3 max = { 0.1f,0.1f,0.1f };

	_float4x4 mat = m_pTransformCom->GetWorld();
	_matrix world = XMLoadFloat4x4(&mat);

	// 1. Center 계산: 로컬 중심점(mid)을 월드 행렬로 변환
	XMVECTOR xmMin = XMLoadFloat3(&min);
	XMVECTOR xmMax = XMLoadFloat3(&max);
	XMVECTOR mid = (xmMin + xmMax) * 0.5f;
	XMVECTOR centerWorld = m_pTransformCom->Get_State(STATE::POSITION);
	XMStoreFloat3(&m_pObbCom->myOBB.Center, centerWorld);

	// 2. Extents 계산: (모델 크기 * 트랜스폼 스케일)의 절반
	// myOBB 자체가 월드에서 클릭되어야 하므로 여기서 스케일을 미리 곱해야 합니다.
	m_pObbCom->myOBB.Extents.x = (max.x - min.x) * 0.2f;
	m_pObbCom->myOBB.Extents.y = (max.y - min.y) * 0.2f;
	m_pObbCom->myOBB.Extents.z = (max.z - min.z) * 0.2f;

	// 3. Orientation 추출: 월드 행렬에서 회전값만 가져옴
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, world);
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 4. 렌더링용 월드 행렬 (m_WorldMatrix) 갱신
	// VIBuffer_Collider는 -0.5 ~ 0.5 (크기 1)이므로, Extents * 2를 하면 딱 맞습니다.
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x,
		m_pObbCom->myOBB.Extents.y,
		m_pObbCom->myOBB.Extents.z);
	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(centerWorld);

	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}
HRESULT CReleaseCollider::Render()
{
	m_pObbCom->Render();
	return S_OK;
}
HRESULT CReleaseCollider::Ready_Components()
{


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

unique_ptr<CReleaseCollider> CReleaseCollider::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CReleaseCollider>(new CReleaseCollider(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CReleaseCollider");
		return nullptr;
	}

	return pInstance;
}
shared_ptr<CPrototype> CReleaseCollider::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CReleaseCollider(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CReleaseCollider");
		return nullptr;
	}

	return pInstance;
}

