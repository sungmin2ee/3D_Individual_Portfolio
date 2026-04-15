#pragma once
#include "Engine_Defines.h"
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	CShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	~CShader();
private:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath,
		const D3D11_INPUT_ELEMENT_DESC* pInputElements, uint32_t iNumElements);
	virtual HRESULT Initialize(void* pArg);
public:
	HRESULT Begin(uint32_t iPassIndex);
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);

	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, uint32_t iSize);

	HRESULT Bind_Texture(const _char* name, ComPtr<ID3D11ShaderResourceView> pSRV);
	HRESULT Apply_Pass();
public:
	virtual shared_ptr<CPrototype> Clone(void* pArg);
	static unique_ptr<CShader> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pInputElements, uint32_t iNumElements);

private:



	ComPtr<ID3DX11Effect>			m_pEffect = { nullptr };

	uint32_t							m_iNumPasses = {};
	vector<ComPtr<ID3D11InputLayout>>	m_InputLayouts;
	uint32_t m_iCurrentPassIndex;
};

NS_END

