#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)

class  CDInput_Manager
{

private:
	CDInput_Manager();
public:
	~CDInput_Manager();

public:
	int8_t	Get_DIKeyState(uint8_t byKeyID)
	{
		return m_byKeyState[byKeyID];
	}

	int8_t	Get_DIMouseState(DIMK eMouse)
	{
		return m_tMouseState.rgbButtons[ETOUI(eMouse)];
	}

	// 현재 마우스의 특정 축 좌표를 반환
	int32_t	Get_DIMouseMove(DIMM eMouseState)
	{
		return *((reinterpret_cast<int32_t*>(&m_tMouseState)) + ETOUI(eMouseState));
	}

	bool Key_Pressing(uint8_t byKeyID) { return m_bKeyPressingState[byKeyID]; }
	bool Key_Up(uint8_t byKeyID) { return m_bKeyUpState[byKeyID]; }
	bool Key_Down(uint8_t byKeyID) { return m_bKeyDownState[byKeyID]; }

	bool Mouse_Pressing(DIMK eMouseState) { return m_bMousePressingState[ETOUI(eMouseState)]; }
	bool Mouse_Up(DIMK eMouseState) { return m_bMouseUpState[ETOUI(eMouseState)]; }
	bool Mouse_Down(DIMK eMouseState) { return m_bMouseDownState[ETOUI(eMouseState)]; }

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	ComPtr<IDirectInput8>			m_pInputSDK = nullptr;

private:
	ComPtr<IDirectInputDevice8>	m_pKeyBoard = nullptr;
	ComPtr<IDirectInputDevice8> 	m_pMouse = nullptr;

private:
	uint8_t					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;

private:
	bool					m_bKeyPressingState[256];
	bool					m_bKeyUpState[256];
	bool					m_bKeyDownState[256];

	bool					m_bMousePressingState[ETOUI(DIMM::END)];
	bool					m_bMouseUpState[ETOUI(DIMM::END)];
	bool					m_bMouseDownState[ETOUI(DIMM::END)];

public:
	static unique_ptr<CDInput_Manager> Create(HINSTANCE hInst, HWND hWnd);

};
NS_END


