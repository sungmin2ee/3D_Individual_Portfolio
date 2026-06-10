#include "Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{

}

CLight_Manager::~CLight_Manager()
{
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
    auto pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
    if (nullptr == pLight)
        return E_FAIL;

    m_Lights.push_back(pLight);

    return S_OK;
}

HRESULT CLight_Manager::Render(shared_ptr<class CShader> pShader, shared_ptr<class CVIBuffer_Rect> pVIBuffer)
{
    for (auto& pLight : m_Lights)
    {
        pLight->Render(pShader, pVIBuffer);
    }

    return S_OK;
}

void CLight_Manager::Clear_Lights()
{
    m_Lights.clear();
}

unique_ptr<CLight_Manager>  CLight_Manager::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    return  unique_ptr<CLight_Manager>(new CLight_Manager(pDevice, pContext));
}
