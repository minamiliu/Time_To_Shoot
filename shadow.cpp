/*******************************************************************************
*
* タイトル:		丸影処理
* プログラム名:	shadow.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(50.0f)							// 弾の幅
#define	SHADOW_SIZE_Z		(50.0f)							// 弾の高さ

#define	MAX_SHADOW			(256)							// 影最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3	rot;			// 回転
	bool bUse;					// 使用しているかどうか
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ

SHADOW g_aShadow[MAX_SHADOW];							// 影ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 影ワークの初期化
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_SHADOW, &g_pTextureShadow);

	// 頂点情報の作成
	MakeVertexShadow(pDevice);
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	if(g_pTextureShadow != NULL)
	{// テクスチャの開放
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	if(g_pVtxBuffShadow != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	SHADOW *pShadow;

	// 減算合成
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 影構造体のポインタに影ワークの先頭アドレスを代入
	pShadow = &g_aShadow[0];
	
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if(pShadow->bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply( &pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation( &mtxRot, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply( &pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// ワールドマトリックスの設定
			pDevice->SetTransform( D3DTS_WORLD, &pShadow->mtxWorld);

			// 頂点バッファをレンダリングパイプラインに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntShadow * 4,			//ロードする最初の頂点インデックス
				2						//ポリゴンの数
			);	
		}
	}

	// 通常ブレンド 
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
																				
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f - (SHADOW_SIZE_X/2), 0.0f, 0.0f + (SHADOW_SIZE_Z/2));
			pVtx[1].vtx = D3DXVECTOR3(0.0f + (SHADOW_SIZE_X/2), 0.0f, 0.0f + (SHADOW_SIZE_Z/2));
			pVtx[2].vtx = D3DXVECTOR3(0.0f - (SHADOW_SIZE_X/2), 0.0f, 0.0f - (SHADOW_SIZE_Z/2));
			pVtx[3].vtx = D3DXVECTOR3(0.0f + (SHADOW_SIZE_X/2), 0.0f, 0.0f - (SHADOW_SIZE_Z/2));

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
			pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
			pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
			pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
		}

		// 頂点データをアンロックする
		g_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtxをインデックス分ズラす
		pVtx += nIdxShadow * 4;

		// 頂点座標の設定（引数を用いて設定）
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f, 0.0f + (fSizeZ/2));
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f, 0.0f + (fSizeZ/2));
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f, 0.0f - (fSizeZ/2));
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f, 0.0f - (fSizeZ/2));

		// 頂点データをアンロックする
		g_pVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorShadow(int nIdxShadow, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtxをインデックス分ズラす
		pVtx += nIdxShadow * 4;

		// 頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点データをアンロックする
		g_pVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// 影の作成
//=============================================================================
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(!g_aShadow[nCntShadow].bUse)
		{
			// 影の位置、回転角を設定
			D3DXVECTOR3 tPos = pos;
			tPos.y = 0.1f;
			g_aShadow[nCntShadow].pos = tPos;

			// 影ワークを使用状態に変更
			g_aShadow[nCntShadow].bUse = true;

			// 頂点座標の設定関数を呼び出す
			SetVertexShadow( nCntShadow, fSizeX, fSizeZ);

			// 影の番号をnIdxShadowに設定
			nIdxShadow = nCntShadow;
			break;
		}
	}

	// nIdxShadowを返す
	return nIdxShadow;
}

//=============================================================================
// 影の破棄
//=============================================================================
void ReleaseShadow(int nIdxShadow)
{
	//指定された影が影ワークの個数ないかどうか
	if(nIdxShadow < MAX_SHADOW)
	{
		// 影ワークを未使用状態に設定
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// 位置の設定
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	// 指定された影ワークに位置を設定
	D3DXVECTOR3 tPos = pos;
	tPos.y = 0.1f;
	g_aShadow[nIdxShadow].pos = tPos;
}

