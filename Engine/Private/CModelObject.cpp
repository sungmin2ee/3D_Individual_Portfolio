#include "CModelObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "CModel.h"
#include "Obb.h"
#include "VIBuffer_Cube.h"

CModelObject::CModelObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) :CGameObject{ pDevice, pContext }
{
}
CModelObject::CModelObject(const CModelObject& Prototype) :CGameObject{ Prototype.m_pDevice,  Prototype.m_pContext }

{
}
CModelObject::~CModelObject()
{
}

HRESULT CModelObject::Initialize_Prototype()
{

    //D3D11_SAMPLER_DESC sampDesc{};
    //sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    //sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    //sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    //m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
    //m_pTransformCom->Scaling(0.001f, 0.001f, 0.001f);

    return S_OK;
}


shared_ptr<CPrototype> CModelObject::Clone(void* pArg)
{
    auto pInstance = make_shared<CModelObject>(*this);



    if (FAILED(pInstance->Initialize(pArg)))
        return nullptr;


    return pInstance;
}
unique_ptr<CModelObject> CModelObject::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) {
    auto pInstance = unique_ptr<CModelObject>(new CModelObject(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype())) {
        return nullptr;
    }
    return pInstance;
}


HRESULT CModelObject::Initialize(void* pArg)
{

    auto		pDesc = static_cast<MODELOBJ_DESC*>(pArg);
    m_ModelDesc.pModelPrototypeTag = pDesc->pModelPrototypeTag;
    m_ModelDesc.pShaderPrototypeTag = pDesc->pShaderPrototypeTag;
    m_ModelDesc.levelIndex = pDesc->levelIndex;
    m_ModelDesc.filePath = pDesc->filePath;
    m_ModelDesc.collide = pDesc->collide;
    m_ModelDesc.worldMatrix = pDesc->worldMatrix;
    m_ModelDesc.modelType = pDesc->modelType;
    modelType = pDesc->modelType;
    // 1. 부모(Transform 생성 등) 초기화
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (nullptr != pArg)
    {

        m_pTransformCom->SetWorld(pDesc->worldMatrix);
        //m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), 270.f);

        // 2. 인자로 들어온 태그를 사용하여 모델 컴포넌트 클론
        m_pModelCom = static_pointer_cast<CModel>(CGameInstance::Get().Clone_Prototype(pDesc->levelIndex, pDesc->pModelPrototypeTag));
        m_pModelCom->Calculate_Box(modelType);
        // 3. 쉐이더 컴포넌트 클론
        m_pShaderCom = static_pointer_cast<CShader>(CGameInstance::Get().Clone_Prototype(0, pDesc->pShaderPrototypeTag));


        m_pColliderCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(0, L"Prototype_OBB"));
        CGameInstance::Get().Add_Collider(m_pColliderCom);

        if (pDesc->collide) {

        }
        m_pColliderCom->SetOwner(this);
        ExpandCollider();
        //m_pTransformCom->
        //pDesc->vInitialPosition;
        // 4. (선택사항) 필요하다면 콜라이더도 여기서 생성
    }

    return S_OK;
}

void CModelObject::Priority_Update(_float fTimeDelta)
{
    
}

void CModelObject::Update(_float fTimeDelta)
{
    if (m_pColliderCom->GetSelected()) {
        ExpandCollider();
    }
    if (modelType == 1) {
        m_pModelCom->Play_Animation(fTimeDelta);
    }
}

void CModelObject::Late_Update(_float fTimeDelta)
{
    CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CModelObject));
}

HRESULT CModelObject::Render()
{

    const _float4x4* view;
    const _float4x4* proj;
    view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
    proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);


    _float4x4 mat = m_pTransformCom->GetWorld();
    //XMMATRIX matWorld = m_pTransformCom->m_WorldMatrix;
    //  스케일 추가 (FBX 안보일 때 필수)

   // XMStoreFloat4x4(&cb.world, XMMatrixTranspose(matWorld));

    _float4x4		IdentityMatrix = {};
    XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &mat)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", CGameInstance::Get().Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    uint32_t	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
            return E_FAIL;
        //if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, aiTextureType_Normals, 0)))
        //	return E_FAIL;
        if (modelType == 1) {
            if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
                return E_FAIL;
        }
        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        m_pModelCom->Render(i);
    }
    m_pColliderCom->Render();
    return S_OK;
}

void CModelObject::ExpandCollider()
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
    XMStoreFloat3(&m_pColliderCom->myOBB.Center, centerWorld);

    // 2. Extents 계산: (모델 크기 * 트랜스폼 스케일)의 절반
    // myOBB 자체가 월드에서 클릭되어야 하므로 여기서 스케일을 미리 곱해야 합니다.
    m_pColliderCom->myOBB.Extents.x = (max.x - min.x) * 0.5f * scale.x;
    m_pColliderCom->myOBB.Extents.y = (max.y - min.y) * 0.5f * scale.y;
    m_pColliderCom->myOBB.Extents.z = (max.z - min.z) * 0.5f * scale.z;

    // 3. Orientation 추출: 월드 행렬에서 회전값만 가져옴
    XMVECTOR vScale, vRot, vTrans;
    XMMatrixDecompose(&vScale, &vRot, &vTrans, world);
    XMStoreFloat4(&m_pColliderCom->myOBB.Orientation, vRot);

    // 4. 렌더링용 월드 행렬 (m_WorldMatrix) 갱신
    // VIBuffer_Cube는 -0.5 ~ 0.5 (크기 1)이므로, Extents * 2를 하면 딱 맞습니다.
    _matrix matOBBWorld = XMMatrixScaling(m_pColliderCom->myOBB.Extents.x * 2.f,
        m_pColliderCom->myOBB.Extents.y * 2.f,
        m_pColliderCom->myOBB.Extents.z * 2.f);
    matOBBWorld *= XMMatrixRotationQuaternion(vRot);
    matOBBWorld *= XMMatrixTranslationFromVector(centerWorld);

    m_pColliderCom->Set_WorldMatrix(matOBBWorld);
}
