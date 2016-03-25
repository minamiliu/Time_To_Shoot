/*******************************************************************************
* タイトル:		フィールド処理
* プログラム名:	field.cpp
* 作成者:		TH教務部　寄光
* 作成日:		2015/10/20
********************************************************************************
* 更新履歴:		- 2015/10/20	 寄光
*					- V1.00		Initial Version
*******************************************************************************/
/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "field.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FIELD	(9)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/field000.jpg"						// 読み込むテクスチャファイル名


#define VALUE_MOVE		(5.0f)											// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 								// 回転量

#define SIZE_X			(100.0f)										// 地面のサイズ(X方向)
#define SIZE_Z			(100.0f)										// 地面のサイズ(Z方向)
																		
																		

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField;								// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;								// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxWorldField;									// ワールドマトリックス

FIELD g_field[MAX_FIELD];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 位置・回転・スケールの初期設定
	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_field[nCntField].posField = D3DXVECTOR3( 0.0f + nCntField%3*SIZE_X, 0.0f, 0.0f + nCntField/3*SIZE_X);
		g_field[nCntField].rotField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_field[nCntField].sclField = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);	
	}

	g_pD3DTextureField = NULL;	
	g_pD3DVtxBuffField = NULL;
	
	// 頂点情報の作成
	MakeVertexField(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureField);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		
	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &g_mtxWorldField);

		// スケールを反映
		D3DXMatrixScaling( &mtxScl, g_field[nCntField].sclField.x, g_field[nCntField].sclField.y, g_field[nCntField].sclField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxScl);
	
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_field[nCntField].rotField.y, g_field[nCntField].rotField.x, g_field[nCntField].rotField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	
		// 移動を反映
		D3DXMatrixTranslation( &mtxTranslate, g_field[nCntField].posField.x, g_field[nCntField].posField.y, g_field[nCntField].posField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
		// ワールドマトリックスの設定
		pDevice->SetTransform( D3DTS_WORLD, &g_mtxWorldField);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureField);

	}
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,						//ロードする最初の頂点インデックス
			NUM_POLYGON*MAX_FIELD	//ポリゴンの数
		);	


}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_FIELD,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffField,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pVtx+=4)
	{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_field[nCntField].posField.x - (SIZE_X/2), 0.0f, g_field[nCntField].posField.z + (SIZE_Z/2));
		pVtx[1].vtx = D3DXVECTOR3(g_field[nCntField].posField.x + (SIZE_X/2), 0.0f, g_field[nCntField].posField.z + (SIZE_Z/2));
		pVtx[2].vtx = D3DXVECTOR3(g_field[nCntField].posField.x - (SIZE_X/2), 0.0f, g_field[nCntField].posField.z - (SIZE_Z/2));
		pVtx[3].vtx = D3DXVECTOR3(g_field[nCntField].posField.x + (SIZE_X/2), 0.0f, g_field[nCntField].posField.z - (SIZE_Z/2));

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

		// 反射光の設定
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
	g_pD3DVtxBuffField->Unlock();

	return S_OK;
}




