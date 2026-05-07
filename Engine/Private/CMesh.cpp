#include "CMesh.h"
#include "CModel.h"
#include "GameInstance.h"

CMesh::CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Initialize(uint32_t eType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
    m_iMaterialIndex = pAIMesh->mMaterialIndex;
    m_iNumVertexBuffers = 1;
    m_iNumVertices = pAIMesh->mNumVertices;

    m_iNumIndices = pAIMesh->mNumFaces * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

    HRESULT     hr = ETOUI(MODEL::NONANIM) == eType ?
        Ready_NonAnimMesh(pAIMesh, PreTransformMatrix) : Ready_AnimMesh(pModel, pAIMesh);

    if (FAILED(hr))
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
    indices_.assign(pIndices, pIndices + m_iNumIndices);
    Safe_Delete_Array(pIndices);

    return S_OK;
}
template <typename T>
HRESULT CMesh::Initialize_Binary(uint32_t matIndex, const vector<T>& vertices, const vector<uint32_t>& indices, uint32_t numBones)
{
    m_iMaterialIndex = matIndex;
    m_iNumVertexBuffers = 1;
    m_iNumVertices = vertices.size();
    m_iVertexStride = sizeof(T);
    m_iNumIndices = indices.size();
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    m_iNumBones = numBones;
    if constexpr (std::is_same_v<T, VTXMESH>) {
        nonAnim_vertices = vertices;
    }
    else if constexpr (std::is_same_v<T, VTXANIMMESH>) {
        Anim_vertices = vertices;
    }
    //if (modelType == 0) {
    //    nonAnim_vertices = vertices;
    //}
    //else if (modelType == 1) {
    //    Anim_vertices = vertices;
    //}
    indices_ = indices;
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


    m_BoneMatrices.resize(m_iNumBones);

    return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(const vector<shared_ptr<class CBone>>& Bones, shared_ptr<class CShader> pShader, const _char* pConstantName)
{
    for (size_t i = 0; i < m_iNumBones; i++)
    {
        XMStoreFloat4x4(&m_BoneMatrices[i],
            XMLoadFloat4x4(&m_OffsetMatrices[i]) * Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
    }

    if (false == m_BoneMatrices.empty())
        pShader->Bind_Matrices(pConstantName, &m_BoneMatrices.front(), m_iNumBones);

    return S_OK;
}

shared_ptr<CMesh> CMesh::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, uint32_t eType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
    auto		pInstance = shared_ptr<CMesh>(new CMesh(pDevice, pContext));

    if (FAILED(pInstance->Initialize(eType, pModel,pAIMesh, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : CMesh");
        return nullptr;
    }

    return pInstance;
}
template <typename T>
shared_ptr<CMesh> CMesh::Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const vector<T>& vertices, const vector<uint32_t>& indices, uint32_t matIndex, uint32_t numBones)
{
    auto		pInstance = shared_ptr<CMesh>(new CMesh(pDevice, pContext));

    if (FAILED(pInstance->Initialize_Binary(matIndex, vertices, indices, numBones)))
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

HRESULT CMesh::Ready_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
    m_iVertexStride = sizeof(VTXMESH);
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
        XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

        memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
        XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

        memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
    }


    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;

    nonAnim_vertices.assign(pVertices, pVertices + m_iNumVertices);

    Safe_Delete_Array(pVertices);
    return S_OK;
}

HRESULT CMesh::Ready_AnimMesh(CModel* pModel, const aiMesh* pAIMesh)
{
    m_iVertexStride = sizeof(VTXANIMMESH);
    D3D11_BUFFER_DESC           VertexBufferDesc{};
    VertexBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = m_iVertexStride;
    VertexBufferDesc.CPUAccessFlags = 0;
    VertexBufferDesc.MiscFlags = 0;



    VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

    for (size_t i = 0; i < m_iNumVertices; i++)
    {
        memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
        memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
        memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
        memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
    }

    m_iNumBones = pAIMesh->mNumBones;

    m_BoneMatrices.resize(m_iNumBones);
    m_OffsetMatrices.reserve(m_iNumBones);

    for (size_t i = 0; i < m_iNumBones; i++)
    {
        aiBone* pAIBone = pAIMesh->mBones[i];

        _float4x4   OffsetMatrix;
        memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

        XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

        m_OffsetMatrices.push_back(OffsetMatrix);

        int32_t    iBoneIndex = pModel->Get_BoneIndex(pAIBone->mName.C_Str());
        if (-1 == iBoneIndex)
            return E_FAIL;

        m_BoneIndices.push_back(iBoneIndex);

        /* pAIBone->mNumWeights : 이 뼈가 영향을 주는 정점의 갯수 */
        for (size_t j = 0; j < pAIBone->mNumWeights; j++)
        {
            if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
            {
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
            }

            else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
            {
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
            }

            else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
            {
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
            }

            else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w)
            {
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
                pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
            }
        }
    }

    if (0 == m_iNumBones)
    {
        m_iNumBones = 1;

        int32_t        iBoneIndex = { -1 };

        iBoneIndex = pModel->Get_BoneIndex(m_szName);

        if (-1 == iBoneIndex)
            return E_FAIL;

        _float4x4       OffsetMatrix;
        XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

        m_BoneIndices.push_back(iBoneIndex);
        m_OffsetMatrices.push_back(OffsetMatrix);
        m_BoneMatrices.resize(iBoneIndex);
    }
    D3D11_SUBRESOURCE_DATA          VertexInitialData{};
    VertexInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VertexBufferDesc, &VertexInitialData, &m_pVB)))
        return E_FAIL;
    Anim_vertices.assign(pVertices, pVertices + m_iNumVertices);
    Safe_Delete_Array(pVertices);

    return S_OK;
}


