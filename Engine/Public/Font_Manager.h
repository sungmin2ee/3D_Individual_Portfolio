#pragma once
#include "Engine_Defines.h"
#include "GameInstance.h"

NS_BEGIN(Engine)

class CFont_Manager
{
private:
	CFont_Manager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
public:
	virtual ~CFont_Manager();

public:
	HRESULT InitializeFont(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);
	void Begin() { m_spriteBatch->Begin(); }
	void End() { m_spriteBatch->End(); }
	void RenderText(uint32_t fontIndex,const _wstring& text, _float posX, _float posY, _vector color, _float scale = 1.0f);
public:
	static unique_ptr<CFont_Manager> Create(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context);

private:
	std::unique_ptr<SpriteBatch> m_spriteBatch;
	std::unique_ptr<SpriteFont> m_spriteRegularFont;
	std::unique_ptr<SpriteFont> m_spriteBoldFont;
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pContext = nullptr;;
};

NS_END
