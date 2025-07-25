/*******************************************************************************
*
* タイトル:		入力処理
* プログラム名:	input.h
* 作成者:		HAL東京ゲーム学科　劉南宏
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
// プロトタイプ宣言
//*****************************************************************************
// キーボード関連
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// マウス関連
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

// ジョイパッド関連
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
void UninitJoypad(void);
void UpdateJoypad(void);
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);

bool GetGamePadTrigger( int padNo, DWORD button );
bool GetGamePadPress( int padNo, DWORD button );
long GetGamePadRightAxisX(int padNo);
long GetGamePadRightAxisY(int padNo);
#endif