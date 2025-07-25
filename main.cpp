/*******************************************************************************
*
* タイトル:		メイン処理
* プログラム名:	main.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"TIME TO SHOOT"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void ResetWindow(void);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// デバイスオブジェクト(描画に必要)
MODE g_mode;
bool g_bDebug;
int g_nWindowMode = -1;

#ifdef _DEBUG
int					g_nCountFPS;			// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2)),// 使用するアイコンを指定
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//デスクトップのサイズを取得
	RECT rcWnd;
	GetClientRect( GetDesktopWindow(), &rcWnd);

	// ウィンドウの作成
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
		// ウィンドウの作成
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
		// ウィンドウの作成
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
	
	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE))) //FALSE フルスクリーン
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
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

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;

				//マウス範囲の制約
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
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す
	
	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// 裏面をカリング
	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// 両面を書く
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//debug
	InitDebugProc();

	//入力の初期化
	InitKeyboard(hInstance, hWnd);
	InitMouse(hInstance, hWnd);
	InitJoypad(hInstance, hWnd);

	//soundの初期化
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
// 終了処理
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスオブジェクトの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	//入力の開放
	UninitKeyboard();
	UninitMouse();
	UninitJoypad();

	//debug
	UninitDebugProc();

	//soundの終了処理
	UninitSound();

	UninitFade();
	UninitMessage();

	UninitTitle();
	UninitGame();
	UninitResult();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	//debug
	UpdateDebugProc();

	//入力の更新処理
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
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画の開始
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
		// FPS表示
		PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

/*******************************************************************************
関数名:	void SetMode(MODE mode)
引数:	なし
戻り値:	なし
説明:	ゲーム遷移の設定関数
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
// デバッグ
//=============================================================================
bool IsDebugMode(void)
{
	return g_bDebug;
}
//=============================================================================
// デバッグモードを変える
//=============================================================================
void SwitchDebug()
{
	g_bDebug = !g_bDebug;
}
