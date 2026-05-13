#pragma once
#include "Engine_Defines.h"
#include "Bone.h"
NS_BEGIN(Engine)
class CAnimation
{
private:
	CAnimation();
public:
	~CAnimation();

public:
	HRESULT Initialize(const aiAnimation* pAiAnimation, class CModel* pModel);
	HRESULT Initialize_Binary(_char* animName, _float duration,_float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>>channels);
	_bool Update_TransformationMatrices(_float fTimeDelta, const vector<shared_ptr<class CBone>>& Bones, _bool isLoop,_bool animChanged);
	void Update_Blending_Matrices(_float fTimeDelta, const vector<shared_ptr<class CBone>>& Bones, const vector<CBone::SRT_DATA>& snapShot, _float ratio, _bool animChanged);
	const _char* Get_Name() { return m_szName; }
public:
	vector<shared_ptr<class CChannel>>& Get_Channels() { return m_Channels; }
	_float &Get_Duration() { return m_fDuration; }
	_float& Get_TickPerSec() { return m_fTickPerSecond; }
	uint32_t& Get_NumbChannels() { return m_iNumChannels; }
		//_float
private:
	/* 이 애니메이션의 총 길이. */
	_char				m_szName[MAX_PATH] = {};
	_float				m_fDuration = {};
	_float				m_fTickPerSecond = {};
	_float				m_fCurrentTrackPosition = {};

	/* 컨트롤해야하는 뼈의 갯수 */
	uint32_t							m_iNumChannels = {};
	vector<shared_ptr<class CChannel>>	m_Channels;
	vector<uint32_t>					m_CurrentKeyFrameIndices;

public:
	static shared_ptr<CAnimation> Create(const aiAnimation* pAIAnimation, class CModel* pModel);
	static shared_ptr<CAnimation> Create_Binary(_char* animName, _float duration, _float tickPerSec, uint32_t NumChannels, vector<shared_ptr<class CChannel>>channels);
	shared_ptr<CAnimation> Clone();


};

NS_END