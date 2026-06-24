#include "Renderer.h"

CRenderer::CRenderer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }
{
}

CRenderer::~CRenderer()
{
}

HRESULT CRenderer::Initialize()
{
    _float2     vViewportSize = CGameInstance::Get().Get_ViewportSize();

    /* For.Target_Diffuse */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Diffuse"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    /* For.Target_Normal */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Normal"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;

    /* For.Target_Shade */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Shade"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;
    /* For.Target_Specular */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Specular"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;
    /* For.Target_Depth */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Depth"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;
    /*For. Target Emissive*/
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Emissive"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;


    /* For.Target_Bloom */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Scene"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_Bright"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;
    /* For.Target_Blur */
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_BlurX"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_RenderTarget(TEXT("Target_BlurY"), vViewportSize.x, vViewportSize.y, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;

    /* For.MRT_GameObject */
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Diffuse"))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Normal"))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Depth"))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Emissive"))))
        return E_FAIL;
    /* For.MRT_LightAcc */
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
        return E_FAIL;

    /* For.MRT_Scene */
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_Scene"), TEXT("Target_Scene"))))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_Bright"), TEXT("Target_Bright"))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_BlurX"), TEXT("Target_BlurX"))))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Add_MRT(TEXT("MRT_BlurY"), TEXT("Target_BlurY"))))
        return E_FAIL;

    m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;

    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
    if (nullptr == m_pShader)
        return E_FAIL;

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(vViewportSize.x, vViewportSize.y, 1.f));
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(vViewportSize.x, vViewportSize.y, 0.f, 1.f));

#ifdef _DEBUG
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Diffuse"), 150.f, 150.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Normal"), 150.f, 450.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Shade"), 450.f, 150.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Specular"), 450.f, 450.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Scene"), 150.f, 150.f, 300.f, 300.f)))
    //    return E_FAIL;
    //
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Bright"), 150.f, 450.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Emissive"), 450.f, 150.f, 300.f, 300.f)))
    //    return E_FAIL;
    //if (FAILED(CGameInstance::Get().Ready_RT_Debug(TEXT("Target_Depth"), 450.f, 450.f, 300.f, 300.f)))
    //    return E_FAIL;
    //


#endif
    return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, shared_ptr<CGameObject> pRenderObject)
{
    if (eRenderGroup >= RENDERGROUP::END || 
        nullptr == pRenderObject)
        return E_FAIL;

    m_RenderObjects[ETOUI(eRenderGroup)].push_back(pRenderObject);

    return S_OK;
}

HRESULT CRenderer::Draw()
{
    if (FAILED(Render_Priority()))
        return E_FAIL;


    if (FAILED(Render_NonBlend()))
        return E_FAIL;
    if (FAILED(Render_Lights()))
        return E_FAIL;

    if (FAILED(Render_Combined()))
        return E_FAIL;

    if (FAILED(Render_Bright()))
        return E_FAIL;
    if (FAILED(Render_BlurX()))
        return E_FAIL;

    if (FAILED(Render_BlurY()))
        return E_FAIL;

    if (FAILED(Render_Final()))
        return E_FAIL;
    if (FAILED(Render_Blocker()))
        return E_FAIL;
    if (FAILED(Render_NonLights()))
        return E_FAIL;

    if (FAILED(Render_Blend()))
        return E_FAIL;



    if (FAILED(Render_UI()))
        return E_FAIL;

    if (FAILED(Render_ICON()))
        return E_FAIL;

#ifdef _DEBUG
    if (FAILED(Render_DEBUG()))
        return E_FAIL;
#endif
    return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::PRIORITY)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::PRIORITY)].clear();

    return S_OK;
}



HRESULT CRenderer::Render_NonBlend()
{
    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_GameObject"))))
        return E_FAIL;
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::NONBLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::NONBLEND)].clear();
    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;
    return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
    /* Shade */
    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_LightAcc"))))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
        return E_FAIL;
    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    m_pShader->Bind_Matrix("g_ProjMatrixInverse", CGameInstance::Get().Get_Transform_Inverse(D3DTS::PROJ));
    m_pShader->Bind_Matrix("g_ViewMatrixInverse", CGameInstance::Get().Get_Transform_Inverse(D3DTS::VIEW));

    m_pShader->Bind_RawValue("g_vCamPosition", CGameInstance::Get().Get_CamPosition(), sizeof(_float4));
    if (FAILED(m_pVIBuffer->Bind_Resources()))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().Render_Lights(m_pShader, m_pVIBuffer)))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRenderer::Render_Combined()
{

    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_Scene"))))
        return E_FAIL;


    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Emissive"), m_pShader, "g_EmissiveTexture")))
        return E_FAIL;

    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    if (FAILED(m_pShader->Begin(ETOUI(DEFERRED::COMBINED))))
        return E_FAIL;

    if (FAILED(m_pVIBuffer->Bind_Resources()))
        return E_FAIL;

    if (FAILED(m_pVIBuffer->Render()))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;


    return S_OK;
}
HRESULT CRenderer::Render_BlurX()
{
    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_BlurX"))))
        return E_FAIL;

    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Scene"), m_pShader, "g_BlurTexture");

    _float2 texel =
    {
        1.f / CGameInstance::Get().Get_ViewportSize().x,
        1.f / CGameInstance::Get().Get_ViewportSize().y
    };
    m_pShader->Bind_RawValue(
        "g_vTexelSize",
        &texel,
        sizeof(_float2));
   // m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
   // m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
   // m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    m_pShader->Begin(ETOUI(DEFERRED::BLUR_X));

    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();
    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;


    return S_OK;
}

