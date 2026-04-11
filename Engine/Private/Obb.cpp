#include "Obb.h"
#include "GameInstance.h"
#include "Helper.h"
#include "VIBuffer_Cube.h"
Obb::Obb(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext) : CCollider(pDevice, pContext)

{
}

Obb::~Obb()
{

}
HRESULT Obb::Initialize(void* pArg) {
    auto ppBuffer = static_cast<shared_ptr<VIBuffer_Cube>*>(pArg);
    m_pBuffer = *ppBuffer;


    return S_OK;
}

HRESULT Obb::Render()
{

    if (nullptr == m_pBuffer)
        return E_FAIL;

    CB_MATRIX cb{};

    _float4x4 view1, proj1;
    view1 = CGameInstance::Get().GetView();
    proj1 = CGameInstance::Get().GetProj();
    _matrix world = XMLoadFloat4x4(&m_WorldMatrix);
    _matrix view = XMLoadFloat4x4(&view1);
    _matrix proj = XMLoadFloat4x4(&proj1);

    _matrix wvp = world * view * proj;

    XMStoreFloat4x4(&cb.matWVP, XMMatrixTranspose(wvp));
    cb.vColor = m_bIsSelected ? _float4(1, 0, 0, 1) : _float4(0, 1, 0, 1);

    m_pBuffer->UpdateConstantBuffer(cb);

    m_pBuffer->Render();

    return S_OK;
}


HRESULT Obb::Initialize_Prototype() {


    // __super::Initialize_Prototype();

    return S_OK;
}

unique_ptr<Obb> Obb::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = unique_ptr<Obb>(new Obb(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Obb");
        return nullptr;
    }
    return pInstance;
}
shared_ptr<CPrototype> Obb::Clone(void* pArg)
{
    auto pInstance = make_shared<Obb>(*this);



    if (FAILED(pInstance->Initialize(pArg))) return nullptr;


    return pInstance;
}

//void Obb::Update_OBB()
//{
//    _vector vCenter = XMLoadFloat3(&myOBB.Center);
//    _vector vExtents = XMLoadFloat3(&myOBB.Extents);
//    _vector vQuat = XMLoadFloat4(&myOBB.Orientation);
//
//    _matrix matWorld = XMMatrixAffineTransformation(vExtents * 2.f, XMVectorSet(0, 0, 0, 1), vQuat, vCenter);
//    XMStoreFloat4x4(&m_WorldMatrix, matWorld);
//
//}

bool Obb::Intersects(shared_ptr<CCollider> pTarget)
{
    return false;
}

void Obb::Update(_matrix TransformMatrix)
{

}

bool Obb::IntersectsRay(float* pOutDist)
{
    float dist = 0.f;
    _float3 rayPos;
    _float3 rayDir;

    CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
    bool hit = myOBB.Intersects(
        XMLoadFloat3(&rayPos),
        XMLoadFloat3(&rayDir),
        dist
    );

    *pOutDist = dist;
    return hit;

}
