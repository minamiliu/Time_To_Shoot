/*******************************************************************************
*
* タイトル:		テクスチャ 時間の数字表示処理
* プログラム名:	time.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "time.h"
#include "fade.h"
#include "message.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	POLYGON_SIZE_X				(35)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(70)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(10)							// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define NUM_OBJECT					(3)								// オブジェクトの数
#define STAGE_TIME					(999)

#define	POLYGON_POS_X				(SCREEN_WIDTH/2 - NUM_OBJECT/2*POLYGON_SIZE_X)	//ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(70)											// ポリゴンの基準位置(Ｙ座標)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);
void SetTextureTime( int nIdx, int number);
HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;

TIME			g_time; //Timeのワーク



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Timeの初期化
	g_time.pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_time.time = STAGE_TIME;
	g_time.fCounterSeconds = 0.0f;

	//頂点情報の作成
	if(FAILED(MakeVertexTime(pDevice)))
	{
		return E_FAIL;
	}

	SetTime(STAGE_TIME);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_POLYGON,	// ファイルの名前
								&g_pTextureTime);	// 読み込むメモリー

	return S_OK;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

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
void UninitTime(void)
{
	if(g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	if(g_pVtxBufferTime != NULL)
	{
		g_pVtxBufferTime->Release();
		g_pVtxBufferTime = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTime,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

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
		pVtx[0].vtx = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}



//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(float fTimeSpeed)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_time.fCounterSeconds += 1 * fTimeSpeed;

	if(g_time.fCounterSeconds >= 60)
	{
		if(g_time.time <= 10)
		{
			//頂点カラーの変更
			ChangeVertexColor(pDevice,255,0,0);
		}
		
		//time's up
		if(g_time.time <= 0)
		{
			FadeOutToNext(MODE_RESULT);
			SetMessage(MSG_TIME_UP);
		}
		else
		{
			g_time.time--;
			SetTime(g_time.time);			
		}
	
		g_time.fCounterSeconds = 0.0f;
	}
}



/*******************************************************************************
関数名:	void SetTextureTime( int nIdx, int number)  
引数:	 int nTdx：何桁目か, int number：表示する数
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureTime( int nIdx, int number) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferTime->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pVtxBufferTime->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTime(int value)
引数:	int value：増加する点数
戻り値:	なし
説明:	総点数を変更する
*******************************************************************************/
void SetTime(int value)
{

	int number;

	for(int keta = 1; keta <= NUM_OBJECT; keta++)
	{
		number = value % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureTime(keta,number);
	}
}

//=============================================================================
// 色を変える
//=============================================================================
HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B)
{

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
	{
		//color
		pVtx[0].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[1].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[2].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[3].col = D3DCOLOR_RGBA(R,G,B,255);
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}
//=============================================================================
// 時間データを取得
//=============================================================================
int GetTime(void)
{
	return g_time.time;
}
