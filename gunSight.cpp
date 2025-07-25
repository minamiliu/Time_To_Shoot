/*******************************************************************************
*
* タイトル:		テクスチャ銃の照準器処理
* プログラム名:	gunSight.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "gunSight.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/sight2.png"		// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(SCREEN_WIDTH/2)				// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(SCREEN_HEIGHT/2)				// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(50)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
//#define	TIME_CHANGE_PATTERN			(5)								// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define MAX_GUNSIGHT				(1)							// オブジェクトの数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice);
void SetVertexGunSight(int nCntExplo);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGunSight = NULL;
LPDIRECT3DTEXTURE9 g_pTextureGunSight = NULL;

GUNSIGHT g_gunSight[MAX_GUNSIGHT]; //プレイヤーのワーク


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGunSight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++)
	{
		//bulletの初期化
		g_gunSight[nCntExplo].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_gunSight[nCntExplo].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		//g_gunSight[nCntExplo].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_gunSight[nCntExplo].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_gunSight[nCntExplo].bUse = true;
	}



	//頂点情報の作成
	if(FAILED(MakeVertexGunSight(pDevice)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureGunSight);	// 読み込むメモリー

	return S_OK;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawGunSight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferGunSight, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGunSight);

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++)
	{
		if(g_gunSight[nCntExplo].bUse)
		{

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntExplo*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);
		}	
	}



}


//=============================================================================
// 終了処理
//=============================================================================
void UninitGunSight(void)
{
	if(g_pTextureGunSight != NULL)
	{
		g_pTextureGunSight->Release();
		g_pTextureGunSight = NULL;
	}
	if(g_pVtxBufferGunSight != NULL)
	{
		g_pVtxBufferGunSight->Release();
		g_pVtxBufferGunSight = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_GUNSIGHT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferGunSight,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}



	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferGunSight->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++, pVtx += NUM_VERTEX)
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
		pVtx[0].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	}



	g_pVtxBufferGunSight->Unlock();

	return S_OK;
}



//=============================================================================
// 更新処理
//=============================================================================
void UpdateGunSight(void)
{

}

/*******************************************************************************
関数名:	void SetVertexGunSight(int nCntExplo)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexGunSight(int nCntExplo)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferGunSight->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntExplo * NUM_VERTEX; 
		
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);	

		

		// 頂点データをアンロックする
		g_pVtxBufferGunSight->Unlock();
	}
}


