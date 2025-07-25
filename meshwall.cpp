/*******************************************************************************
*
* タイトル:		メッシュウォール処理
* プログラム名:	meshwall.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "meshwall.h"
#include "input.h"
#include "debugproc.h"
#include "hitCheck.h"
#include "fountain.h"
#include "stageManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MESH_WALL		(64)							// 壁の総数
#define	TEXTURE_FILENAME	"data/TEXTURE/wall000.jpg"	// 読み込むテクスチャファイル名
#define	VALUE_MOVE_WALL		(5.0f)						// 移動速度
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)			// 回転速度


//**************************************
// メッシュウォール構造体
//**************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;		// インデックスバッファへのポインタ

	D3DXMATRIX mtxWorldField;				// ワールドマトリックス  
	D3DXVECTOR3 pos;						// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rot;						// ポリゴンの回転角

	int nNumBlockX, nNumBlockY;				// ブロック数
	int nNumVertex;							// 総頂点数
	int nNumVertexIndex;					// 総インデックス数
	int nNumPolygon;						// 総ポリゴン数
	float fSizeBlockX, fSizeBlockY;			// ブロックサイズ
} MESH_WALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice, MESH_WALL *pMesh);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;	// テクスチャへのポインタ

MESH_WALL g_aMeshWall[MAX_MESH_WALL];		// メッシュ壁ワーク
int g_nNumMeshWall = 0;						// メッシュ壁の数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_WALL *pMesh;

	if(g_nNumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	if(!g_pTextureWall)
	{
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
									TEXTURE_FILENAME,	// ファイルの名前
									&g_pTextureWall);	// 読み込むメモリー
	}

	// メッシュ壁構造体のポインタに今回使用するメッシュ壁ワークのアドレスを代入
	pMesh = &g_aMeshWall[g_nNumMeshWall];

	// メッシュ壁の数を増やす
	g_nNumMeshWall++;

	// 位置・向きの初期設定
	pMesh->pos = pos;
	pMesh->rot = rot;

	// ブロック数の設定
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;
	
	// 頂点数の設定
	pMesh->nNumVertex = (nNumBlockX+1) * (nNumBlockY+1);
	
	// インデックス数の設定
	pMesh->nNumVertexIndex = (nNumBlockX + 1)*2*nNumBlockY + (nNumBlockY - 1)*2;
	
	// ポリゴン数の設定
	pMesh->nNumPolygon = nNumBlockX*nNumBlockY*2 + (nNumBlockY - 1)*4;
	
	// ブロックサイズの設定
	pMesh->fSizeBlockX = fSizeBlockX;
	pMesh->fSizeBlockY = fSizeBlockY;

	// 頂点情報の作成
	MakeVertexField(pDevice, pMesh);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++)
	{
		// メッシュ壁構造体のポインタにメッシュ壁ワークのアドレスを代入
		pMesh = &g_aMeshWall[nCntMeshWall];

		if(pMesh->pD3DVtxBuff)
		{// 頂点バッファの開放
			pMesh->pD3DVtxBuff->Release();
			pMesh->pD3DVtxBuff = NULL;
		}

		if(pMesh->pD3DIdxBuff)
		{// インデックスバッファの開放
			pMesh->pD3DIdxBuff->Release();
			pMesh->pD3DIdxBuff = NULL;
		}
	}

	if(g_pTextureWall)
	{// テクスチャの開放
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//メッシュ壁の数リセット
	g_nNumMeshWall = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_WALL *pMesh;
	D3DXMATRIX mtxRot, mtxTranslate;

	// メッシュ壁構造体のポインタにメッシュ壁ワークの先頭アドレスを代入
	pMesh = g_aMeshWall;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++, pMesh++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &pMesh->mtxWorldField);
	
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll( &mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxRot);
	
		// 移動を反映
		D3DXMatrixTranslation( &mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxTranslate);
	
		// ワールドマトリックスの設定
		pDevice->SetTransform( D3DTS_WORLD, &pMesh->mtxWorldField);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, pMesh->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(pMesh->pD3DIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);
	
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,						//最初の頂点インデックスまでのオフセット
			0,						//最小の頂点インデックス
			pMesh->nNumVertex,		//頂点の数
			0,						//読み取りを開始する位置
			pMesh->nNumPolygon		//ポリゴンの数
		);

		//噴水は最初の部屋の中しか見えない描画順
		if(nCntMeshWall == 7 && GetStageMode() == STAGE0)
		{
			DrawFountain();
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice, MESH_WALL *pMesh)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * pMesh->nNumVertex,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&pMesh->pD3DVtxBuff,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * pMesh->nNumVertexIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pMesh->pD3DIdxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	
	{//頂点バッファの中身を埋める

		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntY = 0; nCntY < pMesh->nNumBlockY+1; nCntY++)
		{
			for(int nCntX = 0; nCntX < pMesh->nNumBlockX+1; nCntX++)
			{
				pVtx[0].vtx = D3DXVECTOR3( -pMesh->fSizeBlockX *pMesh->nNumBlockX /2 + nCntX*pMesh->fSizeBlockX, pMesh->fSizeBlockY * pMesh->nNumBlockY/2 - nCntY*pMesh->fSizeBlockY, 0.0f);
				pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
				pVtx[0].nor = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
				pVtx[0].tex = D3DXVECTOR2( 1.0f * nCntX, 1.0f * nCntY );
				pVtx++;
			}
		}


		// 頂点データをアンロックする
		pMesh->pD3DVtxBuff->Unlock();	
	}


	{//インデックスバッファの中身を埋める

		WORD *pIdx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pMesh->pD3DIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

		for(int nCntY = 0; nCntY < pMesh->nNumBlockY; nCntY++)
		{
			if(nCntY > 0)
			{
				pIdx[0] = (pMesh->nNumBlockX + 1) * (nCntY + 1);
				pIdx++;
			}
			for(int nCntX = 0; nCntX < pMesh->nNumBlockX+1; nCntX++, pIdx+=2)
			{
				//頂点座標の設定
				pIdx[0] = (pMesh->nNumBlockX + 1) * (nCntY + 1) + nCntX; 
				pIdx[1] = (pMesh->nNumBlockX + 1) * (nCntY) + nCntX;
			}
			if(nCntY < pMesh->nNumBlockY - 1)
			{
				*pIdx = *(pIdx-1);
				pIdx++;
			}
		}

		// 頂点データをアンロックする
		pMesh->pD3DIdxBuff->Unlock();	
	}

	return S_OK;
}
bool HitCheckMeshWall( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, D3DXVECTOR3 *wall_nor, D3DXVECTOR3 *HitPoint)
{
	bool bHit = false;
	D3DXVECTOR3 tHitPosNear = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXVECTOR3 tPos[4];


	// メッシュ壁構造体のポインタにメッシュ壁ワークの先頭アドレスを代入
	MESH_WALL *pMesh = g_aMeshWall;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++, pMesh++)
	{

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &pMesh->mtxWorldField);
	
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll( &mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxRot);
	
		// 移動を反映
		D3DXMatrixTranslation( &mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxTranslate);

		{//頂点バッファの取得

			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pMesh->pD3DVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

			tPos[0] = pVtx[0].vtx;
			tPos[1] = pVtx[pMesh->nNumBlockX].vtx;
			tPos[2] = pVtx[(pMesh->nNumBlockX+1) * pMesh->nNumBlockY].vtx;
			tPos[3] = pVtx[(pMesh->nNumBlockX+1) * (pMesh->nNumBlockY+1) - 1 ].vtx;


			// 頂点データをアンロックする
			pMesh->pD3DVtxBuff->Unlock();	
		}

		//頂点バッファにワールドマトリックスを掛ける、頂点バッファが更新される
		D3DXVec3TransformCoord( &tPos[0], &tPos[0], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[1], &tPos[1], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[2], &tPos[2], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[3], &tPos[3], &pMesh->mtxWorldField);

		if( HitCheck( tPos[0], tPos[1], tPos[2], tNowPos, tNextPos) )
		{
			bHit = true;

			//一番近い交点を求める
			D3DXVECTOR3 tHitPos = GetHitPos();
			D3DXVECTOR3 v1 = tHitPosNear - tNowPos;
			D3DXVECTOR3 v2 = tHitPos - tNowPos;
			if( D3DXVec3Length(&v1) > D3DXVec3Length(&v2))
			{
				tHitPosNear = tHitPos;

				//壁の法線を取得
				if(wall_nor != NULL) *wall_nor = CrossProduct( tPos[0] - tPos[1], tPos[2] - tPos[1]);
			}
			
		}
		else if( HitCheck( tPos[1], tPos[2], tPos[3], tNowPos, tNextPos) )
		{
			bHit = true;

			//一番近い交点を求める
			D3DXVECTOR3 tHitPos = GetHitPos();
			D3DXVECTOR3 v1 = tHitPosNear - tNowPos;
			D3DXVECTOR3 v2 = tHitPos - tNowPos;
			if( D3DXVec3Length(&v1) > D3DXVec3Length(&v2))
			{
				tHitPosNear = tHitPos;

				//壁の法線を取得
				if(wall_nor != NULL) *wall_nor = CrossProduct( tPos[1] - tPos[2], tPos[3] - tPos[2]);
			}
		}
	}

	//一番近い交点を渡す
	if(HitPoint != NULL) *HitPoint = tHitPosNear;

	return bHit;
}