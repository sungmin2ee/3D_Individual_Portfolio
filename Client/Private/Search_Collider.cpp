#include "Search_Collider.h"
#include "Obb.h"
CSearch_Collider::CSearch_Collider(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext):CGameObject(pDevice, pContext)
{
}

CSearch_Collider::CSearch_Collider(const CSearch_Collider& Prototype):
    CGameObject(Prototype)
{
}

CSearch_Collider::~CSearch_Collider()
{
}

HRESULT CSearch_Collider::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSearch_Collider::Initialize(void* pArg)
{
	auto	pDesc = static_cast<SEARCH_COLLIDER_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if(FAILED(Ready_Components()))
		return E_FAIL;

	CGameInstance::Get().Add_Collider(m_pObbCom);
	m_pObbCom->SetOwner(SHARED_THIS(CSearch_Collider));
	ExpandCollider();
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->position);
	m_pTransformCom->Set_Scale(0.05f, 0.05f, 0.05f);
	//m_pTransformCom->SetWorld(pDesc->worldMat);
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.1f, 0.f, 0.f, 1));

    return S_OK;
}

void CSearch_Collider::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSearch_Collider::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	ExpandCollider();
}

void CSearch_Collider::Late_Update(_float fTimeDelta)
{
	CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CSearch_Collider));

	__super::Late_Update(fTimeDelta);

}

void CSearch_Collider::ExpandCollider()
{
	
	_float3 min = {-0.1f,-0.1f,-0.1f };
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
	m_pObbCom->myOBB.Extents.z = (max.z - min.z) * 1.f;

	// 3. Orientation 추출: 월드 행렬에서 회전값만 가져옴
	XMVECTOR vScale, vRot, vTrans;
	XMMatrixDecompose(&vScale, &vRot, &vTrans, world);
	XMStoreFloat4(&m_pObbCom->myOBB.Orientation, vRot);

	// 4. 렌더링용 월드 행렬 (m_WorldMatrix) 갱신
	// VIBuffer_Collider는 -0.5 ~ 0.5 (크기 1)이므로, Extents * 2를 하면 딱 맞습니다.
	_matrix matOBBWorld = XMMatrixScaling(m_pObbCom->myOBB.Extents.x ,
		m_pObbCom->myOBB.Extents.y ,
		m_pObbCom->myOBB.Extents.z );
	matOBBWorld *= XMMatrixRotationQuaternion(vRot);
	matOBBWorld *= XMMatrixTranslationFromVector(centerWorld);

	m_pObbCom->Set_WorldMatrix(matOBBWorld);
}
HRESULT CSearch_Collider::Render()
{
	if (!m_bRender) {
		return E_FAIL;
	}
	m_pObbCom->Render();

	const _float4x4* view;
	const _float4x4* proj;
	view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
	proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;



    return S_OK;
}
HRESULT CSearch_Collider::Ready_Components()
{

	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect")));
	if (FAILED(__super::Add_Component(TEXT("Com_VIBuffer"), m_pVIBufferCom)))
		return E_FAIL;
	m_pTextureCom = dynamic_pointer_cast<CTexture>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Texture_icon_search")));
	if (FAILED(__super::Add_Component(TEXT("Com_Texture"), m_pTextureCom)))
		return E_FAIL;
	m_pShaderCom = dynamic_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(ETOUI(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex")));
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

unique_ptr<CSearch_Collider> CSearch_Collider::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	auto	pInstance = unique_ptr<CSearch_Collider>(new CSearch_Collider(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSearch_Collider");
		return nullptr;
	}

	return pInstance;
}
shared_ptr<CPrototype> CSearch_Collider::Clone(void* pArg)
{
	auto	pInstance = shared_ptr<CGameObject>(new CSearch_Collider(*this));

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSearch_Collider");
		return nullptr;
	}

	return pInstance;
}

