#pragma once

#include "Component.h"
#include "CMesh.h"
#include "Material.h"

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

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const _string& strModelFilePath);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_Materials(shared_ptr<class CShader> pShader, const _char* pConstantName, uint32_t iMeshIndex, aiTextureType eMaterialType, uint32_t iTextureIndex);

public:
	HRESULT Render(uint32_t iMeshIndex);

private:
	MODEL						m_eModelType = {};
	const aiScene* m_pAIScene = { nullptr };
	Assimp::Importer		m_Importer = {};

private:
	uint32_t					m_iNumMeshes = {};
	vector<shared_ptr<CMesh>>	m_Meshes;
	uint32_t						m_iNumMaterials;
	vector<shared_ptr<CMaterial>>	m_Materials;
private:
	HRESULT Ready_Meshes();
	HRESULT Ready_BinaryMeshes(string binPath);

	HRESULT Ready_Materials(const _string& strModelFilePath);



public:
	static unique_ptr<CModel> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, MODEL eModelType, const _string& strModelFilePath);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	void Free();
	const _float3 GetMin() { return min; }
	const _float3 GetMax() { return max; }
	void Update_Box();
	void Calculate_Box();
private:

	_float3 max = { 0.f,0.f,0.f };
	_float3 min = { 0.f,0.f,0.f };





};

NS_END