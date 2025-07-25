/*******************************************************************************
*
* タイトル:		ゲーム全体処理
* プログラム名:	game.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/


#include "game.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"
#include "message.h"
#include "sound.h"

#include "camera.h"
#include "light.h"
#include "time.h"
#include "clock.h"
#include "gun.h"
#include "gunSight.h"
#include "playerLife.h"
#include "enemyNum.h"

#include "stageManager.h"
#include "meshdome.h"
#include "meshwall.h"
#include "meshfield.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "playerBullet.h"
#include "playerBulletNum.h"
#include "enemy.h"
#include "enemyBullet.h"
#include "itemBullet.h"
#include "fountain.h"
#include "fountainCircle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/
GAME_STEP g_gameStep;
int g_nCounterFrame;
int g_nCounterShoot;
int g_nCounterEShoot;
float g_fTimeSpeed;
bool g_bNeedRseet;
bool g_bGameClear;


/*******************************************************************************
関数名:	void InitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの初期化処理
*******************************************************************************/
void InitGame(void)
{
	//ゲームモードの初期化
	SetGameStep(STEP_PLAY);

	//stageの初期化
	SetStageMode(STAGE0);

	// ライトの初期化
	InitLight();

	//dome
	InitMeshDome( D3DXVECTOR3( 0, 0, 0), D3DXVECTOR3( 0, 0, 0), 1500.0f, 32, 32);

	//UI
	InitTime();
	InitClock(D3DXVECTOR3(SCREEN_WIDTH/2 + 100, 70, 0));
	InitGunSight();
	InitNumLife();
	InitEnemyNum();
	
	//Model
	InitStageManager(true, INI_NUM_LIFE);
	//InitModel();

	//グローバル変数の初期化
	g_nCounterFrame = 0;
	g_nCounterShoot = 0;
	g_nCounterEShoot = 0;
	g_fTimeSpeed = 0.01f;
	g_bNeedRseet = false;
	g_bGameClear = false;
}

/*******************************************************************************
関数名:	void UpdateGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの更新関数
*******************************************************************************/
void UpdateGame(void)
{
	//初期値
	g_fTimeSpeed = 0.01f;

	GAME_STEP stepNow = GetGameStep();
	switch(stepNow)
	{
		case STEP_PLAY:

			UpdatePlayer();
			UpdateCamera();
			UpdateGun();
			
			UpdatePlayerBullet(g_fTimeSpeed);
			UpdateEnemy( g_fTimeSpeed);
			UpdateEnemyBullet( g_fTimeSpeed);

			//UpdateBillBoard();
			UpdateParticle( g_fTimeSpeed);
			UpdateStageManager( g_fTimeSpeed);

			//UpdateModel();
			UpdateGunSight();
			UpdateItemBullet();
			UpdateClock( g_fTimeSpeed);
			UpdateTime( g_fTimeSpeed);

			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PAUSE);
				SetMessage(MSG_PAUSE);
			}
			break;
		case STEP_SHOOT:
			
			ChangeTimeSpeed( 1.0f);
			
			UpdateEnemy( g_fTimeSpeed);
			//UpdatePlayer();
			UpdatePlayerBullet( g_fTimeSpeed);
			//UpdateEnemyBullet( g_fTimeSpeed);
			UpdateClock( g_fTimeSpeed);
			UpdateTime( g_fTimeSpeed);

			g_nCounterShoot++;
			if( g_nCounterShoot > 15)
			{
				g_nCounterShoot = 0;
				SetGameStep(STEP_PLAY);
			}
			break;
		case STEP_PAUSE:
			StopSound();
			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_DIE:
			ChangeTimeSpeed( 1.0f);

			UpdateEnemy( g_fTimeSpeed);
			UpdateEnemyBullet( g_fTimeSpeed);
			UpdatePlayerBullet( g_fTimeSpeed);
			UpdateParticle( g_fTimeSpeed);

			g_nCounterFrame++;
			if( g_nCounterFrame > 90)
			{
				g_nCounterFrame = 0;
				if( GetPlayer()->nLife <= 0)
				{
					FadeOutToNext(MODE_RESULT);
				}
				else
				{
					FadeOutToNext(STEP_RESET);
					SetGameStep(STEP_NOTHING);
				}
				
			}

			break;
		case STEP_RESET:
			InitStageManager( false, GetPlayer()->nLife);
			SetGameStep(STEP_PLAY);	
			break;
		case STEP_CLEAR:
			StopSound(SOUND_LABEL_SE_RUN);
			SetMessage(MSG_STAGECLEAR);

			g_nCounterFrame++;
			if( g_nCounterFrame > 90)
			{
				g_nCounterFrame = 0;

				switch(GetStageMode())
				{
				case STAGE0:
					FadeOutToNext(STAGE1);
					break;
				case STAGE1:
					FadeOutToNext(STAGE2);
					break;
				case STAGE2:
					FadeOutToNext(STAGE3);
					break;
				case STAGE3:
					FadeOutToNext(MODE_RESULT);
					g_bGameClear = true;
					break;
				}
				
				SetGameStep(STEP_NOTHING);
			}
			break;
		case STEP_NOTHING:
			//just let time go, and do nothing
			break;
	}	

	//UI update
	UpdateNumBullet();

	//Debug update
	UpdateMeshDome();

	if(GetKeyboardTrigger(DIK_RETURN) )
	{	
		FadeOutToNext(MODE_RESULT);
	}
	if(GetKeyboardTrigger(DIK_F3))
	{	
		PlaySound( SOUND_LABEL_SE_CLEAR);
		SetGameStep(STEP_CLEAR);
	}
	if(GetKeyboardTrigger(DIK_F2))
	{
		PlaySound(SOUND_LABEL_SE_SWITCH);
		SwitchDebug();
	}
}

/*******************************************************************************
関数名:	void DrawGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの描画関数
*******************************************************************************/
void DrawGame(void)
{
	//カメラの設置
	SetCamera();

	//ステージの描画処理
	DrawStageManager();
	DrawMeshDome();
	
	//DrawModel();
	DrawParticle();
	DrawEnemy();
	DrawEnemyBullet();
	DrawItemBullet();

	if( IsDebugMode()) DrawPlayer();
	
	DrawPlayerBullet();
	DrawGun();
	DrawShadow();
	//DrawBillBoard();	

	DrawTime();
	DrawGunSight();
	DrawNumBullet();
	DrawNumLife();
	DrawClock();
	DrawEnemyNum();
}
/*******************************************************************************
関数名:	void UninitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの開放関数
*******************************************************************************/
void UninitGame(void)
{
	// カメラの終了処理
	UninitCamera();
	
	// ライトの終了処理
	UninitLight();

	// ステージの終了処理
	UninitStageManager();
	UninitMeshDome();

	//UninitModel();
	UninitPlayer();
	UninitPlayerBullet();
	UninitGun();
	UninitEnemy();
	UninitEnemyBullet();
	UninitShadow();

	//UninitTime();
	UninitGunSight();
	UninitNumBullet();
	UninitNumLife();
	UninitItemBullet();

	//UninitBillBoard();
	UninitFountain();
	UninitParticle();
	UninitClock();
	UninitEnemyNum();
}

void SetGameStep(GAME_STEP step)
{
	g_gameStep = step;
}

GAME_STEP GetGameStep(void)
{
	return g_gameStep;
}

void ChangeTimeSpeed( float fTimeSpeed)
{
	if( fTimeSpeed > g_fTimeSpeed)
	{
		g_fTimeSpeed = fTimeSpeed;
	}
}

bool IsGameClear(void)
{
	return g_bGameClear;
}






