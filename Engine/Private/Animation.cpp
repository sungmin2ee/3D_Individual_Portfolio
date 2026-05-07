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
    m_fDuration = pAiAnimation->mDuration;

    m_fTickPerSecond = pAiAnimation->mTicksPerSecond;

    m_iNumChannels = pAiAnimation->mNumChannels;

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        shared_ptr<CChannel>    pChannel = CChannel::Create(pAiAnimation->mChannels[i], pModel);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

	return S_OK;
}

HRESULT CAnimation::Initialize_Binary(_float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>> channels)
{
    m_fDuration = duration;

    m_fTickPerSecond = tickPerSec;

    m_iNumChannels = NumChannels;

    m_Channels = channels;
    return S_OK;
}


void CAnimation::Update_TransformationMatrices(_float fTimeDelta, const vector<shared_ptr<class CBone>>& Bones)
{
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    if (m_fCurrentTrackPosition >= m_fDuration)
    {
        // 局聪皋捞记 场车促.
        m_fCurrentTrackPosition = 0;
    }

    for (auto& pChannel : m_Channels)
    {
        pChannel->Update_TransformationMatrix(m_fCurrentTrackPosition, Bones);
    }


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

shared_ptr<CAnimation> CAnimation::Create_Binary(_float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>> channels)
{
    auto		pInstance = shared_ptr<CAnimation>(new CAnimation());

    if (FAILED(pInstance->Initialize_Binary(duration, tickPerSec, NumChannels, channels)))
    {
        MSG_BOX("Failed to Created : CAnimation");
        return nullptr;
    }
    return pInstance;
}