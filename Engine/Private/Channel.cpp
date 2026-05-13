#include "Channel.h"
#include "CModel.h"


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

void CChannel::Update_TransformationMatrix(uint32_t& iCurrentKeyFrameIndex, _float fCurrentTrackPosition, const vector<shared_ptr<class CBone>>& Bones, _bool animChanged)
{
    if (0.f == fCurrentTrackPosition || animChanged)
        iCurrentKeyFrameIndex = 0;

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
        while (fCurrentTrackPosition >= m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition)
            ++iCurrentKeyFrameIndex;

        _float      fRatio = (fCurrentTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition) /
            (m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition);

        vScale = XMVectorLerp(
            XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vScale),
            XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vScale),
            fRatio
        );
        vRotation = XMQuaternionSlerp(
            XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex].vRotation),
            XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vRotation),
            fRatio
        );
        vTranslation = XMVectorSetW(XMVectorLerp(
            XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vTranslation),
            XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vTranslation),
            fRatio
        ), 1.f);
    }

    _matrix         TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Update_Blending_Matrices(uint32_t& iCurrentKeyFrameIndex, _float fCurrentTrackPosition, 
    const vector<shared_ptr<class CBone>>& Bones, const vector<CBone::SRT_DATA>& snapShot, _float ratio, _bool animChanged)
{
    _vector vNewScale, vNewRotation, vNewTranslation;
    Get_SRT_At_Time(fCurrentTrackPosition, iCurrentKeyFrameIndex, vNewScale, vNewRotation, vNewTranslation, animChanged);

    // 2. Snapshot과 새 애니메이션의 SRT를 Blending Ratio로 섞는다
    _vector vFinalScale = XMVectorLerp(snapShot[m_iBoneIndex].vScale, vNewScale, ratio);
    _vector vFinalRotation = XMQuaternionSlerp(snapShot[m_iBoneIndex].vRotation, vNewRotation, ratio);
    _vector vFinalTranslation = XMVectorLerp(snapShot[m_iBoneIndex].vTranslation, vNewTranslation, ratio);

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vFinalScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vFinalRotation, vFinalTranslation);
    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}


void CChannel::Get_SRT_At_Time(_float fCurrentTrackPosition, uint32_t& iCurrentKeyFrameIndex, _vector& outScale, _vector& outRotation, _vector& outTranslation, _bool animChanged)
{

    if (0.f == fCurrentTrackPosition || animChanged)
        iCurrentKeyFrameIndex = 0;

    // 1. 현재 트랙 위치가 마지막 키프레임을 넘었을 때 처리
    if (fCurrentTrackPosition >= m_KeyFrames.back().fTrackPosition)
    {
        outScale = XMLoadFloat3(&m_KeyFrames.back().vScale);
        outRotation = XMLoadFloat4(&m_KeyFrames.back().vRotation);
        outTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames.back().vTranslation), 1.f);
        return;
    }

    // 2. 현재 시간에 해당하는 키프레임 인덱스 찾기 (최적화를 위해 이전 인덱스부터 시작)
    while (fCurrentTrackPosition >= m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition)
        iCurrentKeyFrameIndex++;

    // 3. 두 키프레임 사이의 비율(t) 구하기
    KEYFRAME& CurrentKeyFrame = m_KeyFrames[iCurrentKeyFrameIndex];
    KEYFRAME& NextKeyFrame = m_KeyFrames[iCurrentKeyFrameIndex + 1];

    _float fRatio = (fCurrentTrackPosition - CurrentKeyFrame.fTrackPosition) /
        (NextKeyFrame.fTrackPosition - CurrentKeyFrame.fTrackPosition);

    // 4. S, R, T 각각 보간
    _vector vPrevScale = XMLoadFloat3(&CurrentKeyFrame.vScale);
    _vector vNextScale = XMLoadFloat3(&NextKeyFrame.vScale);
    outScale = XMVectorLerp(vPrevScale, vNextScale, fRatio);

    _vector vPrevRotation = XMLoadFloat4(&CurrentKeyFrame.vRotation);
    _vector vNextRotation = XMLoadFloat4(&NextKeyFrame.vRotation);
    outRotation = XMQuaternionSlerp(vPrevRotation, vNextRotation, fRatio);

    _vector vPrevTranslation = XMLoadFloat3(&CurrentKeyFrame.vTranslation);
    _vector vNextTranslation = XMLoadFloat3(&NextKeyFrame.vTranslation);
    outTranslation = XMVectorSetW(XMVectorLerp(vPrevTranslation, vNextTranslation, fRatio), 1.f);
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

