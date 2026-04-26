#pragma once

#include "Engine_Defines.h"

NS_BEGIN(Engine)

class CMaterial
{
private:
	CMaterial(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	~CMaterial();

public:
	HRESULT Initialize(const aiMaterial* pAIMaterial, const _string& strModelFilePath);
	HRESULT Initialize_Binary(const vector<string> texturePaths[AI_TEXTURE_TYPE_MAX]);
	HRESULT Bind_ShaderResource(shared_ptr<class CShader> pShader, const _char* pConstantName, aiTextureType eMaterialType, uint32_t iTextureIndex);
	const string& Get_TexturePath(uint32_t iType, uint32_t iIndex) {
		return m_TexturePaths[iType][iIndex];
	}
	uint32_t Get_TextureCount(uint32_t iType) {
		return static_cast<uint32_t>(m_TexturePaths[iType].size());
	}
private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };

private:
	vector<ComPtr<ID3D11ShaderResourceView>>			m_Materials[AI_TEXTURE_TYPE_MAX];
	vector<string>										m_TexturePaths[AI_TEXTURE_TYPE_MAX];

public:
	static shared_ptr<CMaterial> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const aiMaterial* pAIMaterial, const _string& strModelFilePath);
	static shared_ptr<CMaterial> Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, vector<string>* pTexturePaths);
};

NS_END