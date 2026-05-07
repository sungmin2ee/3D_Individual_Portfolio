#pragma once

#include "Engine_Defines.h"

/* assimp 뼈 -> aiNode, aiBone, aiAnimNode */

NS_BEGIN(Engine)

class CBone final 
{
private:
	CBone();
public:
	~CBone();

public:
	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}
public:
	HRESULT Initialize(const aiNode* pAINode, int32_t iParentIndex);
	HRESULT Initialize_Binary(_char* szName, _float4x4 transformationMat, int32_t iParentIndex);
	_bool Compare_Name(const _char* pBoneName) {
		return !strcmp(pBoneName, m_szName);
	}

	void Update_CombinedTransformationMatrix(const vector<shared_ptr<CBone>>& Bones, _fmatrix PreTransformMatrix);

public:
	const _char* Get_BoneName()& { return m_szName; }
	const int32_t Get_ParentBoneIndex()& { return m_iParentBoneIndex; }
	const _float4x4 Get_TransformationMatrix()& {
		return m_TransformationMatrix;
	}
private:
	_char			m_szName[MAX_PATH] = {  };
	_float4x4		m_TransformationMatrix = { }; /* 이 뼈만의 상태행렬 */
	_float4x4		m_CombinedTransformationMatrix = {}; /* 부모 뼈의 상태를 포함한 최종 행렬 */
	int32_t			m_iParentBoneIndex = { -1 };

public:
	static shared_ptr<CBone> Create(const aiNode* pAINode, int32_t iParentIndex);
	static shared_ptr<CBone> Create_Binary(_char* szName, _float4x4 transformationMat, int32_t iParentIndex);
};

NS_END