#include "CMesh.h"

CMesh::CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Initialize(const aiMesh* pAIMesh)
{
    m_iNumVertexBuffers = 1;
    m_iNumVertices = pAIMesh->mNumVertices;
    m_iVertexStride = sizeof(VTXMESH);
    m_iNumIndices = pAIMesh->mNumFaces * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;

    VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

    for (size_t i = 0; i < m_iNumVertices; i++)
    {
        memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
        memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
        memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
    }

    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

#pragma endregion


#pragma region INDEX_BUFFER
    D3D11_BUFFER_DESC           IndexBufferDesc{};
    IndexBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.StructureByteStride = m_iIndexStride;
    IndexBufferDesc.CPUAccessFlags = 0;
    IndexBufferDesc.MiscFlags = 0;

    uint32_t* pIndices = new uint32_t[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(uint32_t) * m_iNumIndices);

    uint32_t        iNumIndices = {};

    for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
    {
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
        pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
    }

    D3D11_SUBRESOURCE_DATA          IndexInitialData{};
    IndexInitialData.pSysMem = pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;
 
    //if (pAIMesh->mMaterialIndex >= 0) {
    //    aiMaterial* material = scene->mMaterials[pAIMesh->mMaterialIndex];
    //
    //    std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
    //    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //}
#pragma endregion

    Safe_Delete_Array(pVertices);
    Safe_Delete_Array(pIndices);

    return S_OK;
}

HRESULT CMesh::Initialize_Binary(const vector<VTXMESH>& vertices, const vector<uint32_t>& indices)
{

    m_iNumVertexBuffers = 1;
    m_iNumVertices = vertices.size();
    m_iVertexStride = sizeof(VTXMESH);
    m_iNumIndices = indices.size();
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;





    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = vertices.data();

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

#pragma endregion


#pragma region INDEX_BUFFER
    D3D11_BUFFER_DESC           IndexBufferDesc{};
    IndexBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.StructureByteStride = m_iIndexStride;
    IndexBufferDesc.CPUAccessFlags = 0;
    IndexBufferDesc.MiscFlags = 0;



    D3D11_SUBRESOURCE_DATA          IndexInitialData{};
    IndexInitialData.pSysMem = indices.data();

    if (FAILED(m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitialData, &m_pIB)))
        return E_FAIL;

#pragma endregion



    return S_OK;
}

shared_ptr<CMesh> CMesh::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const aiMesh* pAIMesh)
{
    auto		pInstance = shared_ptr<CMesh>(new CMesh(pDevice, pContext));

    if (FAILED(pInstance->Initialize(pAIMesh)))
    {
        MSG_BOX("Failed to Created : CMesh");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CMesh> CMesh::Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const vector<VTXMESH>& vertices, const vector<uint32_t>& indices)
{
    auto		pInstance = shared_ptr<CMesh>(new CMesh(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Binary(vertices, indices)))
    {
        MSG_BOX("Failed to Created : CMesh");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CPrototype> CMesh::Clone(void* pArg)
{
    return shared_ptr<CPrototype>();
}


