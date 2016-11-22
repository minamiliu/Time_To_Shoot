/*******************************************************************************
*
* �^�C�g��:		���͏���
* �v���O������:	input.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX				(256)	// �L�[�̍ő吔
#define	LIMIT_COUNT_REPEAT		(20)	// ���s�[�g�J�E���g���~�b�^�[
#define	NUM_MOUSE_BUTTON_MAX	(8)		// �}�E�X�̃{�^���ő吔
#define	MOUSE_BUTTON_LEFT		(0)		// �}�E�X�̍��{�^��
#define	MOUSE_BUTTON_RIGHT		(1)		// �}�E�X�̉E�{�^��
#define	MOUSE_BUTTON_CENTER		(2)		// �}�E�X�̒����{�^��

#define MAX_CONTROLER			(2)		// �R���g���[���ő吔(�g�����ɉ����ĕύX���邱��)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevMouse = NULL;					// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��񃏁[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
DIMOUSESTATE2			g_mouseState;						// �}�E�X�̓��͏�񃏁[�N
DIMOUSESTATE2			g_mouseStateTrigger;				// �}�E�X�̃g���K�[��񃏁[�N

DIJOYSTATE				g_joyState[MAX_CONTROLER];			// �W���C�p�b�h���z��
DIJOYSTATE				g_joyStatePrev[MAX_CONTROLER];		// �W���C�p�b�h���z��i���O�j
LPDIRECTINPUTDEVICE8	g_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�W���C�p�b�h)
int						g_nJoypadNum = 0;					// ���������W���C�p�b�h�̐�
BYTE					g_joyStateTrigger[MAX_CONTROLER][32];

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_OK;

	if(!g_pInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pInput, NULL);
	}

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if(g_pInput)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	// �e���[�N�̃N���A
	ZeroMemory(g_aKeyState, sizeof g_aKeyState);
	ZeroMemory(g_aKeyStateTrigger, sizeof g_aKeyStateTrigger);
	ZeroMemory(g_aKeyStateRelease, sizeof g_aKeyStateRelease);
	ZeroMemory(g_aKeyStateRepeat, sizeof g_aKeyStateRepeat);
	ZeroMemory(g_aKeyStateRepeatCnt, sizeof g_aKeyStateRepeatCnt);

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	// ���͏����̏I������
	UninitInput();

	if(g_pDevKeyboard)
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͏����I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�̏�����
//=============================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̃v���p�e�B��ݒ�
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
			MessageBox(hWnd, "�}�E�X�̃f�o�C�X�̃v���p�e�B��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return hr;
		}
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevMouse->Acquire();

	//������
	ZeroMemory(&g_mouseState, sizeof DIMOUSESTATE2);
	ZeroMemory(&g_mouseStateTrigger, sizeof DIMOUSESTATE2);

	return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void UninitMouse(void)
{
	// ���͏����̏I������
	UninitInput();

	if(g_pDevMouse)
	{
		// �}�E�X�ւ̃A�N�Z�X�����J��
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouseState;

	// �f�o�C�X����f�[�^���擾
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
		// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_pDevMouse->Acquire();
	}
}

//=============================================================================
// �}�E�X�f�[�^�擾(���v���X)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(���g���K�[)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[MOUSE_BUTTON_LEFT] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�v���X)
//=============================================================================
bool GetMouseRightPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�g���K�[)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[MOUSE_BUTTON_RIGHT] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����v���X)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_CENTER] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����g���K�[)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return (g_mouseState.rgbButtons[MOUSE_BUTTON_CENTER] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�w���ړ�)
//=============================================================================
long GetMouseAxisX(void)
{
	return g_mouseState.lX;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�x���ړ�)
//=============================================================================
long GetMouseAxisY(void)
{
	return g_mouseState.lY;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�y���ړ�)
//=============================================================================
long GetMouseAxisZ(void)
{
	return g_mouseState.lZ;
}

//=============================================================================
// �W���C�p�b�h�̏�����
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	int nLoop;

	HRESULT hr;

	// ���͏����̏�����
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// ������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		g_pDIDevJoypad[nLoop] = NULL;

	// �f�o�C�X�I�u�W�F�N�g���쐬(�ڑ�����Ă���W���C�p�b�h��񋓂���)
	if(FAILED(g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return E_FAIL;

	// �W���C�p�b�h�̐���������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// �W���C�p�b�h���Ȃ��ꍇ�͂�����΂�
		if(g_pDIDevJoypad[nLoop] == NULL)
			continue;

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		if(FAILED(g_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
			return E_FAIL;

		// �������x���̐ݒ�
		if(FAILED(g_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
			return E_FAIL;

		// �f�o�C�X�ւ̓��͐���J�n
		g_pDIDevJoypad[nLoop]->Acquire();	
	}

	return S_OK;
}

//=============================================================================
// �W���C�p�b�h�₢���킹�p�R�[���o�b�N�֐�
//=============================================================================
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS	diDevCaps;			// �f�o�C�X���

	// �W���C�p�b�h�p�f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(g_pInput->CreateDevice(lpddi->guidInstance, &g_pDIDevJoypad[g_nJoypadNum], NULL)))
		return DIENUM_CONTINUE;		// �񋓂𑱂���

	// �W���C�p�b�h�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(g_pDIDevJoypad[g_nJoypadNum]->GetCapabilities(&diDevCaps)))
	{
		if(g_pDIDevJoypad[g_nJoypadNum])
			g_pDIDevJoypad[g_nJoypadNum]->Release();
		g_pDIDevJoypad[g_nJoypadNum] = NULL;
		return DIENUM_CONTINUE;		// �񋓂𑱂���
	}

	// �K�萔�ɒB������I��
	g_nJoypadNum++;
	if(g_nJoypadNum == MAX_CONTROLER)
		return DIENUM_STOP;			// �񋓂��I������
	else
		return DIENUM_CONTINUE;		// �񋓂𑱂���
}

//=============================================================================
// �W���C�p�b�h�̏I������
//=============================================================================
void UninitJoypad(void)
{
	for(int nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		SAFE_RELEASE(g_pDIDevJoypad[nLoop]);
}

//=============================================================================
// �W���C�p�b�h�̍X�V����
//=============================================================================
void UpdateJoypad(void)
{
	int nLoop;
	
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// ���O�W���C�p�b�h���̃o�b�N�A�b�v
		g_joyStatePrev[nLoop] = g_joyState[nLoop];

		if(g_pDIDevJoypad[nLoop])
		{
			// �f�o�C�X����f�[�^���擾
			if(FAILED(g_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(DIJOYSTATE), &g_joyState[nLoop])))
				g_pDIDevJoypad[nLoop]->Acquire();
		}
		

		// �g���K�[���v�Z
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