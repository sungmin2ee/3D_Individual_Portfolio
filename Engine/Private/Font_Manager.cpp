#include "Font_Manager.h"

CFont_Manager::CFont_Manager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
}

CFont_Manager::~CFont_Manager()
{
}

HRESULT CFont_Manager::InitializeFont(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
	m_spriteBatch = make_unique<SpriteBatch>(context.Get());

	// 파일 이름이 MyFont.spritefont 라면:
	m_spriteRegularFont = make_unique<SpriteFont>(device.Get(), L"../../Resources/Font/MyFont_Regular.spritefont");
	m_spriteBoldFont = make_unique<SpriteFont>(device.Get(), L"../../Resources/Font/MyFont_Bold.spritefont");
	return S_OK;
}

void CFont_Manager::RenderText(uint32_t fontIndex ,const _wstring& text, _float posX, _float posY, _vector color, _float scale)
{
	switch (fontIndex) {
	case 0:
		//regular font 
		m_spriteRegularFont->DrawString(m_spriteBatch.get(), text.c_str(),
			XMFLOAT2(posX, posY), color, 0.f, XMFLOAT2(0, 0), scale);
		break;
	case 1:
		m_spriteBoldFont->DrawString(m_spriteBatch.get(), text.c_str(),
			XMFLOAT2(posX, posY), color, 0.f, XMFLOAT2(0, 0), scale);
		break;
	}
	
}

unique_ptr<CFont_Manager> CFont_Manager::Create(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
	auto pInstance = unique_ptr<CFont_Manager>(new CFont_Manager(device, context));

	if (FAILED(pInstance->InitializeFont(device, context)))
		return nullptr;

	return pInstance;
}

