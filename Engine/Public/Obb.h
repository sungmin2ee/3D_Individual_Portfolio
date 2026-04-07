#pragma once
#include "Engine_Defines.h"
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL Obb :
    public CVIBuffer
{

private:
	Obb(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~Obb();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	const BoundingOrientedBox GetObb() { return myOBB; }

	void Render_Debug();
public:
	static shared_ptr<Obb> Create(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;
	
public:
	BoundingOrientedBox myOBB;


private:
	uint16_t* pIndices;
	VOBB* pVertices;
	ComPtr<ID3D11VertexShader>						m_pVS = { nullptr };
	ComPtr<ID3D11PixelShader>						m_pPS = { nullptr };
	ComPtr<ID3D11InputLayout>						m_pLayout = { nullptr };
	ComPtr<ID3D11SamplerState>						m_pSamplerState = { nullptr };
	//ComPtr<ID3D11Buffer>							m_pConstantBuffer = { nullptr };
	//ComPtr<ID3D11VertexShader>						m_pVS = { nullptr };
	//ComPtr<ID3D11PixelShader>						m_pPS = { nullptr };
	//ComPtr<ID3D11InputLayout>						m_pLayout = { nullptr };
	//ComPtr<ID3D11SamplerState>						m_pSamplerState = { nullptr };
	//ComPtr<ID3D11Device>							m_pDevice = { nullptr };
	//ComPtr<ID3D11DeviceContext>						m_pContext = { nullptr };
};
NS_END

