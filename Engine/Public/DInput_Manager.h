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
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byKeyState[byKeyID];
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[eMouse];
	}

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

	bool Key_Pressing(_ubyte byKeyID) { return m_bKeyPressingState[byKeyID]; }
	bool Key_Up(_ubyte byKeyID) { return m_bKeyUpState[byKeyID]; }
	bool Key_Down(_ubyte byKeyID) { return m_bKeyDownState[byKeyID]; }

	bool Mouse_Pressing(MOUSEKEYSTATE eMouseState) { return m_bMousePressingState[eMouseState]; }
	bool Mouse_Up(MOUSEKEYSTATE eMouseState) { return m_bMouseUpState[eMouseState]; }
	bool Mouse_Down(MOUSEKEYSTATE eMouseState) { return m_bMouseDownState[eMouseState]; }

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	ComPtr<IDirectInput8W>			m_pInputSDK = nullptr;

private:
	ComPtr<IDirectInputDevice8W>	m_pKeyBoard = nullptr;
	ComPtr<IDirectInputDevice8W> 	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;

private:
	bool					m_bKeyPressingState[256];
	bool					m_bKeyUpState[256];
	bool					m_bKeyDownState[256];

	bool					m_bMousePressingState[DIM_END];
	bool					m_bMouseUpState[DIM_END];
	bool					m_bMouseDownState[DIM_END];

public:
	static unique_ptr<CDInput_Manager> Create();

};
NS_END


