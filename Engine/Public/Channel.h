#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class CChannel final
{
private:
	CChannel();
public:
	~CChannel();

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, class CModel* pModel);
	HRESULT Initialize_Binary(int32_t boneIndex, uint32_t NumKeyFrames, vector<KEYFRAME> keyFrames);
	void Update_TransformationMatrix(_float fCurrentTrackPosition, const vector<shared_ptr<class CBone>>& Bones);

public:
	vector<KEYFRAME>& Get_KeyFrames() { return m_KeyFrames; }
	int32_t &Get_BoneIndex() { return m_iBoneIndex; }
	uint32_t &Get_NumKeyFrames() { return m_iNumKeyFrames; }
private:
	char				m_szName[MAX_PATH] = {};
	int32_t				m_iBoneIndex = {};
	uint32_t			m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;
	uint32_t			m_iCurrentKeyFrameIndex = {  };

public:
	static shared_ptr<CChannel> Create(const aiNodeAnim* pAIChannel, class CModel* pModel);
	static shared_ptr<CChannel> Create_Binary(int32_t boneIndex, uint32_t NumKeyFrames,vector<KEYFRAME> keyFrames);
};

NS_END