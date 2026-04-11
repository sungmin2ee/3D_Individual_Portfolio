#include "CModelObject.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Obb.h"
#include "VIBuffer_Cube.h"

CModelObject::CModelObject(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext) :CGameObject{ pDevice, pContext }
{
}

CModelObject::~CModelObject()
{
}

HRESULT CModelObject::Initialize_Prototype()
{
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
    return unique_ptr<CModelObject>(new CModelObject(pDevice, pContext));
}


HRESULT CModelObject::Initialize(void* pArg)
{
    // 1. 부모(Transform 생성 등) 초기화
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (nullptr != pArg)
    {
        MODELOBJ_DESC* pDesc = (MODELOBJ_DESC*)pArg;
        m_pTransformCom->SetWorld(pDesc->worldMatrix);
        // 2. 인자로 들어온 태그를 사용하여 모델 컴포넌트 클론
        m_pModelCom = static_pointer_cast<Model>(CGameInstance::Get().Clone_Prototype(pDesc->levelIndex, pDesc->pModelPrototypeTag));
        m_pModelCom->Calculate_Box();
        // 3. 쉐이더 컴포넌트 클론
        m_pShaderCom = static_pointer_cast<Shader>(CGameInstance::Get().Clone_Prototype(pDesc->levelIndex, pDesc->pShaderPrototypeTag));

        m_pCubeBfCom = static_pointer_cast<VIBuffer_Cube>(CGameInstance::Get().Clone_Prototype(0, L"Prototype_Cube_Buffer"));

        m_pColliderCom = static_pointer_cast<Obb>(CGameInstance::Get().Clone_Prototype(0, L"Prototype_OBB", &m_pCubeBfCom));

        if (pDesc->AddCollider) {
            CGameInstance::Get().Add_Collider(m_pColliderCom);
        }
        m_pColliderCom->SetOwner(this);
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

}

void CModelObject::Late_Update(_float fTimeDelta)
{
    //CGameInstance::Get().Add_RenderObject(RENDERGROUP::BLEND, SHARED_THIS(CModelObject));
}

HRESULT CModelObject::Render()
{
    if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
        return E_FAIL;

    // 1. Matrix 업데이트
    MatrixBuffer cb;
    _float4x4 matWorld = m_pTransformCom->GetWorld();
    XMStoreFloat4x4(&cb.world, XMMatrixTranspose(XMLoadFloat4x4(&matWorld)));

    _float4x4 matView = CGameInstance::Get().GetView();
    XMStoreFloat4x4(&cb.view, XMMatrixTranspose(XMLoadFloat4x4(&matView)));

    _float4x4 matProj = CGameInstance::Get().GetProj();
    XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(XMLoadFloat4x4(&matProj)));

    XMStoreFloat4x4(&cb.socket, XMMatrixIdentity());

    // 2. 바인딩 및 그리기
    m_pShaderCom->Bind_Matrix(cb);


    m_pModelCom->Draw();


    m_pColliderCom->Render();
    return S_OK;
}