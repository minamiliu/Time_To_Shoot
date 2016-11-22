/*******************************************************************************
*
* �^�C�g��:		���͏���
* �v���O������:	input.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

/* Game Pad  */
#define BUTTON_A		(0)
#define BUTTON_B		(1)
#define BUTTON_X		(2)
#define BUTTON_Y		(3)
#define BUTTON_L1		(4)
#define BUTTON_R1		(5)
#define BUTTON_SELECT	(6)
#define BUTTON_START	(7)
#define BUTTON_L3		(8)
#define BUTTON_R3		(9)

#define LSTICK_UP		(10)	
#define LSTICK_DOWN		(11)	
#define LSTICK_LEFT		(12)	
#define LSTICK_RIGHT	(13)

#define RSTICK_UP		(14)	
#define RSTICK_DOWN		(15)	
#define RSTICK_LEFT		(16)	
#define RSTICK_RIGHT	(17)	

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �L�[�{�[�h�֘A
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// �}�E�X�֘A
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);

// �W���C�p�b�h�֘A
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
void UninitJoypad(void);
void UpdateJoypad(void);
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);

bool GetGamePadTrigger( int padNo, DWORD button );
bool GetGamePadPress( int padNo, DWORD button );
long GetGamePadRightAxisX(int padNo);
long GetGamePadRightAxisY(int padNo);
#endif