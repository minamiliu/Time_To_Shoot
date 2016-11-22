/*******************************************************************************
*
* タイトル:		入力処理
* プログラム名:	input.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX				(256)	// キーの最大数
#define	LIMIT_COUNT_REPEAT		(20)	// リピートカウントリミッター
#define	NUM_MOUSE_BUTTON_MAX	(8)		// マウスのボタン最大数
#define	MOUSE_BUTTON_LEFT		(0)		// マウスの左ボタン
#define	MOUSE_BUTTON_RIGHT		(1)		// マウスの右ボタン
#define	MOUSE_BUTTON_CENTER		(2)		// マウスの中央ボタン

#define MAX_CONTROLER			(2)		// コントローラ最大数(使う数に応じて変更すること)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;					// 入力デバイス(マウス)へのポインタ
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ
DIMOUSESTATE2			g_mouseState;						// マウスの入力情報ワーク
DIMOUSESTATE2			g_mouseStateTrigger;				// マウスのトリガー情報ワーク

DIJOYSTATE				g_joyState[MAX_CONTROLER];			// ジョイパッド情報配列
DIJOYSTATE				g_joyStatePrev[MAX_CONTROLER];		// ジョイパッド情報配列（直前）
LPDIRECTINPUTDEVICE8	g_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8インターフェースへのポインタ(ジョイパッド)
int						g_nJoypadNum = 0;					// 見つかったジョイパッドの数
BYTE					g_joyStateTrigger[MAX_CONTROLER][32];

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;

	if(!g_pInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pInput, NULL);
	}

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// DirectInputオブジェクトの開放
	if(g_pInput)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	// 各ワークのクリア
	ZeroMemory(g_aKeyState, sizeof g_aKeyState);
	ZeroMemory(g_aKeyStateTrigger, sizeof g_aKeyStateTrigger);
	ZeroMemory(g_aKeyStateRelease, sizeof g_aKeyStateRelease);
	ZeroMemory(g_aKeyStateRepeat, sizeof g_aKeyStateRepeat);
	ZeroMemory(g_aKeyStateRepeatCnt, sizeof g_aKeyStateRepeatCnt);

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// 入力処理の終了処理
	UninitInput();

	if(g_pDevKeyboard)
	{
		// キーボードへのアクセス権を開放(入力処理終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & ~aKeyState[nCnKey];

			if(aKeyState[nCnKey])
			{
				if(g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if(g_aKeyStateRepeatCnt[nCnKey] == 1
					|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスのプロパティを設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_pDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "マウスのデバイスのプロパティを設定できませんでした。", "警告！", MB_ICONWARNING);
			return hr;
		}
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	g_pDevMouse->Acquire();

	//初期化
	ZeroMemory(&g_mouseState, sizeof DIMOUSESTATE2);
	ZeroMemory(&g_mouseStateTrigger, sizeof DIMOUSESTATE2);

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse(void)
{
	// 入力処理の終了処理
	UninitInput();

	if(g_pDevMouse)
	{
		// マウスへのアクセス権を開放
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//=============================================================================
// マウスの更新処理
//=============================================================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouseState;

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{
		g_mouseStateTrigger.lX = ((g_mouseState.lX ^ mouseState.lX) & mouseState.lX);
		g_mouseStateTrigger.lY = ((g_mouseState.lY ^ mouseState.lY) & mouseState.lY);
		g_mouseStateTrigger.lZ = ((g_mouseState.lZ ^ mouseState.lZ) & mouseState.lZ);

		for(int nCntKey = 0; nCntKey < NUM_MOUSE_BUTTON_MAX; nCntKey++)
		{
			g_mouseStateTrigger.rgbButtons[nCntKey] = ((g_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & mouseState.rgbButtons[nCntKey]);
		}

		g_mouseState = mouseState;
	}
	else
	{
		// マウスへのアクセス権を獲得(入力制御開始)
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
bool GetMouseRightPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_CENTER] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_CENTER] & 0x80) ? true: false;
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
long GetMouseAxisX(void)
{
	return g_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
long GetMouseAxisY(void)
{
	return g_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
long GetMouseAxisZ(void)
{
	return g_mouseState.lZ;
}

//=============================================================================
// ジョイパッドの初期化
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	int nLoop;

	HRESULT hr;

	// 入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 初期化
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		g_pDIDevJoypad[nLoop] = NULL;

	// デバイスオブジェクトを作成(接続されているジョイパッドを列挙する)
	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return E_FAIL;

	// ジョイパッドの数だけ処理
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// ジョイパッドがない場合はすっ飛ばす
		if(g_pDIDevJoypad[nLoop] == NULL)
			continue;

		// データフォーマットの設定
		if(FAILED(g_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
			return E_FAIL;

		// 協調レベルの設定
		if(FAILED(g_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
			return E_FAIL;

		// デバイスへの入力制御開始
		g_pDIDevJoypad[nLoop]->Acquire();	
	}

	return S_OK;
}

//=============================================================================
// ジョイパッド問い合わせ用コールバック関数
//=============================================================================
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS	diDevCaps;			// デバイス情報

	// ジョイパッド用デバイスオブジェクトを作成
	if(FAILED(g_pInput->CreateDevice(lpddi->guidInstance, &g_pDIDevJoypad[g_nJoypadNum], NULL)))
		return DIENUM_CONTINUE;		// 列挙を続ける

	// ジョイパッドの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(g_pDIDevJoypad[g_nJoypadNum]->GetCapabilities(&diDevCaps)))
	{
		if(g_pDIDevJoypad[g_nJoypadNum])
			g_pDIDevJoypad[g_nJoypadNum]->Release();
		g_pDIDevJoypad[g_nJoypadNum] = NULL;
		return DIENUM_CONTINUE;		// 列挙を続ける
	}

	// 規定数に達したら終了
	g_nJoypadNum++;
	if(g_nJoypadNum == MAX_CONTROLER)
		return DIENUM_STOP;			// 列挙を終了する
	else
		return DIENUM_CONTINUE;		// 列挙を続ける
}

//=============================================================================
// ジョイパッドの終了処理
//=============================================================================
void UninitJoypad(void)
{
	for(int nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		SAFE_RELEASE(g_pDIDevJoypad[nLoop]);
}

//=============================================================================
// ジョイパッドの更新処理
//=============================================================================
void UpdateJoypad(void)
{
	int nLoop;
	
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// 直前ジョイパッド情報のバックアップ
		g_joyStatePrev[nLoop] = g_joyState[nLoop];

		if(g_pDIDevJoypad[nLoop])
		{
			// デバイスからデータを取得
			if(FAILED(g_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(DIJOYSTATE), &g_joyState[nLoop])))
				g_pDIDevJoypad[nLoop]->Acquire();
		}
		

		// トリガー情報計算
		for( int i = 0; i < 32; i++ )
		{
			if( g_joyState[nLoop].rgbButtons[i] & 0x80 && 
				!(g_joyStatePrev[nLoop].rgbButtons[i] & 0x80) )
			{
				g_joyStateTrigger[nLoop][i] = 0x80;
			} else {
				g_joyStateTrigger[nLoop][i] = 0x00;
			}
		}

		if(g_joyState[nLoop].lY - 32768 < -16384) g_joyStateTrigger[nLoop][LSTICK_UP] = 0x80;
		if(g_joyState[nLoop].lY - 32768 > 16384) g_joyStateTrigger[nLoop][LSTICK_DOWN] = 0x80;
		if(g_joyState[nLoop].lX - 32768 < -16384) g_joyStateTrigger[nLoop][LSTICK_LEFT] = 0x80;
		if(g_joyState[nLoop].lX - 32768 > 16384) g_joyStateTrigger[nLoop][LSTICK_RIGHT] = 0x80;

	}
}

bool GetGamePadTrigger( int padNo, DWORD button )
{
	if(g_pDIDevJoypad[padNo] == NULL) return false;
	return (g_joyStateTrigger[padNo][button] & 0x80) ? true: false;
}

bool GetGamePadPress( int padNo, DWORD button )
{
	if(g_pDIDevJoypad[padNo] == NULL) return false;
	return (g_joyState[padNo].rgbButtons[button] & 0x80) ? true: false;
}

long GetGamePadRightAxisX(int padNo)
{
	if(g_pDIDevJoypad[padNo] == NULL) return 0;

	if( abs(g_joyState[padNo].lRx - 32768) < 1500) return 0;

	return (g_joyState[padNo].lRx - 32768) * 0.01f;
}
long GetGamePadRightAxisY(int padNo)
{
	if(g_pDIDevJoypad[padNo] == NULL) return 0;

	if( abs(g_joyState[padNo].lRy - 32768) < 1500) return 0;

	return (g_joyState[padNo].lRy - 32768) * 0.01f;
}