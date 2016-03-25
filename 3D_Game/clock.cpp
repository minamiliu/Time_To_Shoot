/*******************************************************************************
*
* タイトル:		テクスチャの時計表示処理
* プログラム名:	clock.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
********************************************************************************

*******************************************************************************/
/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "clock.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_CLOCK				"data/TEXTURE/UI/Clock.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_HAND				"data/TEXTURE/UI/ClockHand.png"		// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(SCREEN_WIDTH/2 + 100)				// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(70)				// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(100)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(100)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
//#define	TIME_CHANGE_PATTERN			(5)								// アニメーションの切り替わるタイミング(フレーム数)
#define MAX_OBJECT					(2)							// オブジェクトの数

#define	VALUE_ROTATE			(D3DX_PI / 30.0f)								// カメラの回転量

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice);
void SetVertexClockRot(int nCntClock, float angle);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferClock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureClock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;

CLOCK g_clock[MAX_OBJECT]; //プレイヤーのワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitClock(D3DXVECTOR3 initPos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++)
	{
		//bulletの初期化
		g_clock[nCntClock].pos = initPos;
		g_clock[nCntClock].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_clock[nCntClock].bUse = true;
	}



	//頂点情報の作成
	if(FAILED(MakeVertexClock(pDevice)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_CLOCK,		// ファイルの名前
								&g_pTextureClock);	// 読み込むメモリー
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_HAND,		// ファイルの名前
								&g_pTextureHand);	// 読み込むメモリー

	return S_OK;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawClock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferClock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++)
	{
		if(g_clock[nCntClock].bUse)
		{
			switch( nCntClock)
			{
			case 0:
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureClock);
				break;
			case 1:
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureHand);
				break;

			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntClock*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);
		}	
	}


}


//=============================================================================
// 終了処理
//=============================================================================
void UninitClock(void)
{
	if(g_pTextureClock != NULL)
	{
		g_pTextureClock->Release();
		g_pTextureClock = NULL;
	}
	if(g_pTextureHand != NULL)
	{
		g_pTextureHand->Release();
		g_pTextureHand = NULL;
	}
	if(g_pVtxBufferClock != NULL)
	{
		g_pVtxBufferClock->Release();
		g_pVtxBufferClock = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferClock,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}



	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferClock->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++, pVtx += NUM_VERTEX)
	{

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//color
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x - (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x + (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x - (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x + (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	

	}



	g_pVtxBufferClock->Unlock();

	return S_OK;
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateClock(float fTimeSpeed)
{
	SetVertexClockRot( 1, VALUE_ROTATE * fTimeSpeed);
}

/*******************************************************************************
関数名:	void SetVertexClockRot(int nCntClock, float angle)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexClockRot(int nCntClock, float angle)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferClock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntClock * NUM_VERTEX; 
		
		// 頂点座標の設定
		float xc = g_clock[nCntClock].pos.x;
		float yc = g_clock[nCntClock].pos.y;
		float nx,ny;

		nx = (pVtx[0].vtx.x - xc) * cosf(angle) - (pVtx[0].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[0].vtx.x - xc) * sinf(angle) + (pVtx[0].vtx.y - yc) * cosf(angle) + yc;
		pVtx[0].vtx.x = nx;
		pVtx[0].vtx.y = ny;

		nx = (pVtx[1].vtx.x - xc) * cosf(angle) - (pVtx[1].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[1].vtx.x - xc) * sinf(angle) + (pVtx[1].vtx.y - yc) * cosf(angle) + yc;
		pVtx[1].vtx.x = nx;
		pVtx[1].vtx.y = ny;

		nx = (pVtx[2].vtx.x - xc) * cosf(angle) - (pVtx[2].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[2].vtx.x - xc) * sinf(angle) + (pVtx[2].vtx.y - yc) * cosf(angle) + yc;
		pVtx[2].vtx.x = nx;
		pVtx[2].vtx.y = ny;

		nx = (pVtx[3].vtx.x - xc) * cosf(angle) - (pVtx[3].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[3].vtx.x - xc) * sinf(angle) + (pVtx[3].vtx.y - yc) * cosf(angle) + yc;
		pVtx[3].vtx.x = nx;
		pVtx[3].vtx.y = ny;

		// 頂点データをアンロックする
		g_pVtxBufferClock->Unlock();
	}
}


