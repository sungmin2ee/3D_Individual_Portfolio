#include "Channel.h"
#include "CModel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

CChannel::~CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, class CModel* pModel)
{
    m_iBoneIndex = pModel->Get_BoneIndex(pAIChannel->mNodeName.C_Str());
    if (-1 == m_iBoneIndex)
        return E_FAIL;

    m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
    m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

    _float3     vScale = {};
    _float4     vRotation = {};
    _float3     vTranslation = {};

    for (size_t i = 0; i < m_iNumKeyFrames; i++)
    {
        KEYFRAME            KeyFrame = {};

        if (i < pAIChannel->mNumScalingKeys)
        {
            memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof vScale);
            KeyFrame.fTrackPosition = pAIChannel->mScalingKeys[i].mTime;
        }

        if (i < pAIChannel->mNumRotationKeys)
        {
            // memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof vRotation);
            vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
            vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
            vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
            vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
            KeyFrame.fTrackPosition = pAIChannel->mRotationKeys[i].mTime;
        }

        if (i < pAIChannel->mNumPositionKeys)
        {
            memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof vTranslation);
            KeyFrame.fTrackPosition = pAIChannel->mPositionKeys[i].mTime;
        }

        KeyFrame.vScale = vScale;
        KeyFrame.vRotation = vRotation;
        KeyFrame.vTranslation = vTranslation;

        m_KeyFrames.push_back(KeyFrame);
    }

    return S_OK;
}

HRESULT CChannel::Initialize_Binary(int32_t boneIndex, uint32_t NumKeyFrames, vector<KEYFRAME> keyFrames)
{
    m_iBoneIndex = boneIndex;
    m_iNumKeyFrames = NumKeyFrames;
    m_KeyFrames = keyFrames;

    return S_OK;
}

void CChannel::Update_TransformationMatrix(_float fCurrentTrackPosition, const vector<shared_ptr<CBone>>& Bones)
{
    KEYFRAME        LastKeyFrame = m_KeyFrames.back();

    _vector         vScale, vRotation, vTranslation;

    if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
    {
        vScale = XMLoadFloat3(&LastKeyFrame.vScale);
        vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
    }
    else
    {
        if (fCurrentTrackPosition >= m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTrackPosition)
            ++m_iCurrentKeyFrameIndex;

        _float      fRatio = (fCurrentTrackPosition - m_KeyFrames[m_iCurrentKeyFrameIndex].fTrackPosition) /
            (m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[m_iCurrentKeyFrameIndex].fTrackPosition);

        vScale = XMVectorLerp(
            XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale),
            XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale),
            fRatio
        );

        vRotation = XMQuaternionSlerp(
            XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation),
            XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation),
            fRatio
        );

        vTranslation = XMVectorSetW(XMVectorLerp(
            XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vTranslation),
            XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vTranslation),
            fRatio
        ), 1.f);
    }

    _matrix         TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

shared_ptr<CChannel> CChannel::Create(const aiNodeAnim* pAIChannel, class CModel* pModel)
{
    auto		pInstance = shared_ptr<CChannel>(new CChannel());

    if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
    {
        MSG_BOX("Failed to Created : CChannel");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CChannel> CChannel::Create_Binary(int32_t boneIndex, uint32_t NumKeyFrames, vector<KEYFRAME> keyFrames)
{
    auto		pInstance = shared_ptr<CChannel>(new CChannel());

    if (FAILED(pInstance->Initialize_Binary(boneIndex, NumKeyFrames, keyFrames)))
    {
        MSG_BOX("Failed to Created : CChannel");
        return nullptr;
    }

    return pInstance;
}

