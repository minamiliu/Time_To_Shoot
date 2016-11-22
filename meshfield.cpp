/*******************************************************************************
*
* タイトル:		メッシュフィールド処理
* プログラム名:	meshfield.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "meshfield.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_FIELD	"data/TEXTURE/field002.jpg"						// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;	// インデックスバッファへのポインタ

D3DXMATRIX g_mtxWorldField;					// ワールドマトリックス
D3DXVECTOR3 g_posField;						// 位置
D3DXVECTOR3 g_rotField;						// 向き

int g_nNumBlockX, g_nNumBlockZ;				// ブロック数
int g_nNumVertex;							// 総頂点数
int g_nNumVertexIndex;						// 総インデックス数
int g_nNumPolygon;							// 総ポリゴン数
float g_fSizeBlockX, g_fSizeBlockZ;			// ブロックサイズ


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・回転・スケールの初期設定

	g_posField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	g_nNumBlockX = nNumBlockX;
	g_nNumBlockZ = nNumBlockZ;
	g_fSizeBlockX = fSizeBlockX;
	g_fSizeBlockZ = fSizeBlockZ;

	g_nNumVertex = (nNumBlockX+1) * (nNumBlockZ+1);
	g_nNumVertexIndex = (nNumBlockX + 1)*2*nNumBlockZ + (nNumBlockZ - 1)*2;
	g_nNumPolygon = nNumBlockX*nNumBlockZ*2 + (nNumBlockZ - 1)*4;

	g_pD3DTextureField = NULL;	
	g_pD3DVtxBuffField = NULL;
	g_pD3DIdxBuffField = NULL;
	
	// 頂点情報の作成
	MakeVertexField(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureField);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
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
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_mtxWorldField);
	
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	
	// 移動を反映
	D3DXMatrixTranslation( &mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
	// ワールドマトリックスの設定
	pDevice->SetTransform( D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_pD3DIdxBuffField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureField);
	
	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//最初の頂点インデックスまでのオフセット
		0,						//最小の頂点インデックス
		g_nNumVertex,			//頂点の数
		0,						//読み取りを開始する位置
		g_nNumPolygon			//ポリゴンの数
	);



}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nNumVertex,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffField,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * g_nNumVertexIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pD3DIdxBuffField,
		NULL)))
	{
		return E_FAIL;
	}

	
	{//頂点バッファの中身を埋める

		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntZ = 0; nCntZ < g_nNumBlockZ+1; nCntZ++)
		{
			for(int nCntX = 0; nCntX < g_nNumBlockX+1; nCntX++)
			{
				pVtx[0].vtx = D3DXVECTOR3( -g_fSizeBlockX*g_nNumBlockX/2 + nCntX*g_fSizeBlockX, 0.0f, g_fSizeBlockZ*g_nNumBlockZ/2 - nCntZ*g_fSizeBlockZ);
				pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
				pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
				pVtx[0].tex = D3DXVECTOR2( 1.0f * nCntX, 1.0f * nCntZ );
				pVtx++;
			}
		}


		// 頂点データをアンロックする
		g_pD3DVtxBuffField->Unlock();	
	}


	{//インデックスバッファの中身を埋める

		WORD *pIdx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0);

		for(int nCntZ = 0; nCntZ < g_nNumBlockZ; nCntZ++)
		{
			if(nCntZ > 0)
			{
				pIdx[0] = (g_nNumBlockX + 1) * (nCntZ + 1);
				pIdx++;
			}
			for(int nCntX = 0; nCntX < g_nNumBlockX+1; nCntX++, pIdx+=2)
			{
				//頂点座標の設定
				pIdx[0] = (g_nNumBlockX + 1) * (nCntZ + 1) + nCntX; 
				pIdx[1] = (g_nNumBlockX + 1) * (nCntZ) + nCntX;
			}
			if(nCntZ < g_nNumBlockZ - 1)
			{
				*pIdx = *(pIdx-1);
				pIdx++;
			}
		}

		// 頂点データをアンロックする
		g_pD3DIdxBuffField->Unlock();	
	}

	return S_OK;
}

