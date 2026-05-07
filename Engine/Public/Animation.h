#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class CAnimation
{
private:
	CAnimation();
public:
	~CAnimation();

public:
	HRESULT Initialize(const aiAnimation* pAiAnimation, class CModel* pModel);
	HRESULT Initialize_Binary(_float duration,_float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>>channels);
	void Update_TransformationMatrices(_float fTimeDelta, const vector<shared_ptr<class CBone>>& Bones);

public:
	vector<shared_ptr<class CChannel>>& Get_Channels() { return m_Channels; }
	_float &Get_Duration() { return m_fDuration; }
	_float& Get_TickPerSec() { return m_fTickPerSecond; }
	uint32_t& Get_NumbChannels() { return m_iNumChannels; }
		//_float
private:
	/* 이 애니메이션의 총 길이. */
	_float				m_fDuration = {};
	_float				m_fTickPerSecond = {};
	_float				m_fCurrentTrackPosition = {};

	/* 컨트롤해야하는 뼈의 갯수 */
	uint32_t							m_iNumChannels = {};
	vector<shared_ptr<class CChannel>>	m_Channels;

public:
	static shared_ptr<CAnimation> Create(const aiAnimation* pAIAnimation, class CModel* pModel);
	static shared_ptr<CAnimation> Create_Binary(_float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>>channels);


};

NS_END