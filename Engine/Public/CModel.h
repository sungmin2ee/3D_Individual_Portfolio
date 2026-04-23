#pragma once

#include "Component.h"
#include "CMesh.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CModel(const CModel& Prototype);
public:
	virtual ~CModel();

public:
	virtual HRESULT Initialize_Prototype(string pModelFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Render();

private:
	const aiScene* m_pAIScene = { nullptr };
	Assimp::Importer		m_Importer = {};

private:
	uint32_t					m_iNumMeshes = {};
	vector<shared_ptr<CMesh>>	m_Meshes;

private:
	HRESULT Ready_Meshes();
	HRESULT Ready_BinaryMeshes(string binPath);


public:
	static unique_ptr<CModel> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, string pModelFilePath);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

	void Free();






};

NS_END