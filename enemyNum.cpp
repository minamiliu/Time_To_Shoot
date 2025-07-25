﻿/*******************************************************************************
*
* タイトル:		テクスチャ敵の残機表示処理
* プログラム名:	enemyNum.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "enemyNum.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"	// 読み込むテクスチャファイル名
#define	POLYGON_SIZE_X				(80)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(100)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(10)							// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define NUM_OBJECT					(2)								// オブジェクトの数

#define	POLYGON_POS_X				(POLYGON_SIZE_X)				//ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(SCREEN_HEIGHT - POLYGON_SIZE_Y)	// ポリゴンの基準位置(Ｙ座標)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice);
void SetTextureEnemyNum( int nIdx, int number);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemyNum = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEnemyNum = NULL;

ENEMY_NUM			g_EnemyNum; //Timeのワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//EnemyNumの初期化
	g_EnemyNum.pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_EnemyNum.value = 0;

	//頂点情報の作成
	if(FAILED(MakeVertexEnemyNum(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_POLYGON,	// ファイルの名前
								&g_pTextureEnemyNum);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyNum(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferEnemyNum, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEnemyNum);

	//ポリゴンの描画
	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			(nCntPlace*NUM_VERTEX),						//ロードする最初の頂点インデックス
			NUM_POLYGON				//ポリゴンの数
		);
	}


}


//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyNum(void)
{
	if(g_pTextureEnemyNum != NULL)
	{
		g_pTextureEnemyNum->Release();
		g_pTextureEnemyNum = NULL;
	}
	if(g_pVtxBufferEnemyNum != NULL)
	{
		g_pVtxBufferEnemyNum->Release();
		g_pVtxBufferEnemyNum = NULL;
	}
}

/*******************************************************************************
関数名:	MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferEnemyNum,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferEnemyNum->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
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
		pVtx[0].vtx = D3DXVECTOR3(g_EnemyNum.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_EnemyNum.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_EnemyNum.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_EnemyNum.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferEnemyNum->Unlock();

	return S_OK;
}



//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyNum(void)
{
	
}



/*******************************************************************************
関数名:	void SetTextureEnemyNum( int nIdx, int number) 
引数:	 int nTdx：何桁目か, int number：表示する数
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureEnemyNum( int nIdx, int number) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferEnemyNum->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;

		//テクスチャ座標の設定
		fPosXLeft = TEX_PATTERN_SIZE_X * number;
		fPosXRight = TEX_PATTERN_SIZE_X * number + TEX_PATTERN_SIZE_X;
		fPosYUp = 0;
		fPosYDown = TEX_PATTERN_SIZE_Y;

		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// 頂点データをアンロックする
		g_pVtxBufferEnemyNum->Unlock();
	}
}

/*******************************************************************************
関数名:	void ChangeEnemyNum(int value)
引数:	int value：増加する点数
戻り値:	なし
説明:	総点数を変更する
*******************************************************************************/
void ChangeEnemyNum(int value)
{
	g_EnemyNum.value += value;
	if( g_EnemyNum.value < 0) g_EnemyNum.value = 0; 

	SetEnemyNum( g_EnemyNum.value);
}

/*******************************************************************************
* 敵の設置
*******************************************************************************/
void SetEnemyNum(int value)
{
	g_EnemyNum.value = value;
	if( value < 0) value = 0;

	int number;
	for(int keta = 1; keta <= NUM_OBJECT; keta++)
	{
		number = value % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureEnemyNum(keta,number);
	}
}

/*******************************************************************************
* 敵のデータを取得
*******************************************************************************/
int GetEnemyNum(void)
{
	return g_EnemyNum.value;
}
