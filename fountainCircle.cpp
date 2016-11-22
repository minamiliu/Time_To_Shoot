/*******************************************************************************
*
* タイトル:		噴水のモデルの表示処理
* プログラム名:	fountainCircle.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "fountainCircle.h"
#include "debugproc.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLANE		"data/MODEL/fountain.x"				// 読み込むXファイル名

#define VALUE_MOVE		(5.0f)								// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 					// 回転量

#define MODEL_POS_X	(-700)
#define MODEL_POS_Y	(5)
#define MODEL_POS_Z	(200)

#define MODEL_SPHERE (20)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
FOUNTAIN_CIRCLE g_FountainCircle;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFountainCircle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 位置・回転・スケールの初期設定
	g_FountainCircle.pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
	g_FountainCircle.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.scl = D3DXVECTOR3( 10.0f, 5.0f, 10.0f);
	g_FountainCircle.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_FountainCircle.rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.fRadius = 150.0f;
	g_FountainCircle.bUse = true;
	
	// モデルに関する変数の初期化
	g_FountainCircle.pD3DXMesh = NULL;			// メッシュ情報へのポインタ
	g_FountainCircle.pD3DXBuffMat = NULL;		// マテリアル情報へのポインタ
	g_FountainCircle.nNumMat = 0;				// マテリアル情報の数

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLANE,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
		pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
		NULL,						// 隣接性データを含むバッファへのポインタ
		&g_FountainCircle.pD3DXBuffMat,		// マテリアルデータを含むバッファへのポインタ
		NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_FountainCircle.nNumMat,			// D3DXMATERIAL構造体の数
		&g_FountainCircle.pD3DXMesh			// ID3DXMeshインターフェイスへのポインタのアドレス
		)))
	{
		return E_FAIL;
	}

	//テクスチャへのポインタの初期化
	for(int nCntMat = 0; nCntMat < MAX_D3D_MAT; nCntMat++)
	{
		g_FountainCircle.pD3DTexture[nCntMat] = NULL;		// テクスチャへのポインタ
	}

	// マテリアル情報に対するポインタを取得
	D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL *)g_FountainCircle.pD3DXBuffMat->GetBufferPointer();
	if((int)g_FountainCircle.nNumMat <= MAX_D3D_MAT)
	{
		for(int nCntMat = 0; nCntMat < (int)g_FountainCircle.nNumMat; nCntMat++)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile( pDevice, pD3DXMat[nCntMat].pTextureFilename, &g_FountainCircle.pD3DTexture[nCntMat]);
		}
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFountainCircle(void)
{
	// テクスチャの開放
	for(int nCntMat = 0; nCntMat < MAX_D3D_MAT; nCntMat++)
	{
		if(g_FountainCircle.pD3DTexture[nCntMat] != NULL)
		{
			g_FountainCircle.pD3DTexture[nCntMat]->Release();
			g_FountainCircle.pD3DTexture[nCntMat] = NULL;
		}
	}

	// メッシュの開放
	if(g_FountainCircle.pD3DXMesh != NULL)
	{
		g_FountainCircle.pD3DXMesh->Release();
		g_FountainCircle.pD3DXMesh = NULL;
	}
	// マテリアルの開放
	if(g_FountainCircle.pD3DXBuffMat != NULL)
	{
		g_FountainCircle.pD3DXBuffMat->Release();
		g_FountainCircle.pD3DXBuffMat = NULL;
	}

	g_FountainCircle.bUse = false;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFountainCircle(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFountainCircle(void)
{
	if(!g_FountainCircle.bUse) return ;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_FountainCircle.mtxWorld);
	
	// スケールを反映
	D3DXMatrixScaling( &mtxScl, g_FountainCircle.scl.x, g_FountainCircle.scl.y, g_FountainCircle.scl.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxScl);
	
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_FountainCircle.rot.y, g_FountainCircle.rot.x, g_FountainCircle.rot.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxRot);
	
	// 移動を反映
	D3DXMatrixTranslation( &mtxTranslate, g_FountainCircle.pos.x, g_FountainCircle.pos.y, g_FountainCircle.pos.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxTranslate);
	
	// ワールドマトリックスの設定
	pDevice->SetTransform( D3DTS_WORLD, &g_FountainCircle.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL *)g_FountainCircle.pD3DXBuffMat->GetBufferPointer();
	
	for(int nCntMat = 0; nCntMat < (int)g_FountainCircle.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// テクスチャの設定
		pDevice->SetTexture( 0, g_FountainCircle.pD3DTexture[nCntMat]);

		// 描画
		g_FountainCircle.pD3DXMesh->DrawSubset( nCntMat);

	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial( &matDef);


}

//=============================================================================
// モデルの取得
//=============================================================================
FOUNTAIN_CIRCLE *GetFountainCircle(void)
{
	return &g_FountainCircle;
}


