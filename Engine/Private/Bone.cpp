#include "Bone.h"

CBone::CBone()
{
}

CBone::~CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, int32_t iParentIndex)
{
    strcpy_s(m_szName, pAINode->mName.C_Str());

    memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

    XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

    m_iParentBoneIndex = iParentIndex;

    return S_OK;
}

HRESULT CBone::Initialize_Binary(_char* szName, _float4x4 transformationMat, int32_t iParentIndex)
{
    strcpy_s(m_szName, szName);

    memcpy(&m_TransformationMatrix, &transformationMat, sizeof(_float4x4));

    //XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

    m_iParentBoneIndex = iParentIndex;
    return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<shared_ptr<CBone>>& Bones, _fmatrix PreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex) {
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
        return;
    }


    XMStoreFloat4x4(&m_CombinedTransformationMatrix,
        XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

shared_ptr<CBone> CBone::Create(const aiNode* pAINode, int32_t iParentIndex)
{
    auto		pInstance = shared_ptr<CBone>(new CBone());

    if (FAILED(pInstance->Initialize(pAINode, iParentIndex)))
    {
        MSG_BOX("Failed to Created : CBone");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CBone> CBone::Create_Binary(_char* szName, _float4x4 transformationMat, int32_t iParentIndex)
{
    auto		pInstance = shared_ptr<CBone>(new CBone());

    if (FAILED(pInstance->Initialize_Binary(szName, transformationMat, iParentIndex)))
    {
        MSG_BOX("Failed to Created : CBone");
        return nullptr;
    }

    return pInstance;
}