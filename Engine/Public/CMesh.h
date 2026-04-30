#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	vector<VTXMESH> nonAnim_vertices;
	vector<VTXANIMMESH> Anim_vertices;
	vector<uint32_t> indices_;
	vector<Texture> textures_;
private:
	CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CMesh();

	uint32_t Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	HRESULT Initialize(uint32_t eType, class CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Bind_BoneMatrices(const vector<shared_ptr<class CBone>>& Bones, shared_ptr<class CShader> pShader, const _char* pConstantName);

	template <typename T>
	HRESULT Initialize_Binary(uint32_t matIndex, const vector<T>& vertices, const vector<uint32_t>& indices);

	// 2. Create_Binary를 템플릿으로 변경
	template <typename T>
	static shared_ptr<CMesh> Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		const vector<T>& vertices, const vector<uint32_t>& indices, uint32_t matIndex);

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