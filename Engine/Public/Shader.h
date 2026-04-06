#pragma once
#include "Engine_Defines.h"
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL Shader : public CComponent
{
private:
	Shader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	~Shader();
private:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
public:
	virtual shared_ptr<CPrototype> Clone(void* pArg);
	HRESULT Bind_Texture(const string& strTag, ComPtr<ID3D11ShaderResourceView> pSRV);
	static unique_ptr<Shader> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	HRESULT Bind_Matrix(const MatrixBuffer& tBuffer);
	ComPtr<ID3D11VertexShader> GetVS() { return m_pVS; }
	ComPtr<ID3D11PixelShader> GetPS() { return m_pPS; }
	ComPtr<ID3D11SamplerState> GetSampler() { return m_pSamplerState; }
	ComPtr<ID3D11InputLayout> GetLayout() { return m_pLayout; }

private:
	ComPtr<ID3D11Buffer>							m_pConstantBuffer = { nullptr };
	ComPtr<ID3D11Buffer>							m_pBoneBuffer = { nullptr };
	ComPtr<ID3D11VertexShader>						m_pVS = { nullptr };
	ComPtr<ID3D11PixelShader>						m_pPS = { nullptr };
	ComPtr<ID3D11InputLayout>						m_pLayout = { nullptr };
	ComPtr<ID3D11SamplerState>						m_pSamplerState = { nullptr };
	ComPtr<ID3D11Device>							m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>						m_pContext = { nullptr };
};

NS_END

