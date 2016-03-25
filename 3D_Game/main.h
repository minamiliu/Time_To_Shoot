/*******************************************************************************
*
* タイトル:		メイン処理
* プログラム名:	main.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#define USE_SOUND		// サウンド使用の有無
//#undef USE_SOUND


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include "dinput.h"

#ifdef USE_SOUND
#include "xaudio2.h"		// XAudio2関連のヘッダー
#endif


//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define DIRECTINPUT_VERTION (0x0800)
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

#define SAFE_RELEASE(ptr)			{ if(ptr) { (ptr)->Release(); (ptr) = NULL; } }

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;	// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	D3DCOLOR col;		// 反射光
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

//３Ｄポリゴン
typedef struct
{
	D3DXVECTOR3 vtx; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR	col; //頂点カラー
	D3DXVECTOR2	tex; //テクスチャ座標
}VERTEX_3D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT GetFont(void);
void SetMode(MODE mode);

//debug mode
bool IsDebugMode(void);
void SwitchDebug(void);
#endif