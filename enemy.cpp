/*******************************************************************************
*
* タイトル:		敵の処理
* プログラム名:	enemy.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "enemy.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "particle.h"
#include "player.h"
#include "enemyBullet.h"
#include "game.h"
#include "fade.h"


#include "message.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_ENEMY_WHITE	"data/MODEL/enemy.x"					// 読み込むテクスチャファイル名
#define MODEL_ENEMY_BLUE	"data/MODEL/enemy_blue.x"				// 読み込むテクスチャファイル名
#define MODEL_ENEMY_RED		"data/MODEL/enemy_red.x"				// 読み込むテクスチャファイル名

#define VALUE_MOVE		(1.0f)									// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// 回転量
#define DIVIDE_ROTATE	(10) 									// 回転量をいくつに分ける

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(70)
#define MODEL_POS_Z	(0)

#define MODEL_SPHERE (20)
#define COOLDOWN	(90.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY g_enemy[MAX_ENEMY];
ENEMY_MODEL g_modelData[ENEMY_TYPE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 位置・回転・スケールの初期設定
		g_enemy[nCntEnemy].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].scl = D3DXVECTOR3( 2.0f, 2.0f, 2.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_enemy[nCntEnemy].rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].nCoolDown = -1;
		g_enemy[nCntEnemy].bUse = false;
		g_enemy[nCntEnemy].bShot = false;
		g_enemy[nCntEnemy].bSeePlayer = false;
		g_enemy[nCntEnemy].nSeePFrame = 0;
		g_enemy[nCntEnemy].bPatrol = true;
		g_enemy[nCntEnemy].posPatrol = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotPatrol = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].bEscaping = false;
		g_enemy[nCntEnemy].type = ENEMY_WHITE;

	}

	//モデルに関する変数の初期化
	strcpy( g_modelData[ENEMY_WHITE].filePath, MODEL_ENEMY_WHITE);
	strcpy( g_modelData[ENEMY_BLUE].filePath, MODEL_ENEMY_BLUE);
	strcpy( g_modelData[ENEMY_RED].filePath, MODEL_ENEMY_RED);

	//モデルに関する変数の初期化
	for(int nCntType = 0; nCntType < ENEMY_TYPE_MAX; nCntType++)
	{
		g_modelData[nCntType].pD3DTexture = NULL;
		g_modelData[nCntType].pD3DXMesh = NULL;
		g_modelData[nCntType].pD3DXBuffMat = NULL;
		g_modelData[nCntType].nNumMat = 0;

		// Xファイルの読み込み
		if(FAILED(D3DXLoadMeshFromX(
			g_modelData[nCntType].filePath,			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
			pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
			NULL,									// 隣接性データを含むバッファへのポインタ
			&g_modelData[nCntType].pD3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ
			NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_modelData[nCntType].nNumMat,			// D3DXMATERIAL構造体の数
			&g_modelData[nCntType].pD3DXMesh		// ID3DXMeshインターフェイスへのポインタのアドレス
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
void UninitEnemy(void)
{
	for(int nCntType = 0; nCntType < ENEMY_TYPE_MAX; nCntType++)
	{
		// メッシュの開放
		if(g_modelData[nCntType].pD3DXMesh != NULL)
		{
			g_modelData[nCntType].pD3DXMesh->Release();
			g_modelData[nCntType].pD3DXMesh = NULL;
		}
		// マテリアルの開放
		if(g_modelData[nCntType].pD3DXBuffMat != NULL)
		{
			g_modelData[nCntType].pD3DXBuffMat->Release();
			g_modelData[nCntType].pD3DXBuffMat = NULL;
		}
		//テクスチャの開放
		if(g_modelData[nCntType].pD3DTexture != NULL)
		{
			g_modelData[nCntType].pD3DTexture->Release();
			g_modelData[nCntType].pD3DTexture = NULL;
		}
	}


	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//影と判定の開放
		ReleaseShadow(g_enemy[nCntEnemy].nIdxShadow);

		g_enemy[nCntEnemy].bUse = false;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(float fTimeSpeed)
{
	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();
	int nCountAlive = 0;

	//PrintDebugProc("enemy mark %d\n", g_enemy[0].nSeePFrame);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			nCountAlive++;

			{//プレイヤーに気づく判断処理
				if( !HitCheckMeshWall( g_enemy[nCntEnemy].pos, pCamera->posV, NULL, NULL) && pPlayer->bAlive && //壁に遮られたか 
					//HitCheckBall( g_enemy[nCntEnemy].pos, 1000.0f, pCamera->posV, 20.0f) && //一定の距離以内
					HitCheckViewArea( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot, D3DXToRadian( 90.0f) ) && //後ろには見えない
					!IsDebugMode()) //デバッグモードのときに、当たり判定はなし
				{
					if( g_enemy[nCntEnemy].nSeePFrame <= 1000) g_enemy[nCntEnemy].nSeePFrame++;
					if( g_enemy[nCntEnemy].nSeePFrame >= 50)
					{
						g_enemy[nCntEnemy].bSeePlayer = true;

						//プレイヤーを見た時の情報を保存
						if( g_enemy[nCntEnemy].bPatrol)
						{
							//びっくりマークを設定
							SetFlashMessage(MSG_EXCLAMATION);

							g_enemy[nCntEnemy].bPatrol = false;
						
							g_enemy[nCntEnemy].posPatrol = g_enemy[nCntEnemy].pos;
							g_enemy[nCntEnemy].rotPatrol = g_enemy[nCntEnemy].rotTarget;
						}
					}
				}
				else if( HitCheckBall( g_enemy[nCntEnemy].pos, 120.0f, pCamera->posV, 20.0f) ) //一定の距離以内
				{
					if( g_enemy[nCntEnemy].nSeePFrame <= 65535) g_enemy[nCntEnemy].nSeePFrame++;
					if( g_enemy[nCntEnemy].nSeePFrame >= 50)
					{
						g_enemy[nCntEnemy].bSeePlayer = true;

						//プレイヤーを見た時の情報を保存
						if( g_enemy[nCntEnemy].bPatrol)
						{
							g_enemy[nCntEnemy].bPatrol = false;
						
							g_enemy[nCntEnemy].posPatrol = g_enemy[nCntEnemy].pos;
							g_enemy[nCntEnemy].rotPatrol = g_enemy[nCntEnemy].rotTarget;
						}
					}					
				}
				else
				{
					if( g_enemy[nCntEnemy].nSeePFrame > 0)
					{
						g_enemy[nCntEnemy].nSeePFrame--;
					}
					g_enemy[nCntEnemy].bSeePlayer = false;
				}
			}

			//CoolDown
			if( g_enemy[nCntEnemy].nCoolDown > 0)
			{
				g_enemy[nCntEnemy].nCoolDown -= 1 * fTimeSpeed;
				if(g_enemy[nCntEnemy].nCoolDown <= 0)
				{
					g_enemy[nCntEnemy].nCoolDown = -1;
					g_enemy[nCntEnemy].bShot = false;
				}
			}

			//プレイヤーに気づいたら、行動処理
			if( g_enemy[nCntEnemy].bSeePlayer || g_enemy[nCntEnemy].nSeePFrame >= 50)
			{
				if( !g_enemy[nCntEnemy].bEscaping)
				{
					//向き
					g_enemy[nCntEnemy].rotTarget = GetRotationAngle( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot);		
				}

				//弾を発射
				if(!g_enemy[nCntEnemy].bShot)
				{
					//向き
					g_enemy[nCntEnemy].rot = GetRotationAngle( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot);

					if(g_enemy[nCntEnemy].type == ENEMY_RED)
					{
						for(int cnt = 0; cnt < 3; cnt++)
						{
							SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV + D3DXVECTOR3( rand() % 60 - 30, rand() % 40 - 20, rand() % 60 - 30));
						}

						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);
					}
					else if(g_enemy[nCntEnemy].type == ENEMY_BLUE)
					{
						SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV);
						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN/2 + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);					
					}
					else
					{
						SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV);
						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);					
					}

				}

			}
			else
			{
				if( g_enemy[nCntEnemy].bPatrol)
				{
					//nothing
				}
				else
				{
					//パトロールに戻る
					g_enemy[nCntEnemy].rotTarget = GetRotationAngle( g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].posPatrol, g_enemy[nCntEnemy].rot);
					D3DXVECTOR3 vec = g_enemy[nCntEnemy].pos - g_enemy[nCntEnemy].posPatrol;
					if( D3DXVec3Length( &vec) < VALUE_MOVE)
					{
						g_enemy[nCntEnemy].bPatrol = true;
						g_enemy[nCntEnemy].pos = g_enemy[nCntEnemy].posPatrol;
						g_enemy[nCntEnemy].rotTarget = g_enemy[nCntEnemy].rotPatrol;
					}
				}
			}

			//弾キレ、逃げる処理
			//if( g_enemy[nCntEnemy].bShot && !g_enemy[nCntEnemy].bEscaping)
			//{
			//	float movRot = rand() % 180 - 90;
			//	g_enemy[nCntEnemy].rotTarget.y = g_enemy[nCntEnemy].rot.y + D3DXToRadian(movRot);
			//	if( g_enemy[nCntEnemy].rotTarget.y > D3DX_PI)
			//	{
			//		g_enemy[nCntEnemy].rotTarget.y -= D3DX_PI * 2.0f;
			//	}
			//	g_enemy[nCntEnemy].bEscaping = true;
			//}

			bool bMove = true;
			//プレイヤーに近づき過ぎないように
			D3DXVECTOR3 vec = g_enemy[nCntEnemy].pos - pPlayer->pos;
			if( (D3DXVec3Length(&vec) < 200.0f) && g_enemy[nCntEnemy].bSeePlayer)
			{
				bMove = false;
			}

			{//移動処理


				if( bMove)
				{
					//移動慣性の初期化
					g_enemy[nCntEnemy].move.x = VALUE_MOVE;
					g_enemy[nCntEnemy].move.z = VALUE_MOVE;	

					if(g_enemy[nCntEnemy].type == ENEMY_BLUE)
					{
						g_enemy[nCntEnemy].move.x = 0;
						g_enemy[nCntEnemy].move.z = 0;
					}
				}


				//時計回り、または逆時計回りを決める
				g_enemy[nCntEnemy].rotAngle =  Get2VecRotAngle( g_enemy[nCntEnemy].rot, g_enemy[nCntEnemy].rotTarget, DIVIDE_ROTATE, -1);


				//回転慣性
				g_enemy[nCntEnemy].rotAngle += (-g_enemy[nCntEnemy].rotAngle) * 0.001f;

				//モデルY軸角度修正
				if(g_enemy[nCntEnemy].rot.y > D3DX_PI)
				{
					g_enemy[nCntEnemy].rot.y = g_enemy[nCntEnemy].rot.y - D3DX_PI*2;
				}
				else if(g_enemy[nCntEnemy].rot.y < 0.0f - D3DX_PI)
				{
					g_enemy[nCntEnemy].rot.y = g_enemy[nCntEnemy].rot.y + D3DX_PI*2;
				}

				//次の回転位置に到着したら
				if( fabs(g_enemy[nCntEnemy].rot.y - g_enemy[nCntEnemy].rotTarget.y) < VALUE_ROTATE &&
					fabs(g_enemy[nCntEnemy].rot.x - g_enemy[nCntEnemy].rotTarget.x) < VALUE_ROTATE &&
					fabs(g_enemy[nCntEnemy].rot.z - g_enemy[nCntEnemy].rotTarget.z) < VALUE_ROTATE
					)
				{
					g_enemy[nCntEnemy].rot = g_enemy[nCntEnemy].rotTarget;
					g_enemy[nCntEnemy].rotAngle = D3DXVECTOR3( 0, 0, 0);
				}
				else //次の回転位置にまだ到着してない
				{
					g_enemy[nCntEnemy].rot += g_enemy[nCntEnemy].rotAngle * fTimeSpeed;
				}	

				//回転していない時
				if(g_enemy[nCntEnemy].rotAngle == D3DXVECTOR3( 0, 0, 0))
				{
					//慣性処理
					g_enemy[nCntEnemy].move.x -= g_enemy[nCntEnemy].move.x * 0.05f;
					g_enemy[nCntEnemy].move.z -= g_enemy[nCntEnemy].move.z * 0.05f;

					D3DXVECTOR3 tNextPos = g_enemy[nCntEnemy].pos;
					tNextPos.x += g_enemy[nCntEnemy].move.x * sinf(g_enemy[nCntEnemy].rot.y);
					tNextPos.z += g_enemy[nCntEnemy].move.z * cosf(g_enemy[nCntEnemy].rot.y);

					D3DXVECTOR3 tSphere  = D3DXVECTOR3( 0, 0, 0);
					D3DXVECTOR3 front = tNextPos - g_enemy[nCntEnemy].pos;
					D3DXVec3Normalize( &tSphere, &front);
					tSphere *= MODEL_SPHERE;

					//壁の法線ベクトルを取得変数の初期化
					D3DXVECTOR3 wall_nor = D3DXVECTOR3(0,0,0);

					//Side
					g_enemy[nCntEnemy].posLSide.x = g_enemy[nCntEnemy].pos.x + 15.0f * sinf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(-90.0f));
					g_enemy[nCntEnemy].posLSide.z = g_enemy[nCntEnemy].pos.z + 15.0f * cosf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(-90.0f));

					g_enemy[nCntEnemy].posRSide.x = g_enemy[nCntEnemy].pos.x + 15.0f * sinf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(90.0f));
					g_enemy[nCntEnemy].posRSide.z = g_enemy[nCntEnemy].pos.z + 15.0f * cosf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(90.0f));

					//壁に当たったら
					if( HitCheckMeshWall( g_enemy[nCntEnemy].posLSide, g_enemy[nCntEnemy].posLSide + tSphere, &wall_nor, NULL) ||
						HitCheckMeshWall( g_enemy[nCntEnemy].posRSide, g_enemy[nCntEnemy].posRSide + tSphere, &wall_nor, NULL) )
					{

						if( g_enemy[nCntEnemy].bSeePlayer || !g_enemy[nCntEnemy].bPatrol)
						{
							//壁ずり処理：移動ベクトルを修正
							GetWallScratchVector( &front, front, wall_nor);
				
							D3DXVec3Normalize( &tSphere, &front);
							tSphere *= MODEL_SPHERE;

							if( !HitCheckMeshWall( g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].pos + tSphere, &wall_nor, NULL) )
							{
								//移動処理
								g_enemy[nCntEnemy].pos += front;
							}
						}
						
						if( g_enemy[nCntEnemy].bPatrol)
						{
							//180反転
							g_enemy[nCntEnemy].rotTarget.y = g_enemy[nCntEnemy].rot.y + D3DXToRadian(180.0f);
							if( g_enemy[nCntEnemy].rotTarget.y > D3DX_PI)
							{
								g_enemy[nCntEnemy].rotTarget.y -= D3DX_PI * 2.0f;
							}	
						}



					}
					else
					{
						//移動処理
						g_enemy[nCntEnemy].pos += front * fTimeSpeed;
					}		

				}			
			}

			//shadow
			SetPositionShadow( g_enemy[nCntEnemy].nIdxShadow, g_enemy[nCntEnemy].pos);

		}
	}

	if( nCountAlive == 0)
	{
		PlaySound( SOUND_LABEL_SE_CLEAR);
		SetGameStep(STEP_CLEAR);
	}

	//PrintDebugProc("\n敵の位置：(X:%f, Y:%f, Z:%f)\n", g_enemy[0].pos.x, g_enemy[0].pos.y, g_enemy[0].pos.z);
	//PrintDebugProc("敵の向き　：(X:%f, Y:%f, Z:%f)\n", g_enemy[0].rot.x, D3DXToDegree(g_enemy[0].rot.y), g_enemy[0].rot.z);
	//if( g_enemy[0].bSeePlayer) PrintDebugProc("see Player\n");
	//if( g_enemy[0].bPatrol)PrintDebugProc("patral\n");

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_enemy[nCntEnemy].mtxWorld);
	
			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_enemy[nCntEnemy].scl.x, g_enemy[nCntEnemy].scl.y, g_enemy[nCntEnemy].scl.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxScl);
	
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_enemy[nCntEnemy].rot.y, g_enemy[nCntEnemy].rot.x, g_enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxRot);
	
			// 移動を反映
			D3DXMatrixTranslation( &mtxTranslate, g_enemy[nCntEnemy].pos.x, g_enemy[nCntEnemy].pos.y, g_enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxTranslate);
	
			// ワールドマトリックスの設定
			pDevice->SetTransform( D3DTS_WORLD, &g_enemy[nCntEnemy].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
	
			
			ENEMY_TYPE type = g_enemy[nCntEnemy].type;
			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL *)g_modelData[type].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_modelData[type].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture( 0, g_modelData[type].pD3DTexture);
				// 描画
				g_modelData[type].pD3DXMesh->DrawSubset( nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial( &matDef);		
		}
	}

}

//=============================================================================
// モデルの取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_enemy[0];
}

void SetEnemy( D3DXVECTOR3 pos, float rotY, ENEMY_TYPE type)
{
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if( !g_enemy[nCntEnemy].bUse)
		{
			g_enemy[nCntEnemy].pos = pos;
			g_enemy[nCntEnemy].rotTarget.y = rotY;
			g_enemy[nCntEnemy].rot.y = rotY;
			g_enemy[nCntEnemy].type = type;
			g_enemy[nCntEnemy].bUse = true;
			g_enemy[nCntEnemy].nIdxShadow = CreateShadow( g_enemy[nCntEnemy].pos, 50.0f, 50.0f);
			break;
		}
	}
}




