#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	vector<VTXMESH> vertices_;
	vector<UINT> indices_;
	vector<Texture> textures_;
private:
	CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CMesh();

	uint32_t Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	HRESULT Initialize(const aiMesh* pAIMesh);
	HRESULT Initialize_Binary(uint32_t matIndex, const vector<VTXMESH>& vertices, const vector<uint32_t>& indices);

public:
	static shared_ptr<CMesh> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const aiMesh* pAIMesh);
	static shared_ptr<CMesh> Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		const vector<VTXMESH>& vertices, const vector<uint32_t>& indices, uint32_t matIndex);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

private:
	uint32_t		m_iMaterialIndex = {};

};

NS_END