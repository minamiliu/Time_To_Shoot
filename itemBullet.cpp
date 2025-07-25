/*******************************************************************************
*
* タイトル:		アイテム(弾)の処理
* プログラム名:	itemBullet.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "itemBullet.h"
#include "debugproc.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "player.h"
#include "playerBulletNum.h"
#include "shadow.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_BULLET		"data/MODEL/item_bullet.x"					// 読み込むテクスチャファイル名

#define VALUE_MOVE		(0.0f)									// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.01f) 						// 回転量

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(100)
#define MODEL_POS_Z	(0)



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
ITEM_BULLET g_ItemBullet[MAX_ITEM_BULLET];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItemBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		// 位置・回転・スケールの初期設定
		g_ItemBullet[nCntItem].pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
		g_ItemBullet[nCntItem].rot = D3DXVECTOR3( D3DXToRadian(30.0f), 0.0f, 0.0f);
		g_ItemBullet[nCntItem].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_ItemBullet[nCntItem].bUse = false;

		// モデルに関する変数の初期化							
		g_ItemBullet[nCntItem].pD3DTexture = NULL;		// テクスチャへのポインタ
		g_ItemBullet[nCntItem].pD3DXMesh = NULL;		// メッシュ情報へのポインタ
		g_ItemBullet[nCntItem].pD3DXBuffMat = NULL;		// マテリアル情報へのポインタ
		g_ItemBullet[nCntItem].nNumMat = 0;				// マテリアル情報の数

		// Xファイルの読み込み
		if(FAILED(D3DXLoadMeshFromX(
			MODEL_BULLET,				// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
			pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
			NULL,						// 隣接性データを含むバッファへのポインタ
			&g_ItemBullet[nCntItem].pD3DXBuffMat,		// マテリアルデータを含むバッファへのポインタ
			NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_ItemBullet[nCntItem].nNumMat,			// D3DXMATERIAL構造体の数
			&g_ItemBullet[nCntItem].pD3DXMesh			// ID3DXMeshインターフェイスへのポインタのアドレス
			)))
		{
			return E_FAIL;
		}

	}
													
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItemBullet(void)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		// メッシュの開放
		if(g_ItemBullet[nCntItem].pD3DXMesh != NULL)
		{
			g_ItemBullet[nCntItem].pD3DXMesh->Release();
			g_ItemBullet[nCntItem].pD3DXMesh = NULL;
		}
		// マテリアルの開放
		if(g_ItemBullet[nCntItem].pD3DXBuffMat != NULL)
		{
			g_ItemBullet[nCntItem].pD3DXBuffMat->Release();
			g_ItemBullet[nCntItem].pD3DXBuffMat = NULL;
		}
		g_ItemBullet[nCntItem].bUse = false;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItemBullet( void)
{
	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();

	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(g_ItemBullet[nCntItem].bUse)
		{
			//アイテムを回転させる
			g_ItemBullet[nCntItem].rot.y += VALUE_ROTATE;

			if( HitCheckBall( g_ItemBullet[nCntItem].pos, 10.0f, pCamera->posV, 30.0f))
			{
				pPlayer->nBullet = 3;
				ChangeNumBullet( 0, pPlayer->nBullet);
				g_ItemBullet[nCntItem].bUse = false;
				ReleaseShadow( g_ItemBullet[nCntItem].nIdxShadow);

				PlaySound(SOUND_LABEL_SE_RELOAD);
			}
		}
	}



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItemBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(g_ItemBullet[nCntItem].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_ItemBullet[nCntItem].mtxWorld);
	
			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_ItemBullet[nCntItem].scl.x, g_ItemBullet[nCntItem].scl.y, g_ItemBullet[nCntItem].scl.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxScl);
	
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_ItemBullet[nCntItem].rot.y, g_ItemBullet[nCntItem].rot.x, g_ItemBullet[nCntItem].rot.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxRot);
	
			// 移動を反映
			D3DXMatrixTranslation( &mtxTranslate, g_ItemBullet[nCntItem].pos.x, g_ItemBullet[nCntItem].pos.y, g_ItemBullet[nCntItem].pos.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxTranslate);
	
			// ワールドマトリックスの設定
			pDevice->SetTransform( D3DTS_WORLD, &g_ItemBullet[nCntItem].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
	
			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL *)g_ItemBullet[nCntItem].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_ItemBullet[nCntItem].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture( 0, g_ItemBullet[nCntItem].pD3DTexture);
				// 描画
				g_ItemBullet[nCntItem].pD3DXMesh->DrawSubset( nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial( &matDef);		
		}
	}
}

//=============================================================================
// アイテムの設置
//=============================================================================
void SetItemBullet( D3DXVECTOR3 pos)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(!g_ItemBullet[nCntItem].bUse) 
		{
			//位置を設定
			pos.y = MODEL_POS_Y;
			g_ItemBullet[nCntItem].pos = pos;
			g_ItemBullet[nCntItem].bUse = true;
		
			//shadow
			g_ItemBullet[nCntItem].nIdxShadow = CreateShadow( g_ItemBullet[nCntItem].pos, 10.0f, 10.0f);
			break;
		}
	}	
}


