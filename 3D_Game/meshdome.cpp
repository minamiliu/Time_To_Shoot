/*******************************************************************************
*
* タイトル:		メッシュドーム処理
* プログラム名:	meshdome.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "meshdome.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_MESHDOME	"data/TEXTURE/dome.jpg"	// 読み込むテクスチャファイル名
#define	VALUE_MOVE_DOME		(4.0f)						// 移動速度
#define	VALUE_ROTATE_DOME	(D3DX_PI * 0.01f)			// 回転速度

#define	DOME_HEIGHT_RATE	(1.0f)		// 空ドームの高さ係数
#define	TEX_COUNT_LOOP		(8)			// テクスチャの繰り返し回数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureDome;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDome;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffDome;		// インデックスバッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDomeTop;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffDomeTop;		// インデックスバッファへのポインタ

	D3DXMATRIX mtxWorldDome;					// ワールドマトリックス
	D3DXVECTOR3 posDome;						// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rotDome;						// ポリゴンの回転角

	int nNumBlockHDome;						// ブロック数
	int nNumBlockVDome;						// ブロック数
	int nNumVertexDome;						// 総頂点数	
	int nNumVertexIndexDome;				// 総インデックス数
	int nNumPolygonDome;					// 総ポリゴン数
	float fRadius;							// 半径
} MESH_DOME;

MESH_DOME g_aMeshDome;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nNumBlockH, int nNumBlockV)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向き・半径（グローバル変数）の初期設定
	g_aMeshDome.posDome = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_aMeshDome.rotDome = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_aMeshDome.fRadius = fRadius;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_MESHDOME, &g_aMeshDome.pTextureDome);

	// ブロック数H/V（グローバル変数）の設定
	g_aMeshDome.nNumBlockHDome = nNumBlockH;
	g_aMeshDome.nNumBlockVDome = nNumBlockV;

	// 頂点数（グローバル変数）の設定
	g_aMeshDome.nNumVertexDome = (g_aMeshDome.nNumBlockHDome + 1) * (g_aMeshDome.nNumBlockVDome + 1);

	// ポリゴン数（グローバル変数）の設定
	g_aMeshDome.nNumPolygonDome = ( g_aMeshDome.nNumBlockHDome * (g_aMeshDome.nNumBlockVDome * 2) + (g_aMeshDome.nNumBlockVDome - 1) * 4);

	// インデックス数（グローバル変数）の設定
	g_aMeshDome.nNumVertexIndexDome = (g_aMeshDome.nNumBlockHDome * (g_aMeshDome.nNumBlockVDome * 2) + g_aMeshDome.nNumBlockVDome * 2 + (g_aMeshDome.nNumBlockVDome - 1) * 2);

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome.nNumVertexDome,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_aMeshDome.pVtxBuffDome,					// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_aMeshDome.nNumBlockHDome + 1),		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&g_aMeshDome.pVtxBuffDomeTop,						// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumVertexIndexDome,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_aMeshDome.pIdxBuffDome,					// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (g_aMeshDome.nNumBlockHDome + 2),			// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_aMeshDome.pIdxBuffDomeTop,					// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		const float angleH = (D3DX_PI * 2.0f) / g_aMeshDome.nNumBlockHDome;
		const float angleV = (D3DX_PI / 2.0f) / (g_aMeshDome.nNumBlockVDome + 1);
		const float width = 1.0f / g_aMeshDome.nNumBlockHDome;
		const float height = 1.0f / g_aMeshDome.nNumBlockVDome;
		float lengthXZ;
		D3DXVECTOR3 nor;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_aMeshDome.pVtxBuffDome->Lock( 0, 0, (void**)&pVtx, 0);
		for(int nCntV = 0; nCntV < (g_aMeshDome.nNumBlockVDome + 1); nCntV++)
		{
			lengthXZ = cosf(angleV * nCntV) * g_aMeshDome.fRadius;

			for(int nCntH = 0; nCntH < (g_aMeshDome.nNumBlockHDome + 1); nCntH++, pVtx++)
			{
				// 頂点座標の設定
				pVtx->vtx.x = sinf(angleH * nCntH) * lengthXZ;
				pVtx->vtx.y = sinf(angleV * nCntV) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
				pVtx->vtx.z = -cosf(angleH * nCntH) * lengthXZ;

				// 法線の設定
				nor = pVtx->vtx;
				D3DXVec3Normalize(&nor, &nor);
				pVtx->nor = nor;

				// 頂点カラーの設定
				pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

				// テクスチャ座標の設定
				pVtx->tex.x = nCntH * width * TEX_COUNT_LOOP;

				if(nCntV == g_aMeshDome.nNumBlockVDome)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * height;
				}
			}
		}
		// 頂点データをアンロックする
		g_aMeshDome.pVtxBuffDome->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_aMeshDome.pVtxBuffDomeTop->Lock( 0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx->vtx.x = 0.0f;
		pVtx->vtx.y = sinf(angleV * (g_aMeshDome.nNumBlockVDome + 1)) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
		pVtx->vtx.z = 0.0f;

		// 法線の設定
		nor = pVtx->vtx;
		D3DXVec3Normalize(&nor, &nor);
		pVtx->nor = nor;

		// 頂点カラーの設定
		pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

		// テクスチャ座標の設定
		pVtx->tex.x = 0.5f;
		pVtx->tex.y = 0.0f;

		// 頂点バッファのポインタをインクリメント
		pVtx++;

		lengthXZ = cosf(angleV * g_aMeshDome.nNumBlockVDome) * g_aMeshDome.fRadius;
		for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome; nCntH++, pVtx++)
		{
			// 頂点座標の設定
			pVtx->vtx.x = -sinf(angleH * nCntH) * lengthXZ;
			pVtx->vtx.y = sinf(angleV * g_aMeshDome.nNumBlockVDome) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
			pVtx->vtx.z = -cosf(angleH * nCntH) * lengthXZ;

			// 法線の設定
			nor = pVtx->vtx;
			D3DXVec3Normalize(&nor, &nor);
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

			// テクスチャ座標の設定
			pVtx->tex.x = 0.5f;
			pVtx->tex.y = 1.0f;
		}

		// 頂点データをアンロックする
		g_aMeshDome.pVtxBuffDomeTop->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_aMeshDome.pIdxBuffDome->Lock( 0, 0, (void**)&pIdx, 0);
		for(int nCntV = 0; nCntV < g_aMeshDome.nNumBlockVDome; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (g_aMeshDome.nNumBlockHDome + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome + 1; nCntH++)
			{
				*pIdx = nCntV * (g_aMeshDome.nNumBlockHDome + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (g_aMeshDome.nNumBlockHDome + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (g_aMeshDome.nNumBlockVDome - 1))
			{
				*pIdx = (nCntV + 1) * (g_aMeshDome.nNumBlockHDome + 1);
				pIdx++;
			}
		}

		// インデックスデータをアンロックする
		g_aMeshDome.pIdxBuffDome->Unlock();

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_aMeshDome.pIdxBuffDomeTop->Lock( 0, 0, (void**)&pIdx, 0);
		for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// インデックスデータをアンロックする
		g_aMeshDome.pIdxBuffDomeTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshDome(void)
{
	if(g_aMeshDome.pVtxBuffDome != NULL)
	{// 頂点バッファの開放
		g_aMeshDome.pVtxBuffDome->Release();
		g_aMeshDome.pVtxBuffDome = NULL;
	}

	if(g_aMeshDome.pIdxBuffDome != NULL)
	{// インデックスバッファの開放
		g_aMeshDome.pIdxBuffDome->Release();
		g_aMeshDome.pIdxBuffDome = NULL;
	}

	if(g_aMeshDome.pVtxBuffDomeTop != NULL)
	{// 頂点バッファの開放
		g_aMeshDome.pVtxBuffDomeTop->Release();
		g_aMeshDome.pVtxBuffDomeTop = NULL;
	}

	if(g_aMeshDome.pIdxBuffDomeTop != NULL)
	{// インデックスバッファの開放
		g_aMeshDome.pIdxBuffDomeTop->Release();
		g_aMeshDome.pIdxBuffDomeTop = NULL;
	}

	if(g_aMeshDome.pTextureDome != NULL)
	{// テクスチャの開放
		g_aMeshDome.pTextureDome->Release();
		g_aMeshDome.pTextureDome = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshDome(void)
{

	//PrintDebugProc("*** 半球情報 ***\n");
	//PrintDebugProc("位置           : (%f : %f : %f)\n", g_posDome.x, g_posDome.y, g_posDome.z);
	//PrintDebugProc("向き           : (%f : %f : %f)\n", g_rotDome.x, g_rotDome.y, g_rotDome.z);
	//PrintDebugProc("ブロック数     : (%d x %d)ブロック\n", g_nNumBlockHDome, g_nNumBlockVDome);
	//PrintDebugProc("頂点数         : %d頂点 + %d頂点\n", 頂点数, g_nNumBlockHDome + 1);
	//PrintDebugProc("インデックス数 : %d個 + %d個\n", g_nNumVertexIndexDome, g_nNumBlockHDome + 2);
	//PrintDebugProc("ポリゴン数     : %dポリゴン + %dポリゴン\n", g_nNumPolygonDome, g_nNumBlockHDome);
	//PrintDebugProc("\n");

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// 裏面をカリング

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_aMeshDome.mtxWorldDome);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aMeshDome.rotDome.y, g_aMeshDome.rotDome.x, g_aMeshDome.rotDome.z);
	D3DXMatrixMultiply( &g_aMeshDome.mtxWorldDome, &g_aMeshDome.mtxWorldDome, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation( &mtxTranslate, g_aMeshDome.rotDome.x, g_aMeshDome.rotDome.y, g_aMeshDome.rotDome.z);
	D3DXMatrixMultiply( &g_aMeshDome.mtxWorldDome, &g_aMeshDome.mtxWorldDome, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform( D3DTS_WORLD, &g_aMeshDome.mtxWorldDome);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_aMeshDome.pVtxBuffDome, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_aMeshDome.pIdxBuffDome);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aMeshDome.pTextureDome);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,								//最初の頂点インデックスまでのオフセット
		0,								//最小の頂点インデックス
		g_aMeshDome.nNumVertexDome,		//頂点の数
		0,								//読み取りを開始する位置
		g_aMeshDome.nNumPolygonDome		//ポリゴンの数
	);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_aMeshDome.pVtxBuffDomeTop, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_aMeshDome.pIdxBuffDomeTop);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLEFAN,				//プリミティブの種類
		0,								//最初の頂点インデックスまでのオフセット
		0,								//最小の頂点インデックス
		(g_aMeshDome.nNumBlockHDome + 1),		//頂点の数
		0,								//読み取りを開始する位置
		g_aMeshDome.nNumBlockHDome		//ポリゴンの数
	);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング
}

