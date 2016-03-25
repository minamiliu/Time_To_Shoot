/*******************************************************************************
*
* タイトル:		テクスチャ プレイヤーの弾の数表示処理
* プログラム名:	playerBulletNum.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "playerBulletNum.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/UI/PlayerBullet.png"// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(1100)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(25)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(100)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(2)							// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define NUM_DIGIT					(3)


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexNumBullet(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexScore(void);
void SetTextureNumBullet( int nPlayerIdx, int nIdx, int number);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferNumWin = NULL;
LPDIRECT3DTEXTURE9 g_pTextureNumWin = NULL;

NUM_BULLET	g_numBullet[MAX_PLAYER];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitNumBullet()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Player 1 life
	g_numBullet[0].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_numBullet[0].col = D3DCOLOR_RGBA(255,255,0,255); //yellow 


	//人数分のみ、初期化
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_numBullet[nCntPlayer].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_numBullet[nCntPlayer].num = 0;
		g_numBullet[nCntPlayer].bDraw = true;
		g_numBullet[nCntPlayer].nCntFlash = -1;
	}


	//頂点情報の作成
	if(FAILED(MakeVertexNumBullet(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_POLYGON,	// ファイルの名前
								&g_pTextureNumWin);	// 読み込むメモリー

	return S_OK;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawNumBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferNumWin, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureNumWin);

	//ポリゴンの描画
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_numBullet[nCntPlayer].bDraw == false) continue;
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				(nCntPlayer*NUM_DIGIT + nCntDigit) * NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);			
		}
	}
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitNumBullet(void)
{
	if(g_pTextureNumWin != NULL)
	{
		g_pTextureNumWin->Release();
		g_pTextureNumWin = NULL;
	}
	if(g_pVtxBufferNumWin!= NULL)
	{
		g_pVtxBufferNumWin->Release();
		g_pVtxBufferNumWin = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexNumBullet(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexNumBullet(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_DIGIT*MAX_PLAYER,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferNumWin,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferNumWin->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
		{

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//color
			pVtx[0].col = g_numBullet[nCntPlayer].col;
			pVtx[1].col = g_numBullet[nCntPlayer].col;
			pVtx[2].col = g_numBullet[nCntPlayer].col;
			pVtx[3].col = g_numBullet[nCntPlayer].col;

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(g_numBullet[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numBullet[nCntPlayer].pos.y , 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_numBullet[nCntPlayer].pos.x  + g_numBullet[nCntPlayer].size.x +  nCntDigit*POLYGON_SIZE_X, g_numBullet[nCntPlayer].pos.y , 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_numBullet[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numBullet[nCntPlayer].pos.y + g_numBullet[nCntPlayer].size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_numBullet[nCntPlayer].pos.x  + g_numBullet[nCntPlayer].size.x + nCntDigit*POLYGON_SIZE_X, g_numBullet[nCntPlayer].pos.y + g_numBullet[nCntPlayer].size.y, 0.0f);

			//テクスチャ座標指定
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferNumWin->Unlock();

	return S_OK;
}



//=============================================================================
// 更新処理
//=============================================================================
void UpdateNumBullet(void)
{
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if( g_numBullet[nCntPlayer].num <= 0)
		{
			if( g_numBullet[nCntPlayer].nCntFlash == -1)
			{
				g_numBullet[nCntPlayer].nCntFlash = 60;
			}
			else if( g_numBullet[nCntPlayer].nCntFlash > 0)
			{
				g_numBullet[nCntPlayer].nCntFlash--;
			}
			else if( g_numBullet[nCntPlayer].nCntFlash == 0)
			{
				g_numBullet[nCntPlayer].bDraw = !g_numBullet[nCntPlayer].bDraw;
				g_numBullet[nCntPlayer].nCntFlash = -1;
			}
			
		}
		else
		{
			g_numBullet[nCntPlayer].bDraw = true;
		}
	}
}



/*******************************************************************************
関数名:	SetTextureNumBullet( int nPlayerIdx, int nIdx, int number) 
引数:	 int nTdx：何桁目か, int number：表示する数
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureNumBullet( int nPlayerIdx, int nIdx, int number) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferNumWin->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nPlayerIdx * NUM_DIGIT * NUM_VERTEX);

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
		g_pVtxBufferNumWin->Unlock();
	}
}

/*******************************************************************************
関数名:	ChangeNumBullet(int nPlayerIdx, int value)
引数:	int value：増加する点数
戻り値:	なし
説明:	総点数を変更する
*******************************************************************************/
void ChangeNumBullet(int nPlayerIdx, int value)
{
	g_numBullet[nPlayerIdx].num = value;

	if(g_numBullet[nPlayerIdx].num >= NUM_DIGIT ) g_numBullet[nPlayerIdx].num = NUM_DIGIT; //最大値
	if(g_numBullet[nPlayerIdx].num < 0 ) g_numBullet[nPlayerIdx].num = 0; //最小値

	int number = g_numBullet[nPlayerIdx].num;
	for(int keta = NUM_DIGIT; keta >= 1; keta--)
	{
		if(number > 0)
		{
			SetTextureNumBullet(nPlayerIdx , keta , 1);
			number--;
		}		
		else
		{
			SetTextureNumBullet(nPlayerIdx , keta , 0);
		}
	}
}
