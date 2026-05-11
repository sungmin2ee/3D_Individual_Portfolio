#include "Animation.h"
#include "Channel.h"
#include "Bone.h"

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
}

HRESULT CAnimation::Initialize(const aiAnimation* pAiAnimation, CModel* pModel)
{
    strcpy_s(m_szName, MAX_PATH, pAiAnimation->mName.C_Str());
    m_fDuration = pAiAnimation->mDuration;
    m_fTickPerSecond = pAiAnimation->mTicksPerSecond;

    m_iNumChannels = pAiAnimation->mNumChannels;

    m_CurrentKeyFrameIndices.resize(m_iNumChannels);

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        shared_ptr<CChannel>    pChannel = CChannel::Create(pAiAnimation->mChannels[i], pModel);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

    return S_OK;
}

HRESULT CAnimation::Initialize_Binary(_char* animName, _float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>> channels)
{
    strcpy_s(m_szName, MAX_PATH, animName);

    m_fDuration = duration;

    m_fTickPerSecond = tickPerSec;

    m_iNumChannels = NumChannels;

    m_Channels = channels;

    m_CurrentKeyFrameIndices.resize(m_iNumChannels);

    return S_OK;
}


_bool CAnimation::Update_TransformationMatrices(_float fTimeDelta, const vector<shared_ptr<class CBone>>& Bones,_bool isLoop)
{
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    if (m_fCurrentTrackPosition >= m_fDuration)
    {
        if (true == isLoop)
            m_fCurrentTrackPosition = 0.f;
        else
            return true;
    }



    for (uint32_t i = 0; i < m_iNumChannels; ++i)
    {
        m_Channels[i]->Update_TransformationMatrix(m_CurrentKeyFrameIndices[i], m_fCurrentTrackPosition, Bones);
    }

    return false;


}

shared_ptr<CAnimation> CAnimation::Create(const aiAnimation* pAIAnimation, CModel* pModel)
{
    auto		pInstance = shared_ptr<CAnimation>(new CAnimation());

    if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
    {
        MSG_BOX("Failed to Created : CAnimation");
        return nullptr;
    }

    return pInstance;
}

shared_ptr<CAnimation> CAnimation::Create_Binary(_char* animName,_float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>> channels)
{
    auto		pInstance = shared_ptr<CAnimation>(new CAnimation());

    if (FAILED(pInstance->Initialize_Binary(animName, duration, tickPerSec, NumChannels, channels)))
    {
        MSG_BOX("Failed to Created : CAnimation");
        return nullptr;
    }
    return pInstance;
}
shared_ptr<CAnimation> CAnimation::Clone()
{
    return shared_ptr<CAnimation>(new CAnimation(*this));

}
