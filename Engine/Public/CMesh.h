#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	vector<VERTEX> vertices_;
	vector<UINT> indices_;
	vector<Texture> textures_;
private:
	CMesh(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CMesh();


public:
	HRESULT Initialize(const aiMesh* pAIMesh);
	HRESULT Initialize_Binary(const vector<VTXMESH>& vertices, const vector<uint32_t>& indices);

public:
	static shared_ptr<CMesh> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const aiMesh* pAIMesh);
	static shared_ptr<CMesh> Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext,
		const vector<VTXMESH>& vertices, const vector<uint32_t>& indices);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
};

NS_END