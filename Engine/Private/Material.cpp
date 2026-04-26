#include "Material.h"
#include "GameInstance.h"

CMaterial::CMaterial(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::Initialize(const aiMaterial* pAIMaterial, const _string& strModelFilePath)
{
	_char	szDrive[MAX_PATH] = { };
	_char	szDir[MAX_PATH] = { };

	_splitpath_s(strModelFilePath.c_str(), szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);


	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{

		uint32_t		iNumTextures = pAIMaterial->GetTextureCount(static_cast<aiTextureType>(i));

		for (size_t j = 0; j < iNumTextures; j++)
		{
			_char	szFileName[MAX_PATH] = { };
			_char	szExt[MAX_PATH] = { };

			aiString		strTexturePath = {};

			if (FAILED(pAIMaterial->GetTexture(static_cast<aiTextureType>(i), j, &strTexturePath)))
				return E_FAIL;

			_splitpath_s(strTexturePath.C_Str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char	szFullPath[MAX_PATH] = {};
			string Dir = "../../Resources/Textures/";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, Dir.c_str());
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			HRESULT         hr = {};
			ComPtr<ID3D11ShaderResourceView>		pSRV = { nullptr };

			_tchar	szFinalPath[MAX_PATH] = {};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), 
				szFinalPath, MAX_PATH);



			if (false == strcmp(szExt, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice.Get(), szFinalPath, nullptr, &pSRV);

			else if (false == strcmp(szExt, ".tga"))
				hr = E_FAIL;
			else
				hr = CreateWICTextureFromFile(m_pDevice.Get(), szFinalPath, nullptr, &pSRV);

			m_Materials[i].push_back(pSRV);
			m_TexturePaths[i].push_back(szFullPath);
		}
	}

	


	return S_OK;
}

HRESULT CMaterial::Initialize_Binary(const vector<string> texturePaths[AI_TEXTURE_TYPE_MAX])
{
	// 이미 바이너리에 전체 경로(Full Path)를 저장하셨다면 
	// 여기서 szDrive, szDir를 이용해 경로를 재생성할 필요가 없습니다.
	// 만약 파일명만 저장하셨다면 기존처럼 조립 로직이 필요합니다.

	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++) // i는 Texture Type (Diffuse, Normal 등)
	{
		// 해당 타입(i)에 해당하는 경로들만 순회
		for (const string& strFullPath : texturePaths[i])
		{
			ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
			HRESULT hr = S_OK;

			// 확장자 추출
			size_t dotIdx = strFullPath.find_last_of(".");
			string strExt = strFullPath.substr(dotIdx);

			// 유니코드 변환
			_tchar szFinalPath[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, strFullPath.c_str(), -1, szFinalPath, MAX_PATH);

			// 파일 로드
			if (strExt == ".dds")
				hr = CreateDDSTextureFromFile(m_pDevice.Get(), szFinalPath, nullptr, &pSRV);
			else if (strExt == ".tga")
				continue; // TGA는 별도 로더 필요
			else
				hr = CreateWICTextureFromFile(m_pDevice.Get(), szFinalPath, nullptr, &pSRV);

			if (FAILED(hr)) {
				// 여기서 실패한다면 파일 경로 szFinalPath가 실제 존재하는지 확인해야 합니다.
				OutputDebugStringW(L"Texture Load Failed: ");
				OutputDebugStringW(szFinalPath);
				OutputDebugStringW(L"\n");
			}
			if (SUCCEEDED(hr))
			{
				m_Materials[i].push_back(pSRV);
				m_TexturePaths[i].push_back(strFullPath);
			}
		}
	}

	return S_OK;
}

HRESULT CMaterial::Bind_ShaderResource(shared_ptr<class CShader> pShader, const _char* pConstantName, aiTextureType eMaterialType, uint32_t iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName,
		m_Materials[eMaterialType][iTextureIndex]);
	
}

shared_ptr<CMaterial> CMaterial::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const aiMaterial* pAIMaterial, const _string& strModelFilePath)
{
	auto	pInstance = shared_ptr<CMaterial>(new CMaterial(pDevice, pContext));

	if (FAILED(pInstance->Initialize(pAIMaterial, strModelFilePath)))
		MSG_BOX("Failed to Created : CMainApp");

	return pInstance;
}

shared_ptr<CMaterial> CMaterial::Create_Binary(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, vector<string>* pTexturePaths)
{
	auto	pInstance = shared_ptr<CMaterial>(new CMaterial(pDevice, pContext));

	if (FAILED(pInstance->Initialize_Binary(pTexturePaths)))
		MSG_BOX("Failed to Created : CMainApp");

	return pInstance;
}

