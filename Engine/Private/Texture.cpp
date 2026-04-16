#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CComponent{ pDevice, pContext }
{
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, uint32_t iNumTextures)
{
    m_iNumTextures = iNumTextures;

    m_Textures.reserve(m_iNumTextures);

    for (size_t i = 0; i < m_iNumTextures; i++)
    {
        ID3D11ShaderResourceView* pSRV = { nullptr };

        _tchar          szTextureFilePath[MAX_PATH] = {};

        wsprintf(szTextureFilePath, pTextureFilePath, i);

        _tchar          szEXT[MAX_PATH] = {};

        _wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

        HRESULT         hr = {};

        if (false == lstrcmp(szEXT, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, &pSRV);

        else if (false == lstrcmp(szEXT, TEXT(".tga")))
            hr = E_FAIL;
        else
            hr = CreateWICTextureFromFile(m_pDevice.Get(), szTextureFilePath, nullptr, &pSRV);

        m_Textures.push_back(pSRV);
    }

    return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iTextureIndex)
{
    if (iTextureIndex >= m_iNumTextures)
        return E_FAIL;

    return pShader->Bind_SRV(pConstantName, m_Textures[iTextureIndex]);
}

unique_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pTextureFilePath, uint32_t iNumTextures)
{
    auto		pInstance = unique_ptr<CTexture>(new CTexture(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
    {
        MSG_BOX("Failed to Created : CTexture");
        return nullptr;
    }

    return pInstance;
}


shared_ptr<CPrototype> CTexture::Clone(void* pArg)
{
    auto		pInstance = shared_ptr<CTexture>(new CTexture(*this));

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CTexture");
        return nullptr;
    }

    return pInstance;
}
