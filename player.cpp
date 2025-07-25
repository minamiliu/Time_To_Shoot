/*******************************************************************************
*
* タイトル:		プレイヤー処理
* プログラム名:	player.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "player.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "shadow.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "game.h"
#include "gun.h"
#include "playerBullet.h"
#include "playerBulletNum.h"
#include "playerLife.h"
#include "fountainCircle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLAYER	"data/MODEL/player.x"					// 読み込むテクスチャファイル名

#define VALUE_MOVE		(3.0f)									// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 						// 回転量
#define DIVIDE_ROTATE	(3) 									// 回転量をいくつに分ける

#define PLAYER_POS_X	(-50)
#define PLAYER_POS_Y	(50)
#define PLAYER_POS_Z	(-150)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER g_player;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 位置・回転・スケールの初期設定
	g_player.pos = D3DXVECTOR3( PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z);
	g_player.posLSide = g_player.posRSide = g_player.pos;
	g_player.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	g_player.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_player.rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.jumpTime = 0;
	g_player.inAir = false;
	g_player.bAlive = true;
	g_player.bRun = false;
	g_player.nBullet = INI_NUM_LIFE;
	g_player.nLife = INI_NUM_BULLET;

	// モデルに関する変数の初期化							
	g_player.pD3DTexture = NULL;		// テクスチャへのポインタ
	g_player.pD3DXMesh = NULL;		// メッシュ情報へのポインタ
	g_player.pD3DXBuffMat = NULL;		// マテリアル情報へのポインタ
	g_player.nNumMat = 0;				// マテリアル情報の数

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLAYER,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
		pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
		NULL,						// 隣接性データを含むバッファへのポインタ
		&g_player.pD3DXBuffMat,		// マテリアルデータを含むバッファへのポインタ
		NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_player.nNumMat,			// D3DXMATERIAL構造体の数
		&g_player.pD3DXMesh			// ID3DXMeshインターフェイスへのポインタのアドレス
		)))
	{
		return E_FAIL;
	}		

	g_player.nIdxShadow = CreateShadow( g_player.pos, 50.0f, 50.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// テクスチャの開放

	// メッシュの開放
	if(g_player.pD3DXMesh != NULL)
	{
		g_player.pD3DXMesh->Release();
		g_player.pD3DXMesh = NULL;
	}
	// マテリアルの開放
	if(g_player.pD3DXBuffMat != NULL)
	{
		g_player.pD3DXBuffMat->Release();
		g_player.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *pCamera = GetCamera();
	GUN *pGun = GetGun();

	//弾を撃つ
	if((GetMouseLeftTrigger() || GetGamePadTrigger(0, BUTTON_R1)) && !pGun->bShot)
	{
		//弾あり
		if( g_player.nBullet > 0 && IsGunReady())
		{
			PlaySound(SOUND_LABEL_SE_PSHOT);

			SetPlayerBullet( pGun->pos, pCamera->posR);
		
			SetGameStep(STEP_SHOOT);

			SetGunShootAni();

			g_player.nBullet--;
			ChangeNumBullet( 0, g_player.nBullet);
		}
		else//弾なし
		{
			if(IsGunReady())
			{
				SetGunBulletOutAni(true);
				PlaySound(SOUND_LABEL_SE_BULLET_OUT);
			}
		}

	}
	if(GetMouseRightTrigger() && IsDebugMode())
	{
		g_player.nBullet = 3;
		ChangeNumBullet( 0, g_player.nBullet);
	}



	//JUMP
	if(GetKeyboardPress(DIK_SPACE) || GetGamePadTrigger( 0, BUTTON_A))
	{
		g_player.move.y = VALUE_MOVE * 1.4f;
		g_player.inAir = true;
	}
	if( g_player.inAir)
	{
		ChangeTimeSpeed( 0.5f);
		g_player.jumpTime++;
		g_player.pos.y = PLAYER_POS_Y + g_player.move.y * g_player.jumpTime - 0.5f * 0.2f * g_player.jumpTime * g_player.jumpTime;
	}
	if( g_player.pos.y < PLAYER_POS_Y)
	{
		g_player.inAir = false;
		g_player.jumpTime = 0;
	}

	//移動処理
	bool isKeyPressed = false;
	//斜め移動
	if( (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W)) ||(GetGamePadTrigger( 0, LSTICK_RIGHT) && GetGamePadTrigger( 0, LSTICK_UP)) ) //右上
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(45.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(45.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S)) ||(GetGamePadTrigger( 0, LSTICK_RIGHT) && GetGamePadTrigger( 0, LSTICK_DOWN)) ) //右下
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(135.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(135.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_A)  && GetKeyboardPress(DIK_W)) ||(GetGamePadTrigger( 0, LSTICK_LEFT) && GetGamePadTrigger( 0, LSTICK_UP)) ) //左上
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-45.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-45.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S)) ||(GetGamePadTrigger( 0, LSTICK_LEFT) && GetGamePadTrigger( 0, LSTICK_DOWN)) ) //左下
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-135.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-135.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_W) || GetGamePadTrigger( 0, LSTICK_UP))
	{
		//g_player.rot.y = pCamera->rot.y;
		g_player.rotTarget.y = pCamera->rot.y;
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_S) || GetGamePadTrigger( 0, LSTICK_DOWN))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(180.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(180.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_A) || GetGamePadTrigger( 0, LSTICK_LEFT))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-90.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-90.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_D) || GetGamePadTrigger( 0, LSTICK_RIGHT))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(90.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(90.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	if(isKeyPressed)
	{
		if(GetKeyboardPress(DIK_LSHIFT) || GetGamePadPress( 0, BUTTON_L1))
		{
			//移動慣性の初期化
			g_player.move.x = VALUE_MOVE * 1.5f;
			g_player.move.z = VALUE_MOVE * 1.5f;

			ChangeTimeSpeed( 1.5f);
		}
		else
		{
			//移動慣性の初期化
			g_player.move.x = VALUE_MOVE;
			g_player.move.z = VALUE_MOVE;

			ChangeTimeSpeed( 1.0f);
		}	

		//時計回り、または逆時計回りを決める
		g_player.rotAngle =  Get2VecRotAngle( g_player.rot, g_player.rotTarget, DIVIDE_ROTATE, VALUE_ROTATE);


		if(!g_player.bRun)
		{
			//走る音
			PlaySound(SOUND_LABEL_SE_RUN);
			SetGunShakeAni(true);
		}
		g_player.bRun = true;
	}
	else
	{
		if(g_player.bRun)
		{
			//走る音 一時停止
			StopSound(SOUND_LABEL_SE_RUN);
			SetGunShakeAni(false);
		}
		g_player.bRun = false;
	}


	//回転慣性
	g_player.rotAngle.y += (-g_player.rotAngle.y) * 0.001f;

	//モデル角度修正
	if(g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y = g_player.rot.y - D3DX_PI*2;
	}
	else if(g_player.rot.y < 0.0f - D3DX_PI)
	{
		g_player.rot.y = g_player.rot.y + D3DX_PI*2;
	}

	//次の回転位置に到着したら
	if(fabs(g_player.rot.y - g_player.rotTarget.y) < VALUE_ROTATE)
	{
		g_player.rot.y = g_player.rotTarget.y;
		g_player.rotAngle.y = 0;
	}
	//次の回転位置にまだ到着してない
	else
	{
		g_player.rot.y += g_player.rotAngle.y;
	}	
	


	//回転していない時
	if(g_player.rotAngle.y == 0)
	{
		D3DXVECTOR3 tNextPos = g_player.pos;
		tNextPos.x += g_player.move.x * sinf(g_player.rot.y);
		tNextPos.z += g_player.move.z * cosf(g_player.rot.y);

		//慣性処理
		g_player.move.x -= g_player.move.x * 0.25f;
		g_player.move.z -= g_player.move.z * 0.25f;

		D3DXVECTOR3 tSphere  = D3DXVECTOR3( 0, 0, 0);
		D3DXVECTOR3 front = tNextPos - g_player.pos;
		D3DXVec3Normalize( &tSphere, &front);
		tSphere *= 20.0f;


		//壁の法線
		D3DXVECTOR3 wall_nor = D3DXVECTOR3(0,0,0);

		//Side
		g_player.posLSide.x = g_player.pos.x +  15.0f * sinf(g_player.rotTarget.y+D3DXToRadian(-90.0f));
		g_player.posLSide.z = g_player.pos.z +  15.0f * cosf(g_player.rotTarget.y+D3DXToRadian(-90.0f));
		g_player.posLSide.y = g_player.pos.y;
											
		g_player.posRSide.x = g_player.pos.x +  15.0f * sinf(g_player.rotTarget.y+D3DXToRadian(90.0f));
		g_player.posRSide.z = g_player.pos.z +  15.0f * cosf(g_player.rotTarget.y+D3DXToRadian(90.0f));
		g_player.posLSide.y = g_player.pos.y;

		//壁に当たったら
		if( //HitCheckMeshWall( g_player.posRSide, g_player.posRSide + tSphere, &wall_nor, NULL) ||
			//HitCheckMeshWall( g_player.posLSide, g_player.posLSide + tSphere, &wall_nor, NULL) ||	
			HitCheckMeshWall( g_player.pos, g_player.pos + tSphere, &wall_nor, NULL)
			)
		{

			//壁ずり処理：移動ベクトルを修正
			GetWallScratchVector( &front, front, wall_nor);
				
			D3DXVec3Normalize( &tSphere, &front);
			tSphere *= 20.0f;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			if( !HitCheckMeshWall( g_player.pos, g_player.pos + tSphere, &wall_nor, NULL) &&
				!HitCheckMeshWall( g_player.posLSide, g_player.posLSide + tSphere, &wall_nor, NULL) &&
				!HitCheckMeshWall( g_player.posRSide, g_player.posRSide + tSphere, &wall_nor, NULL))
			{
				//移動処理
				g_player.pos += front;
			}

		}
		else
		{
			//移動処理
			g_player.pos += front;
		}

		//噴水との当たり判定
		FOUNTAIN_CIRCLE *pCircle = GetFountainCircle();
		if( pCircle->bUse && HitCheckXZ( g_player.pos, 20.0f, pCircle->pos, pCircle->fRadius))
		{
			g_player.pos -= front;	
		}

	}

	if( IsDebugMode())
	{
		//モデル上下移動
		if(GetKeyboardPress(DIK_PRIOR))
		{
			g_player.pos.y += VALUE_MOVE * 0.5f;
		}
		if(GetKeyboardPress(DIK_NEXT))
		{
			g_player.pos.y -= VALUE_MOVE * 0.5f;
		}
	
	}
	
	//shadow
	SetPositionShadow( g_player.nIdxShadow, g_player.pos);

	//PrintDebugProc("Playerの向き　：(X:%f, Y:%f, Z:%f)\n", g_player.rot.x, g_player.rot.y, g_player.rot.z);
	//PrintDebugProc("Playerの位置：(X:%f, Y:%f, Z:%f)\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	//PrintDebugProc("Leftの位置：(X:%f, Y:%f, Z:%f)\n", g_player.posLSide.x, g_player.posLSide.y, g_player.posLSide.z);
	//PrintDebugProc("Rightの位置：(X:%f, Y:%f, Z:%f)\n", g_player.posRSide.x, g_player.posRSide.y, g_player.posRSide.z);

	

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_player.mtxWorld);
	
	// スケールを反映
	D3DXMatrixScaling( &mtxScl, g_player.scl.x, g_player.scl.y, g_player.scl.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxScl);
	
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);
	
	// 移動を反映
	D3DXMatrixTranslation( &mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxTranslate);
	
	// ワールドマトリックスの設定
	pDevice->SetTransform( D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL *)g_player.pD3DXBuffMat->GetBufferPointer();
	for(int nCntMat = 0; nCntMat < (int)g_player.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// テクスチャの設定
		pDevice->SetTexture( 0, g_player.pD3DTexture);
		// 描画
		g_player.pD3DXMesh->DrawSubset( nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial( &matDef);
}

//=============================================================================
// モデルの取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

void SetPlayerDeath(void)
{
	//足音を消す
	g_player.bRun = false;
	StopSound(SOUND_LABEL_SE_RUN);

	g_player.bAlive = false;
	ChangeNumLife( 0, --g_player.nLife);

	CAMERA *pCamera = GetCamera();
	//カメラ回転
	float a = pCamera->rot.y + D3DXToRadian(90.0f);
	float x = sinf(a);
	float z = cosf(a);
	pCamera->vecU = D3DXVECTOR3( x, 0.0f, z);

	//地面に
	pCamera->posV.y = 30.0f;
	
	ReleaseShadow( g_player.nIdxShadow);

	SetGameStep(STEP_DIE);
}

//=============================================================================
// プレイヤーの設置
//=============================================================================
void SetPlayer( D3DXVECTOR3 pos, float rotY, int numBullet, int numLife)
{
	CAMERA *pCamera = GetCamera();

	g_player.pos = pos;
	g_player.rot.y = rotY;
	pCamera->rot.y = rotY;
	g_player.nBullet = numBullet;
	g_player.nLife = numLife;

	//UIの初期化
	ChangeNumBullet( 0, numBullet);
	ChangeNumLife( 0, numLife);
}