HRESULT CRenderer::Render_BlurY()
{
    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_BlurY"))))
        return E_FAIL;

    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_BlurX"), m_pShader, "g_BlurTexture");

    //m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    //m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    //m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
    _float2 texel =
    {
        1.f / CGameInstance::Get().Get_ViewportSize().x,
        1.f / CGameInstance::Get().Get_ViewportSize().y
    };
    m_pShader->Bind_RawValue(
        "g_vTexelSize",
        &texel,
        sizeof(_float2));
    m_pShader->Begin(ETOUI(DEFERRED::BLUR_Y));

    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();

    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;

    return S_OK;
}
//HRESULT CRenderer::Render_BlurX()
//{
//    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_BlurX"))))
//        return E_FAIL;
//
//    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Bright"), m_pShader, "g_BlurTexture");
//
//    _float2 texel =
//    {
//        1.f / CGameInstance::Get().Get_ViewportSize().x,
//        1.f / CGameInstance::Get().Get_ViewportSize().y
//    };
//    m_pShader->Bind_RawValue(
//        "g_vTexelSize",
//        &texel,
//        sizeof(_float2));
//   // m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
//   // m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
//   // m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
//
//    m_pShader->Begin(ETOUI(DEFERRED::BLUR_X));
//
//    m_pVIBuffer->Bind_Resources();
//    m_pVIBuffer->Render();
//    if (FAILED(CGameInstance::Get().End_MRT()))
//        return E_FAIL;
//
//
//    return S_OK;
//}
//
//HRESULT CRenderer::Render_BlurY()
//{
//    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_BlurY"))))
//        return E_FAIL;
//
//    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_BlurX"), m_pShader, "g_BlurTexture");
//
//    //m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
//    //m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
//    //m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
//    _float2 texel =
//    {
//        1.f / CGameInstance::Get().Get_ViewportSize().x,
//        1.f / CGameInstance::Get().Get_ViewportSize().y
//    };
//    m_pShader->Bind_RawValue(
//        "g_vTexelSize",
//        &texel,
//        sizeof(_float2));
//    m_pShader->Begin(ETOUI(DEFERRED::BLUR_Y));
//
//    m_pVIBuffer->Bind_Resources();
//    m_pVIBuffer->Render();
//
//    if (FAILED(CGameInstance::Get().End_MRT()))
//        return E_FAIL;
//
//    return S_OK;
//}

HRESULT CRenderer::Render_Final()
{
    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Scene"),m_pShader, "g_SceneTexture");
    //CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_BlurY"),m_pShader, "g_BloomTexture");

    m_pShader->Begin(ETOUI(DEFERRED::FINAL));

    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();


    return S_OK;
}

HRESULT CRenderer::Render_Bright()
{
    if (FAILED(CGameInstance::Get().Begin_MRT(TEXT("MRT_Bright"))))
        return E_FAIL;

    CGameInstance::Get().Bind_RT_ShaderResource(TEXT("Target_Scene"), m_pShader, "g_SceneTexture");

    m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);
    m_pShader->Begin( ETOUI(DEFERRED::BRIGHT));

    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();

    if (FAILED(CGameInstance::Get().End_MRT()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::BLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::BLEND)].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Blocker()
{

    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::BLOCKER)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::BLOCKER)].clear();

    return S_OK;
}
HRESULT CRenderer::Render_UI()
{
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::UI)].clear();

    return S_OK;
}
HRESULT CRenderer::Render_ICON()
{
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::ICON)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::ICON)].clear();

    return S_OK;
}


HRESULT CRenderer::Render_NonLights()
{
    for (auto& pRenderObject : m_RenderObjects[ETOUI(RENDERGROUP::NONLIGHT)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();
    }

    m_RenderObjects[ETOUI(RENDERGROUP::NONLIGHT)].clear();

    return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugComponent(shared_ptr<CComponent> pDebugComponent)
{
    m_DebugComponents.push_back(pDebugComponent);

    return S_OK;
}
#endif
#ifdef _DEBUG

HRESULT CRenderer::Render_DEBUG()
{

    for (auto& pDebugCom : m_DebugComponents)
    {
        pDebugCom->Render();
    }
    m_DebugComponents.clear();

    _float4x4   WorldMatrix = {};

    XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


    m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    if (FAILED(m_pVIBuffer->Bind_Resources()))
        return E_FAIL;

    if (FAILED(CGameInstance::Get().Debug_RT_Render(TEXT("MRT_GameObject"), m_pShader, "g_Texture", m_pVIBuffer)))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Debug_RT_Render(TEXT("MRT_LightAcc"), m_pShader, "g_Texture", m_pVIBuffer)))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Debug_RT_Render(TEXT("MRT_Scene"), m_pShader, "g_Texture", m_pVIBuffer)))
        return E_FAIL;
    if (FAILED(CGameInstance::Get().Debug_RT_Render(TEXT("MRT_Bright"), m_pShader, "g_Texture", m_pVIBuffer)))
        return E_FAIL;

    return S_OK;
}
#endif

unique_ptr<CRenderer> CRenderer::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    auto		pInstance = unique_ptr<CRenderer>(new CRenderer(pDevice, pContext));

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CRenderer");
        return nullptr;
    }

    return pInstance;
}

