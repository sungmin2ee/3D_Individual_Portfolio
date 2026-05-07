#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	vector<VTXMESH> nonAnim_vertices;
	vector<VTXANIMMESH> Anim_vertices;
	vector<uint32_t> indices_;
private:
	CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CMesh();

	uint32_t Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	_char*	Get_BoneName()& { return m_szName; }
	vector<uint32_t>& Get_BoneIndices() { return m_BoneIndices;}
	vector<_float4x4>& Get_BoneMatrices() { return m_BoneMatrices; }
	vector<_float4x4>& Get_OffsetMatrices() { return m_OffsetMatrices; }
	uint32_t Get_NumBones()& { return m_iNumBones; }
	void Set_NumBones(uint32_t num) { m_iNumBones = num; }
	void Set_OffsetMatrices(vector<_float4x4>offsetMat) { m_OffsetMatrices = offsetMat; }
	void Set_BoneIndices(vector<uint32_t> boneIndices) { m_BoneIndices = boneIndices; }
	void Set_Name(_char* name) {
		strcpy_s(m_szName, name); }
public:
	HRESULT Initialize(uint32_t eType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Bind_BoneMatrices(const vector<shared_ptr<class CBone>>& Bones, shared_ptr<class CShader> pShader, const _char* pConstantName);

	template <typename T>
	HRESULT Initialize_Binary(uint32_t matIndex, const vector<T>& vertices, const vector<uint32_t>& indices, uint32_t numBones);

	// 2. Create_Binary를 템플릿으로 변경
	template <typename T>
	static shared_ptr<CMesh> Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		const vector<T>& vertices, const vector<uint32_t>& indices, uint32_t matIndex, uint32_t numBones);

public:
	static shared_ptr<CMesh> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, uint32_t eType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);



	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
private:
	HRESULT Ready_NonAnimMesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT	Ready_AnimMesh(class CModel* pModel, const aiMesh* pAIMesh);

private:
	uint32_t		m_iMaterialIndex = {};
	uint32_t		m_iNumBones = {}; /* 이 메시가 이용하는 뼈의 갯수. */

	/*  이 메시에 영향을 주는 뼈들의 전체뼈기준의 인덱스 */
	_char				m_szName[MAX_PATH] = {};
	vector<uint32_t>	m_BoneIndices;
	vector<_float4x4>	m_BoneMatrices;
	vector<_float4x4>	m_OffsetMatrices;

};

NS_END