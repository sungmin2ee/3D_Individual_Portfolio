#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(ComPtr<ID3D11Device>	pDevice, ComPtr<ID3D11DeviceContext> pContext);

public:
	virtual ~CVIBuffer_Rect();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static shared_ptr<CVIBuffer_Rect> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CPrototype> Clone(void* pArg) override;

};

NS_END