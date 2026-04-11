#include "AABB.h"
#include "GameInstance.h"
#include "Helper.h"
#include "VIbuffer_Cube.h"

AABB::AABB(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext):CCollider(pDevice, pContext)

{
}

AABB::~AABB()
{
   /* Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);*/
}
HRESULT AABB::Initialize(void* pArg) {

    return S_OK;
}

HRESULT AABB::Render()
{
    if (nullptr == m_pBuffer)
        return E_FAIL;
    _float4 vRenderColor = m_bIsSelected ? _float4(1.f, 0.f, 0.f, 1.f) : _float4(0.f, 1.f, 0.f, 1.f);

    m_pBuffer->Render();
    // 2. 이제 그려!
    //__super::Render();

    return S_OK;
}
  

HRESULT AABB::Initialize_Prototype() {

   // __super::Initialize_Prototype();


    return S_OK;
}
//void Obb::Render_Debug()
//{
//    CB_MATRIX cb{};
//
//    // View * Projection (보통 Transpose 필요)
//    cb.matVP = XMMatrixTranspose(CGameInstance::Get().GetViewXM() *CGameInstance::Get().GetProjXM());
//
//    // 1. OBB의 8개 꼭짓점 좌표를 가져옵니다.
//
//    XMFLOAT3 corners[8];
//    myOBB.GetCorners(corners);
//
//    // 2. 8개의 꼭짓점을 선으로 잇기 위해 PrimitiveBatch를 사용합니다.
//    // (아래는 꼭짓점들을 연결하는 인덱스 순서입니다)
//    for (int i = 0; i < 8; ++i) {
//        pVertices[i].vPos = corners[i];
//        pVertices[i].vColor = XMFLOAT4(1.f, 1.f, 0.f, 1.f); // 녹색
//    }
//    m_pContext->IASetInputLayout(m_pLayout.Get());
//    Bind_Resources();
//    m_pContext->VSSetShader(m_pVS.Get(), 0, 0);
//    m_pContext->PSSetShader(m_pPS.Get(), 0, 0);
//
//    m_pContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
//    // 1. 데이터를 GPU 버퍼로 복사
//    m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
//    //// 2. 파이프라인에 바인딩
//
//    m_pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
//
//
//
//    D3D11_MAPPED_SUBRESOURCE mapped;
//    m_pContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//
//    memcpy(mapped.pData, pVertices, sizeof(VOBB) * m_iNumVertices);
//
//    m_pContext->Unmap(m_pVB.Get(), 0);
//
//    Render();
//
//}

unique_ptr<AABB> AABB::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = unique_ptr<AABB>(new AABB(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : AABB");
        return nullptr;
    }

    return pInstance;
}
shared_ptr<CPrototype> AABB::Clone(void* pArg)
{
    return nullptr;
}

void AABB::Update(_matrix TransformMatrix)
{

}
void AABB::Update_AABB()
{
    _float3 rayPos;
    _float3 rayDir;

    CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
    float dist = 0.f;

    bool hit = myAABB.Intersects(
        XMLoadFloat3(&rayPos),
        XMLoadFloat3(&rayDir),
        dist
    );
    // 2. OBB corners 가져오기
    XMFLOAT3 corners[8];
    myAABB.GetCorners(corners); //

    // 3. Vertex 채우기
    //for (int i = 0; i < 8; ++i)
    //{
    //    pVertices[i].vPos = corners[i];
    //    if (hit) {
    //        pVertices[i].vColor = XMFLOAT4(1, 0, 0, 1); // 노란색
    //    }
    //    else {
    //        pVertices[i].vColor = XMFLOAT4(0, 1, 0, 1); // 노란색
    //    }
    //}
}

bool AABB::Intersects(shared_ptr<CCollider> pTarget)
{
    return false;
}

bool AABB::IntersectsRay(_float* pOutDist)
{
    float dist = 0.f;
    _float3 rayPos;
    _float3 rayDir;

    CGameInstance::Get().GetMousePointRay(&rayPos, &rayDir);
    bool hit = myAABB.Intersects(
        XMLoadFloat3(&rayPos),
        XMLoadFloat3(&rayDir),
        dist
    );
    *pOutDist = 0;
    //for (int i = 0; i < 8; ++i)
    //{
    //    pVertices[i].vPos = corners[i];
    //    if (hit) {
    //        pVertices[i].vColor = XMFLOAT4(1, 0, 0, 1); // 노란색
    //    }
    //    else {
    //        pVertices[i].vColor = XMFLOAT4(0, 1, 0, 1); // 노란색
    //    }
    //}

    return hit;
}
