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
#include "enemyBoard.h"
#include "input.h"
#include "camera.h"
#include "hitCheck.h"
#include "debugproc.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/xxx.png"				// 読み込むテクスチャファイル名


#define VALUE_MOVE		(5.0f)								// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 					// 回転量

#define SIZE_X			(30.0f)								// 地面のサイズ(X方向)
#define SIZE_Y			(30.0f)								// 地面のサイズ(Z方向)
																		
																		

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEnemyBoard(LPDIRECT3DDEVICE9 pDevice);
void ResetVertexEnemyBoard(int nIdxBoard);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMYBOARD g_aEnemyBoard[MAX_ENEMYBOARD];
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemyBoard;								// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEnemyBoard;								// 頂点バッファインターフェースへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 位置・回転・スケールの初期設定
	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++)
	{
		g_aEnemyBoard[nCntBoard].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aEnemyBoard[nCntBoard].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aEnemyBoard[nCntBoard].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_aEnemyBoard[nCntBoard].bUse = false;
	}

	g_pD3DVtxBuffEnemyBoard = NULL;

	
	// 頂点情報の作成
	MakeVertexEnemyBoard(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureEnemyBoard);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyBoard(void)
{
	if(g_pD3DTextureEnemyBoard != NULL)
	{// テクスチャの開放
		g_pD3DTextureEnemyBoard->Release();
		g_pD3DTextureEnemyBoard = NULL;
	}


	if(g_pD3DVtxBuffEnemyBoard != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEnemyBoard->Release();
		g_pD3DVtxBuffEnemyBoard = NULL;
	}	


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyBoard(void)
{
	CAMERA *pCamera = GetCamera();
	ENEMY *pEnemy = GetEnemy();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++, pVtx+=4)
	{
		if(!g_aEnemyBoard[nCntBoard].bUse) continue;

		ResetVertexEnemyBoard(nCntBoard);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &g_aEnemyBoard[nCntBoard].mtxWorld);

		//ワールドマトリックスにカメラの行列を転置行列として設定する
		g_aEnemyBoard[nCntBoard].mtxWorld._11 = pCamera->mtxView._11;
		g_aEnemyBoard[nCntBoard].mtxWorld._22 = pCamera->mtxView._22;//垂直方向
		g_aEnemyBoard[nCntBoard].mtxWorld._33 = pCamera->mtxView._33;

		g_aEnemyBoard[nCntBoard].mtxWorld._12 = pCamera->mtxView._21;
		g_aEnemyBoard[nCntBoard].mtxWorld._13 = pCamera->mtxView._31;
		g_aEnemyBoard[nCntBoard].mtxWorld._23 = pCamera->mtxView._32;//垂直方向

		g_aEnemyBoard[nCntBoard].mtxWorld._21 = pCamera->mtxView._12;//垂直方向
		g_aEnemyBoard[nCntBoard].mtxWorld._31 = pCamera->mtxView._13;
		g_aEnemyBoard[nCntBoard].mtxWorld._32 = pCamera->mtxView._23;

		// スケールを反映
		D3DXMatrixScaling( &mtxScl, g_aEnemyBoard[nCntBoard].scl.x, g_aEnemyBoard[nCntBoard].scl.y, g_aEnemyBoard[nCntBoard].scl.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxScl);
	
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aEnemyBoard[nCntBoard].rot.y, g_aEnemyBoard[nCntBoard].rot.x, g_aEnemyBoard[nCntBoard].rot.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxRot);
	
		// 移動を反映
		D3DXMatrixTranslation( &mtxTranslate, g_aEnemyBoard[nCntBoard].pos.x, g_aEnemyBoard[nCntBoard].pos.y, g_aEnemyBoard[nCntBoard].pos.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxTranslate);

		//頂点バッファにワールドマトリックスを掛ける、頂点バッファが更新される
		D3DXVec3TransformCoord( &pVtx[0].vtx, &pVtx[0].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[1].vtx, &pVtx[1].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[2].vtx, &pVtx[2].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[3].vtx, &pVtx[3].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffEnemyBoard->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	CAMERA *pCamera = GetCamera();

	// ライティングモードをOFF
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
		
	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++)
	{

		if(!g_aEnemyBoard[nCntBoard].bUse) continue;

		//// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &g_aEnemyBoard[nCntBoard].mtxWorld);
		
		// ワールドマトリックスの設定
		pDevice->SetTransform( D3DTS_WORLD, &g_aEnemyBoard[nCntBoard].mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffEnemyBoard, 0, sizeof(VERTEX_3D));
	
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEnemyBoard);

		//αテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntBoard*NUM_VERTEX,	//ロードする最初の頂点インデックス
			NUM_POLYGON				//ポリゴンの数
		);	
	}



	// ライティングモードをON
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEnemyBoard(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ENEMYBOARD,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffEnemyBoard,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++, pVtx+=4)
	{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3( 0.0f - (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3( 0.0f + (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3( 0.0f - (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3( 0.0f + (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);

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
	g_pD3DVtxBuffEnemyBoard->Unlock();

	return S_OK;
}

bool HitCheckEnemyBoard( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, int nIdxBoard)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	// pVtxをインデックス分ズラす
	pVtx += nIdxBoard * 4;

	if( HitCheck( pVtx[0].vtx, pVtx[1].vtx, pVtx[2].vtx, tNowPos, tNextPos) )
	{
		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemyBoard->Unlock();
		return true;
	}
	if( HitCheck( pVtx[1].vtx, pVtx[2].vtx, pVtx[3].vtx, tNowPos, tNextPos) )
	{
		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemyBoard->Unlock();
		return true;
	}


	// 頂点データをアンロックする
	g_pD3DVtxBuffEnemyBoard->Unlock();


	return false;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void ResetVertexEnemyBoard(int nIdxBoard)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtxをインデックス分ズラす
		pVtx += nIdxBoard * 4;

		// 頂点座標の設定（引数を用いて設定）
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemyBoard->Unlock();
	}
}

void SetVertexEnemyBoard(int nIdxBoard, float fSizeX, float fSizeZ)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtxをインデックス分ズラす
		pVtx += nIdxBoard * 4;

		// 頂点座標の設定（引数を用いて設定）
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f + (fSizeZ/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f + (fSizeZ/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f - (fSizeZ/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f - (fSizeZ/2), 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemyBoard->Unlock();
	}
}

int CreateEnemyBoard(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxBoard = -1;

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMY; nCntBoard++)
	{
		if(!g_aEnemyBoard[nCntBoard].bUse)
		{
			// 影の位置、回転角を設定
			D3DXVECTOR3 tPos = pos;
			tPos.y += 15.0f;
			g_aEnemyBoard[nCntBoard].pos = tPos;

			// 影ワークを使用状態に変更
			g_aEnemyBoard[nCntBoard].bUse = true;

			// 頂点座標の設定関数を呼び出す
			SetVertexEnemyBoard( nCntBoard, fSizeX, fSizeZ);

			// 影の番号をnIdxShadowに設定
			nIdxBoard = nCntBoard;
			break;
		}
	}

	// nIdxShadowを返す
	return nIdxBoard;
}

void ReleaseEnemyBoard(int nIdxBoard)
{
	//指定された影が影ワークの個数ないかどうか
	if(nIdxBoard < MAX_ENEMY)
	{
		// 影ワークを未使用状態に設定
		g_aEnemyBoard[nIdxBoard].bUse = false;
	}
}

//=============================================================================
// 位置の設定
//=============================================================================
void SetPositionEnemyBoard(int nIdxBoard, D3DXVECTOR3 pos)
{
	// 指定され位置に設定
	D3DXVECTOR3 tPos = pos;
	g_aEnemyBoard[nIdxBoard].pos.x = pos.x;
	g_aEnemyBoard[nIdxBoard].pos.z = pos.z;
}
