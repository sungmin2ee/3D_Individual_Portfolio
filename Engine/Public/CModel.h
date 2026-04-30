#pragma once

#include "Component.h"
#include "CMesh.h"
#include "Material.h"
#include "Bone.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CModel(const CModel& Prototype);
public:
	virtual ~CModel();

	uint32_t Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	int32_t Get_BoneIndex(const _char* pBoneName);

public:
	virtual HRESULT Initialize_Prototype(uint32_t eModelType, const _string& strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	void Play_Animation(_float fTimeDelta);
	HRESULT Bind_Materials(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iMeshIndex, aiTextureType eMaterialType, uint32_t iTextureIndex);
	HRESULT Bind_BoneMatrices(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iMeshIndex);


public:
	HRESULT Render(uint32_t iMeshIndex);

private:
	uint32_t						m_eModelType = {};
	const aiScene* m_pAIScene = { nullptr };
	Assimp::Importer		m_Importer = {};
	_float4x4				m_PreTransformMatrix = {};
private:
	uint32_t					m_iNumMeshes = {};
	vector<shared_ptr<CMesh>>	m_Meshes;
	uint32_t						m_iNumMaterials;
	vector<shared_ptr<CMaterial>>	m_Materials;
	vector<shared_ptr<CBone>>		m_Bones;

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_BinaryMeshes(string binPath, uint32_t ModelType);

	HRESULT Ready_Materials(const _string& strModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAINode, int32_t iParentBoneIndex);



public:
	static unique_ptr<CModel> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, uint32_t eModelType, const _string& strModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	void Free();
	const _float3 GetMin() { return min; }
	const _float3 GetMax() { return max; }
	void Update_Box(uint32_t modelType);
	void Calculate_Box(uint32_t modelType);
	uint32_t Get_ModelType() { return m_eModelType; }
private:

	_float3 max = { 0.f,0.f,0.f };
	_float3 min = { 0.f,0.f,0.f };





};

NS_END