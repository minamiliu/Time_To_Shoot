/*******************************************************************************
*
* �^�C�g��:		���C������
* �v���O������:	main.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "resource.h"
#include "sound.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"
#include "message.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"TIME TO SHOOT"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void ResetWindow(void);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
MODE g_mode;
bool g_bDebug;
int g_nWindowMode = -1;

#ifdef _DEBUG
int					g_nCountFPS;			// FPS�J�E���^
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	HWND hWnd;
	MSG msg;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2)),// �g�p����A�C�R�����w��
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�f�X�N�g�b�v�̃T�C�Y���擾
	RECT rcWnd;
	GetClientRect( GetDesktopWindow(), &rcWnd);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 
						CW_USEDEFAULT,  
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	if( g_nWindowMode == 0)
	{
		// �E�B���h�E�̍쐬
		hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_VISIBLE | WS_POPUP,
							0, 
							0,  
							rcWnd.right + GetSystemMetrics(SM_CXDLGFRAME) * 2,
							rcWnd.bottom + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
							NULL,
							NULL,
							hInstance,
							NULL);	
	}
	if( g_nWindowMode == 1)
	{
		// �E�B���h�E�̍쐬
		hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 
							CW_USEDEFAULT,  
							SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
							SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
							NULL,
							NULL,
							hInstance,
							NULL);
	}
	
	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE))) //FALSE �t���X�N���[��
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;

				//�}�E�X�͈͂̐���
				POINT curPos;
				GetCursorPos(&curPos);
				WINDOWINFO windowInfo;
				GetWindowInfo( hWnd, &windowInfo);
				if(curPos.x > windowInfo.rcClient.right || curPos.x < windowInfo.rcClient.left || curPos.y > windowInfo.rcClient.bottom || curPos.y < windowInfo.rcClient.top)
				{
					SetCursorPos( (windowInfo.rcClient.right + windowInfo.rcClient.left)/2, (windowInfo.rcClient.top + windowInfo.rcClient.bottom)/2);
				}
			
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�
	
	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int id;

	switch(uMsg)
	{
		case WM_CREATE:
			if( g_nWindowMode == -1)
			{
				id = MessageBox(hWnd,"Do you want to use FULL SCREEN mode?","Set Game Mode",MB_YESNO);
				if(id == IDYES){
					g_nWindowMode = 0;
				}
				else
				{
					g_nWindowMode = 1;
				}			
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				DestroyWindow(hWnd);
				break;
			}
			break;
		case WM_SETCURSOR:
			ShowCursor(false);
			break;
		default:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// ���ʂ��J�����O
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// ���ʂ�����
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//debug
	InitDebugProc();

	//���͂̏�����
	InitKeyboard(hInstance, hWnd);
	InitMouse(hInstance, hWnd);
	InitJoypad(hInstance, hWnd);

	//sound�̏�����
	InitSound(hWnd);

	InitFade();
	InitMessage();

#ifdef _DEBUG
	//InitTitle();
	//InitGame();
	//InitResult();
	//UninitGame();
	//UninitResult();
#endif

	SetMode( MODE_TITLE);
	g_bDebug = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	//���͂̊J��
	UninitKeyboard();
	UninitMouse();
	UninitJoypad();

	//debug
	UninitDebugProc();

	//sound�̏I������
	UninitSound();

	UninitFade();
	UninitMessage();

	UninitTitle();
	UninitGame();
	UninitResult();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	//debug
	UpdateDebugProc();

	//���͂̍X�V����
	UpdateKeyboard();
	UpdateMouse();
	UpdateJoypad();

	UpdateFade();
	UpdateMessage();


	switch(g_mode)
	{
		case MODE_TITLE:
			UpdateTitle();
			break;
		case MODE_GAME:
			UpdateGame();
			break;
		case MODE_RESULT:
			UpdateResult();
			break;
	}	

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch(g_mode)
		{
			case MODE_TITLE:
				DrawTitle();
				break;
			case MODE_GAME:
				DrawGame();
				break;
			case MODE_RESULT:
				DrawResult();
				break;
		}
		DrawMessage();
		DrawFade();

		DrawDebugProc();
#ifdef _DEBUG
		// FPS�\��
		PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

/*******************************************************************************
�֐���:	void SetMode(MODE mode)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���J�ڂ̐ݒ�֐�
*******************************************************************************/
void SetMode(MODE mode)
{
	g_mode = mode;

	switch(g_mode)
	{
		case MODE_TITLE:
			StopSound();
			PlaySound(SOUND_LABEL_BGM_OP);
			UninitResult();
			InitTitle();
			break;
		case MODE_GAME:
			StopSound();
			//PlaySound(SOUND_LABEL_BGM_PLAY);
			UninitTitle();
			InitGame();
			break;
		case MODE_RESULT:
			StopSound();
			PlaySound(SOUND_LABEL_BGM_RANK);
			UninitGame();
			InitResult();
			break;
	}
}
//=============================================================================
// �f�o�b�O
//=============================================================================
bool IsDebugMode(void)
{
	return g_bDebug;
}
//=============================================================================
// �f�o�b�O���[�h��ς���
//=============================================================================
void SwitchDebug()
{
	g_bDebug = !g_bDebug;
}
