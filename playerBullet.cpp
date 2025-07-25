/*******************************************************************************
*
* タイトル:		プレイヤーの弾の処理
* プログラム名:	playerBullet.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "playerBullet.h"
#include "debugproc.h"
#include "input.h"
//#include "shadow.h"
#include "camera.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "enemy.h"
#include "shadow.h"
#include "particle.h"
#include "itemBullet.h"
#include "stageManager.h"
#include "enemyNum.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_BULLET		"data/MODEL/player_bullet.x"					// 読み込むテクスチャファイル名

#define VALUE_MOVE		(0.0f)									// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 						// 回転量

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(100)
#define MODEL_POS_Z	(0)

#define MODEL_SPHERE (20)
#define BULLET_SPEED	(7.5f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER_BULLET g_P_Bullet[MAX_PLAYER_BULLET];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntBullet = 0; nCntBullet < MAX_PLAYER_BULLET; nCntBullet++)
	{
		// 位置・回転・スケールの初期設定
		g_P_Bullet[nCntBullet].pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
		g_P_Bullet[nCntBullet].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_P_Bullet[nCntBullet].scl = D3DXVECTOR3( 0.2f, 0.2f, 0.2f);
		g_P_Bullet[nCntBullet].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_P_Bullet[nCntBullet].rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_P_Bullet[nCntBullet].rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_P_Bullet[nCntBullet].bUse = false;

		// モデルに関する変数の初期化							
		g_P_Bullet[nCntBullet].pD3DTexture = NULL;		// テクスチャへのポインタ
		g_P_Bullet[nCntBullet].pD3DXMesh = NULL;		// メッシュ情報へのポインタ
		g_P_Bullet[nCntBullet].pD3DXBuffMat = NULL;		// マテリアル情報へのポインタ
		g_P_Bullet[nCntBullet].nNumMat = 0;				// マテリアル情報の数

		// Xファイルの読み込み
		if(FAILED(D3DXLoadMeshFromX(
			MODEL_BULLET,				// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
			pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
			NULL,						// 隣接性データを含むバッファへのポインタ
			&g_P_Bullet[nCntBullet].pD3DXBuffMat,		// マテリアルデータを含むバッファへのポインタ
			NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_P_Bullet[nCntBullet].nNumMat,			// D3DXMATERIAL構造体の数
			&g_P_Bullet[nCntBullet].pD3DXMesh			// ID3DXMeshインターフェイスへのポインタのアドレス
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
void UninitPlayerBullet(void)
{
	for(int nCntBullet = 0; nCntBullet < MAX_PLAYER_BULLET; nCntBullet++)
	{
		// メッシュの開放
		if(g_P_Bullet[nCntBullet].pD3DXMesh != NULL)
		{
			g_P_Bullet[nCntBullet].pD3DXMesh->Release();
			g_P_Bullet[nCntBullet].pD3DXMesh = NULL;
		}
		// マテリアルの開放
		if(g_P_Bullet[nCntBullet].pD3DXBuffMat != NULL)
		{
			g_P_Bullet[nCntBullet].pD3DXBuffMat->Release();
			g_P_Bullet[nCntBullet].pD3DXBuffMat = NULL;
		}
		g_P_Bullet[nCntBullet].bUse = false;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayerBullet( float fTimeSpeed)
{
	CAMERA *pCamera = GetCamera();

	for(int nCntBullet = 0; nCntBullet < MAX_PLAYER_BULLET; nCntBullet++)
	{
		if(g_P_Bullet[nCntBullet].bUse)
		{
			//ステージの中にいなかったら
			if(!HitCheckBall( g_P_Bullet[nCntBullet].pos, 2000.0f, D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 0.0f))
			{
				g_P_Bullet[nCntBullet].bUse = false;
			}

			//壁に当たったら
			D3DXVECTOR3 hitPos;
			if( HitCheckMeshWall( g_P_Bullet[nCntBullet].pos, g_P_Bullet[nCntBullet].pos + g_P_Bullet[nCntBullet].front, NULL, &hitPos))
			{
				g_P_Bullet[nCntBullet].bUse = false;
				//弾の再生
				hitPos += (-g_P_Bullet[nCntBullet].front) * 10.0f;
				SetItemBullet( hitPos);
			}
			else if( g_P_Bullet[nCntBullet].pos.y > STAGE_ROOF || g_P_Bullet[nCntBullet].pos.y < 0.0f) //天井と地面を超えるか
			{
				g_P_Bullet[nCntBullet].bUse = false;
				//弾の再生
				SetItemBullet( g_P_Bullet[nCntBullet].pos);				
			}
			else
			{
				//弾の移動
				g_P_Bullet[nCntBullet].pos += g_P_Bullet[nCntBullet].front * fTimeSpeed;
			}

			{//敵と弾の当たり判定
				ENEMY *pEnemy = GetEnemy();

				for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
				{
					if( pEnemy[nCntEnemy].bUse) 
					{
						if( HitCheckBall(g_P_Bullet[nCntBullet].pos, 1.0f, pEnemy[nCntEnemy].pos, 20.0f))
						{
							PlaySound(SOUND_LABEL_SE_EXPLOSION);
							pEnemy[nCntEnemy].bUse = false;
							ReleaseShadow(pEnemy[nCntEnemy].nIdxShadow);
							ChangeEnemyNum( -1);

							g_P_Bullet[nCntBullet].bUse = false;
							//弾の再生
							SetItemBullet(pEnemy[nCntEnemy].pos);

							//particle
							SetParticle(30, g_P_Bullet[nCntBullet].pos);							
						}
					}
				}	

			}
		

		}
	}



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntBullet = 0; nCntBullet < MAX_PLAYER_BULLET; nCntBullet++)
	{
		if(g_P_Bullet[nCntBullet].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_P_Bullet[nCntBullet].mtxWorld);
	
			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_P_Bullet[nCntBullet].scl.x, g_P_Bullet[nCntBullet].scl.y, g_P_Bullet[nCntBullet].scl.z);
			D3DXMatrixMultiply( &g_P_Bullet[nCntBullet].mtxWorld, &g_P_Bullet[nCntBullet].mtxWorld, &mtxScl);
	
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_P_Bullet[nCntBullet].rot.y, g_P_Bullet[nCntBullet].rot.x, g_P_Bullet[nCntBullet].rot.z);
			D3DXMatrixMultiply( &g_P_Bullet[nCntBullet].mtxWorld, &g_P_Bullet[nCntBullet].mtxWorld, &mtxRot);
	
			// 移動を反映
			D3DXMatrixTranslation( &mtxTranslate, g_P_Bullet[nCntBullet].pos.x, g_P_Bullet[nCntBullet].pos.y, g_P_Bullet[nCntBullet].pos.z);
			D3DXMatrixMultiply( &g_P_Bullet[nCntBullet].mtxWorld, &g_P_Bullet[nCntBullet].mtxWorld, &mtxTranslate);
	
			// ワールドマトリックスの設定
			pDevice->SetTransform( D3DTS_WORLD, &g_P_Bullet[nCntBullet].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
	
			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL *)g_P_Bullet[nCntBullet].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_P_Bullet[nCntBullet].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture( 0, g_P_Bullet[nCntBullet].pD3DTexture);
				// 描画
				g_P_Bullet[nCntBullet].pD3DXMesh->DrawSubset( nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial( &matDef);		
		}
	}


	

}
//=============================================================================
// 弾の設置
//=============================================================================
void SetPlayerBullet( D3DXVECTOR3 pos, D3DXVECTOR3 target)
{
	D3DXVECTOR3 front = target - pos;
	D3DXVec3Normalize( &front, &front);

	for(int nCntBullet = 0; nCntBullet < MAX_PLAYER_BULLET; nCntBullet++)
	{
		if(!g_P_Bullet[nCntBullet].bUse) 
		{
			//向きを変更
			g_P_Bullet[nCntBullet].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_P_Bullet[nCntBullet].rot = GetRotationAngle( pos, target, g_P_Bullet[nCntBullet].rot);

			//位置を設定
			g_P_Bullet[nCntBullet].pos = pos;
			g_P_Bullet[nCntBullet].front = front * BULLET_SPEED;
			g_P_Bullet[nCntBullet].bUse = true;
			break;
		}
	}	
}

//=============================================================================
// 弾データを取得
//=============================================================================
PLAYER_BULLET *GetPlayerBullet(void)
{
	return &g_P_Bullet[0];
}

