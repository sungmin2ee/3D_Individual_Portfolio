#include "Obb.h"
#include "GameInstance.h"
#include "Helper.h"
#include "VIBuffer_Collider.h"
Obb::Obb(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext) : CCollider(pDevice, pContext)

{
}

Obb::~Obb()
{

}
HRESULT Obb::Initialize(void* pArg) {

    return S_OK;
}

HRESULT Obb::Render()
{

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;
    
    
    if (nullptr == m_pBuffer)
        return E_FAIL;
    
    if (nullptr == m_pShaderCom) {
        return E_FAIL;
    }
    
    
    const _float4x4* view;
    const _float4x4* proj;
    view = CGameInstance::Get().Get_Transform(D3DTS::VIEW);
    proj = CGameInstance::Get().Get_Transform(D3DTS::PROJ);
    
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", view)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", proj)))
        return E_FAIL;
    
    _float4 vColor = m_bIsSelected ? _float4(1, 0, 0, 1) : _float4(0, 1, 0, 1);
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
        return E_FAIL;
    //XMStoreFloat4x4(&cb.matWVP, XMMatrixTranspose(wvp));
    
    //m_pBuffer->UpdateConstantBuffer(cb);
    m_pBuffer->Bind_Resources();
    m_pBuffer->Render();
    
    return S_OK;
}


HRESULT Obb::Initialize_Prototype() {

    if (FAILED(__super::Initialize()))
        return E_FAIL;
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
    auto		pInstance = shared_ptr<Obb>(new Obb(*this));

    if (FAILED(pInstance->Initialize(pArg))) 
        return nullptr;

    return pInstance;
}


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
