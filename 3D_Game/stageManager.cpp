/*******************************************************************************
*
* タイトル:		ステージ管理
* プログラム名:	stageManager.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/


#include "stageManager.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"
#include "message.h"
#include "sound.h"
#include "camera.h"

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
#include "gun.h"
#include "enemyNum.h"
#include "fountain.h"
#include "fountainCircle.h"
#include "time.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_POS_Y	(70)
#define ITEM_POS_Y	(100)
#define PLAYER_POS_Y	(50)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void InitStage(STAGE_MODE mode);

void InitStageWall0();
void InitStageWall1();
void InitStageWall2();
void InitStageWall3();

void InitStageEnemy0();
void InitStageEnemy1();
void InitStageEnemy2();
void InitStageEnemy3();

void InitStageItem0();
void InitStageItem1();
void InitStageItem2();
void InitStageItem3();
/*******************************************************************************
* グローバル変数
*******************************************************************************/
STAGE_MODE g_stageMode;



//=============================================================================
// 初期化処理
//=============================================================================
void InitStageManager(bool bFirst, int playerLife)
{
	InitCamera();
	InitShadow();
	InitPlayer();
	InitNumBullet();
	InitEnemy();
	InitEnemyBullet();
	InitPlayerBullet();
	InitItemBullet();
	InitParticle();
	InitGun();
	SetMessage(MSG_ENEMY_LEFT);

	switch(g_stageMode)
	{
	case STAGE0:
		//噴水
		InitFountainCircle();
		InitFountain();
		SetFountain( -1, D3DXVECTOR3( -700, 0, 200));
		PlaySound(SOUND_LABEL_SE_WATER);

		SetPlayer( D3DXVECTOR3( -700.0f, PLAYER_POS_Y, -400.0f), D3DXToRadian(0.0f), 0, playerLife);
		if(bFirst) InitStageWall0();
		InitStageEnemy0();
		InitStageItem0();
		break;
	case STAGE1:
		if(bFirst) InitTime();
		SetPlayer( D3DXVECTOR3( 400.0f, PLAYER_POS_Y, -800.0f), D3DXToRadian(-90.0f), 0, playerLife);
		if(bFirst) InitStageWall1();
		InitStageEnemy1();
		InitStageItem1();
		break;
	case STAGE2:
		SetPlayer( D3DXVECTOR3( -50.0f, PLAYER_POS_Y, -150.0f), D3DXToRadian(0.0f), 0, playerLife);
		if(bFirst) InitStageWall2();
		InitStageEnemy2();
		InitStageItem2();
		break;
	case STAGE3:
		SetPlayer( D3DXVECTOR3( 0.0f, PLAYER_POS_Y, 0.0f), D3DXToRadian(0.0f), 3, playerLife);
		if(bFirst) InitStageWall3();
		InitStageEnemy3();
		InitStageItem3();
		break;
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateStageManager(float fTimeSpeed)
{
	switch(g_stageMode)
	{
	case STAGE0:
		//噴水
		UpdateFountain( fTimeSpeed);
		break;
	case STAGE1:

		break;
	case STAGE2:

		break;
	case STAGE3:

		break;
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawStageManager(void)
{
	switch(g_stageMode)
	{
	case STAGE0:
		DrawFountainCircle();
		break;
	case STAGE1:

		break;
	case STAGE2:

		break;
	case STAGE3:

		break;
	}
	DrawMeshField();
	DrawMeshWall();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStageManager(void)
{
	UninitMeshField();
	UninitMeshWall();
	UninitFountainCircle();
	StopSound(SOUND_LABEL_SE_WATER);
}

//=============================================================================
// ゲームモードを取得
//=============================================================================
STAGE_MODE GetStageMode(void)
{
	return g_stageMode;
}
//=============================================================================
// ゲームモードの設置
//=============================================================================
void SetStageMode(STAGE_MODE mode)
{
	g_stageMode = mode;
}
//=============================================================================
// ステージ変更
//=============================================================================
bool NextStage(void)
{
	g_stageMode = STAGE_MODE(g_stageMode + 1);
	if( g_stageMode >= STAGE_MAX)
	{
		g_stageMode = STAGE_MAX;
		return false;
	}
	else
	{
		UninitStageManager();
		InitStageManager( true, INI_NUM_LIFE);
		return true;
	}
}
//=============================================================================
// 以下はステージデータ
//=============================================================================
void InitStageWall0()
{
	const float fWallSize = 100.0f;

	// 地面の初期化処理
	InitMeshField( 20, 20, fWallSize, fWallSize);

	//壁の初期化処理
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, 10 * fWallSize), D3DXVECTOR3( 0, 0, 0), 20, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, -10 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 20, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -10 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 20, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 10 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 20, 2, fWallSize, fWallSize); //右

	//1
	InitMeshWall( D3DXVECTOR3( -7 * fWallSize, STAGE_WALL_HEIGHT, 4 * fWallSize), D3DXVECTOR3( 0, 0, 0), 6, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( -7 * fWallSize, STAGE_WALL_HEIGHT, -6 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 6, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -4 * fWallSize, STAGE_WALL_HEIGHT, 2 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 4, 2, fWallSize, fWallSize); //右
	InitMeshWall( D3DXVECTOR3( -4 * fWallSize, STAGE_WALL_HEIGHT, -4 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 4, 2, fWallSize, fWallSize); //右

	//2
	InitMeshWall( D3DXVECTOR3( -3 * fWallSize, STAGE_WALL_HEIGHT, 0 * fWallSize), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( -3 * fWallSize, STAGE_WALL_HEIGHT, -2 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize); //下

	//3
	InitMeshWall( D3DXVECTOR3( -1 * fWallSize, STAGE_WALL_HEIGHT, -8 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -2 * fWallSize, STAGE_WALL_HEIGHT, 3 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 6, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( -2 * fWallSize, STAGE_WALL_HEIGHT, -5 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 6, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, 0 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 16, 2, fWallSize, fWallSize); //右

	//4
	InitMeshWall( D3DXVECTOR3( -4 * fWallSize, STAGE_WALL_HEIGHT, 6 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 4, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -6 * fWallSize, STAGE_WALL_HEIGHT, 8 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 2, fWallSize, fWallSize); //左

	//5
	InitMeshWall( D3DXVECTOR3( 2 * fWallSize, STAGE_WALL_HEIGHT, 8 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 4, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( 4 * fWallSize, STAGE_WALL_HEIGHT, 6 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 6 * fWallSize, STAGE_WALL_HEIGHT, 7 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 6, 2, fWallSize, fWallSize); //右

	//6
	InitMeshWall( D3DXVECTOR3( 3 * fWallSize, STAGE_WALL_HEIGHT, 4 * fWallSize), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 7 * fWallSize, STAGE_WALL_HEIGHT, 4 * fWallSize), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 3 * fWallSize, STAGE_WALL_HEIGHT, 0 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( 5 * fWallSize, STAGE_WALL_HEIGHT, 2 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( 2 * fWallSize, STAGE_WALL_HEIGHT, 2 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 6 * fWallSize, STAGE_WALL_HEIGHT, 0 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 4 * fWallSize, STAGE_WALL_HEIGHT, 1 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize); //右
	InitMeshWall( D3DXVECTOR3( 8 * fWallSize, STAGE_WALL_HEIGHT, 1 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 6, 2, fWallSize, fWallSize); //右

	//7
	InitMeshWall( D3DXVECTOR3( 4 * fWallSize, STAGE_WALL_HEIGHT, -2 * fWallSize), D3DXVECTOR3( 0, 0, 0), 4, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 9 * fWallSize, STAGE_WALL_HEIGHT, -2 * fWallSize), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 6 * fWallSize, STAGE_WALL_HEIGHT, -10 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 8, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( 2 * fWallSize, STAGE_WALL_HEIGHT, -6 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 8, 2, fWallSize, fWallSize); //左

	//roof
	//InitMeshWall( D3DXVECTOR3( 0, 200, 0), D3DXVECTOR3( D3DXToRadian(-90.0f), 0, 0), 20, 20, fWallSize, fWallSize);

}
void InitStageWall1()
{
	const float fWallSize = 100.0f;

	// 地面の初期化処理
	InitMeshField( 20, 20, fWallSize, fWallSize);

	//壁の初期化処理
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, 10 * fWallSize), D3DXVECTOR3( 0, 0, 0), 20, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, -10 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 20, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -10 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 20, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 10 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 20, 2, fWallSize, fWallSize); //右

	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, 6 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 12, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, -6 * fWallSize), D3DXVECTOR3( 0, 0, 0), 12, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -6 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 12, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 6 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 12, 2, fWallSize, fWallSize); //右

	InitMeshWall( D3DXVECTOR3( -1 * fWallSize, STAGE_WALL_HEIGHT/2, -8 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 4, 1, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 1 * fWallSize, STAGE_WALL_HEIGHT/2, -8 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 1, fWallSize, fWallSize); //右

	//roof
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, -8 * fWallSize), D3DXVECTOR3( D3DXToRadian(90.0f), 0, 0), 2, 4, fWallSize, fWallSize);
}

void InitStageWall2()
{
	const float fWallSize = 100.0f;

	// 地面の初期化処理
	InitMeshField( 20, 20, fWallSize, fWallSize);

	//壁の初期化処理
	InitMeshWall( D3DXVECTOR3( 0, STAGE_WALL_HEIGHT, 1000), D3DXVECTOR3( 0, 0, 0), 20, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 0, STAGE_WALL_HEIGHT, -1000), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 20, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -1000, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 20, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 1000, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 20, 2, fWallSize, fWallSize); //右

	//1
	InitMeshWall( D3DXVECTOR3( -600, STAGE_WALL_HEIGHT, 800), D3DXVECTOR3( 0, 0, 0), 8, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -200, STAGE_WALL_HEIGHT, 900), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 2, 2, fWallSize, fWallSize);

	//2
	InitMeshWall( D3DXVECTOR3( -500, STAGE_WALL_HEIGHT, 600), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -500, STAGE_WALL_HEIGHT, 400), D3DXVECTOR3( 0, 0, 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -800, STAGE_WALL_HEIGHT, 500), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -200, STAGE_WALL_HEIGHT, 500), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 2, 2, fWallSize, fWallSize);

	//3
	InitMeshWall( D3DXVECTOR3( -900, STAGE_WALL_HEIGHT, 200), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -900, STAGE_WALL_HEIGHT, -400), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -800, STAGE_WALL_HEIGHT, -100), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 6, 2, fWallSize, fWallSize);

	//4
	InitMeshWall( D3DXVECTOR3( -400, STAGE_WALL_HEIGHT, 200), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 4, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -400, STAGE_WALL_HEIGHT, -400), D3DXVECTOR3( 0, 0, 0), 4, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -600, STAGE_WALL_HEIGHT, -100), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -200, STAGE_WALL_HEIGHT, -100), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 6, 2, fWallSize, fWallSize);

	//5
	InitMeshWall( D3DXVECTOR3( -400, STAGE_WALL_HEIGHT, -600), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 8, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -400, STAGE_WALL_HEIGHT, -800), D3DXVECTOR3( 0, 0, 0), 8, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -800, STAGE_WALL_HEIGHT, -700), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 0, STAGE_WALL_HEIGHT, -700), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 2, 2, fWallSize, fWallSize);

	//6
	InitMeshWall( D3DXVECTOR3( 100, STAGE_WALL_HEIGHT, 400), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 100, STAGE_WALL_HEIGHT, 200), D3DXVECTOR3( 0, 0, 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( -200, STAGE_WALL_HEIGHT, 300), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 400, STAGE_WALL_HEIGHT, 300), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 2, 2, fWallSize, fWallSize);

	//7
	InitMeshWall( D3DXVECTOR3( 300, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 100, STAGE_WALL_HEIGHT, -200), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 2, 2, fWallSize, fWallSize);

	InitMeshWall( D3DXVECTOR3( 300, STAGE_WALL_HEIGHT, -400), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 100, STAGE_WALL_HEIGHT, -600), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize);

	InitMeshWall( D3DXVECTOR3( 200, STAGE_WALL_HEIGHT, -100), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 0, STAGE_WALL_HEIGHT, -400), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 4, 2, fWallSize, fWallSize);
	
	InitMeshWall( D3DXVECTOR3( 400, STAGE_WALL_HEIGHT, -200), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 4, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 200, STAGE_WALL_HEIGHT, -500), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 2, 2, fWallSize, fWallSize);

	//8
	InitMeshWall( D3DXVECTOR3( 400, STAGE_WALL_HEIGHT, 800), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 8, 2, fWallSize, fWallSize);

	InitMeshWall( D3DXVECTOR3( 300, STAGE_WALL_HEIGHT, 600), D3DXVECTOR3( 0, 0, 0), 6, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 700, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, 0, 0), 2, 2, fWallSize, fWallSize);

	InitMeshWall( D3DXVECTOR3( 0, STAGE_WALL_HEIGHT, 700), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 600, STAGE_WALL_HEIGHT, 300), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 6, 2, fWallSize, fWallSize);

	InitMeshWall( D3DXVECTOR3( 800, STAGE_WALL_HEIGHT, 400), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 8, 2, fWallSize, fWallSize);

	//9
	InitMeshWall( D3DXVECTOR3( 800, STAGE_WALL_HEIGHT, -200), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 4, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 800, STAGE_WALL_HEIGHT, -600), D3DXVECTOR3( 0, 0, 0), 4, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 600, STAGE_WALL_HEIGHT, -400), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 4, 2, fWallSize, fWallSize);

	//10
	InitMeshWall( D3DXVECTOR3( 600, STAGE_WALL_HEIGHT, -800), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 8, 2, fWallSize, fWallSize);
	InitMeshWall( D3DXVECTOR3( 200, STAGE_WALL_HEIGHT, -900), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 2, 2, fWallSize, fWallSize);

	//roof
	//InitMeshWall( D3DXVECTOR3( 0, 200, 0), D3DXVECTOR3( D3DXToRadian(-90.0f), 0, 0), 20, 20, fWallSize, fWallSize);

}

void InitStageWall3()
{
	float fWallSize = 100.0f;
	// 地面の初期化処理
	InitMeshField( 10, 10, fWallSize, fWallSize);

	//壁の初期化処理
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, 5 * fWallSize), D3DXVECTOR3( 0, 0, 0), 10, 2, fWallSize, fWallSize); //上
	InitMeshWall( D3DXVECTOR3( 0 * fWallSize, STAGE_WALL_HEIGHT, -5 * fWallSize), D3DXVECTOR3( 0, D3DXToRadian(180.0f), 0), 10, 2, fWallSize, fWallSize); //下
	InitMeshWall( D3DXVECTOR3( -5 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(-90.0f), 0), 10, 2, fWallSize, fWallSize); //左
	InitMeshWall( D3DXVECTOR3( 5 * fWallSize, STAGE_WALL_HEIGHT, 0), D3DXVECTOR3( 0, D3DXToRadian(90.0f), 0), 10, 2, fWallSize, fWallSize); //右
}

void InitStageEnemy0()
{
	const int ENEMY_NUM = 5;
	SetEnemyNum( ENEMY_NUM);

	D3DXVECTOR3 tInitPos[ENEMY_NUM] = 
	{
		//1 - 5
		D3DXVECTOR3( -100, ENEMY_POS_Y, -700),
		D3DXVECTOR3( -500, ENEMY_POS_Y, 900),
		D3DXVECTOR3( 300, ENEMY_POS_Y, 300),
		D3DXVECTOR3( 300, ENEMY_POS_Y, -600),
		D3DXVECTOR3( 900, ENEMY_POS_Y, -600),
	};	
	float tInitRotY[ENEMY_NUM] = 
	{
		//1 - 5
		0.0f,
		D3DXToRadian(180.0f),
		D3DXToRadian(90.0f),
		0.0f,
		D3DXToRadian(180.0f),
	};	

	for(int cnt = 0; cnt < ENEMY_NUM; cnt++)
	{
		switch(cnt)
		{
		default:
			SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_WHITE);
		}
		
	}
}
void InitStageEnemy1()
{
	const int ENEMY_NUM = 6;
	SetEnemyNum( ENEMY_NUM);

	D3DXVECTOR3 tInitPos[ENEMY_NUM] = 
	{
		//1 - 5
		D3DXVECTOR3( 800, ENEMY_POS_Y, 0),
		D3DXVECTOR3( 200, ENEMY_POS_Y, 700),
		D3DXVECTOR3( -200, ENEMY_POS_Y, 900),
		D3DXVECTOR3( -900, ENEMY_POS_Y, 400),
		D3DXVECTOR3( -700, ENEMY_POS_Y, 0),

		//6
		D3DXVECTOR3( -900, ENEMY_POS_Y, -400),
	};	
	float tInitRotY[ENEMY_NUM] = 
	{
		//1 - 5
		D3DXToRadian(180.0f),
		D3DXToRadian(90.0f),
		D3DXToRadian(90.0f),
		0.0f,
		0.0f,

		//6
		0.0f,
	};	

	for(int cnt = 0; cnt < ENEMY_NUM; cnt++)
	{
		SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_BLUE);
	}
}
void InitStageEnemy2()
{
	const int ENEMY_NUM = 10;
	SetEnemyNum( ENEMY_NUM);

	D3DXVECTOR3 tInitPos[ENEMY_NUM] = 
	{
		//1 - 5
		D3DXVECTOR3(-50+ -700, ENEMY_POS_Y, 0),
		D3DXVECTOR3( 500, ENEMY_POS_Y, 300),
		D3DXVECTOR3( 700, ENEMY_POS_Y, -100),
		D3DXVECTOR3( 900, ENEMY_POS_Y, 500),
		D3DXVECTOR3( 700, ENEMY_POS_Y,-50+ 900),

		//6 - 10
		D3DXVECTOR3( -100, ENEMY_POS_Y, 700),
		D3DXVECTOR3( -900, ENEMY_POS_Y, 500),
		D3DXVECTOR3( -900, ENEMY_POS_Y, -700),
		D3DXVECTOR3( 100, ENEMY_POS_Y, -700),
		D3DXVECTOR3( -300, ENEMY_POS_Y,-50+ -900),


	};	
	float tInitRotY[ENEMY_NUM] = 
	{
		//1 - 5
		D3DXToRadian(0.0f),
		0.0f,
		D3DXToRadian(-90.0f),
		0.0f,
		D3DXToRadian(90.0f),
		//6 - 10
		0.0f,
		0.0f,
		0.0f,
		D3DXToRadian(90.0f),
		D3DXToRadian(-90.0f),

	};	

	for(int cnt = 0; cnt < ENEMY_NUM; cnt++)
	{
		switch(cnt)
		{
		case 2:
		case 9:
			SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_BLUE);
			break;
		case 3:
		case 5:
		case 7:
			SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_RED);
			break;
		default:
			SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_WHITE);
		}
	}
}
void InitStageEnemy3()
{
	const int ENEMY_NUM = 8;
	SetEnemyNum( ENEMY_NUM);

	D3DXVECTOR3 tInitPos[ENEMY_NUM] = 
	{
		//1 - 4
		D3DXVECTOR3( 0, ENEMY_POS_Y, 200),
		D3DXVECTOR3( -200, ENEMY_POS_Y, 0),
		D3DXVECTOR3( 0, ENEMY_POS_Y, -200),
		D3DXVECTOR3( 200, ENEMY_POS_Y, 0),


		//5 - 8
		D3DXVECTOR3( -200, ENEMY_POS_Y, 200),
		D3DXVECTOR3( -200, ENEMY_POS_Y, -200),
		D3DXVECTOR3( 200, ENEMY_POS_Y, -200),
		D3DXVECTOR3( 200, ENEMY_POS_Y, 200),

	};	
	float tInitRotY[ENEMY_NUM] = 
	{
		//1 - 4
		0.0f,
		D3DXToRadian(-90.0f),
		D3DXToRadian(180.0f),
		D3DXToRadian(90.0f),


		//5 - 8
		D3DXToRadian( -45.0f),
		D3DXToRadian( -135.0f),
		D3DXToRadian( 135.0f),
		D3DXToRadian( 45.0f),
	};	

	for(int cnt = 0; cnt < ENEMY_NUM; cnt++)
	{
		SetEnemy( tInitPos[cnt], tInitRotY[cnt], ENEMY_RED);
	}
}
void InitStageItem0()
{
	const int ITEM_NUM = 3;
	D3DXVECTOR3 tInitPos[ITEM_NUM] = 
	{
		//1 - 3
		D3DXVECTOR3( -300, ITEM_POS_Y, -100),
		D3DXVECTOR3( 200, ITEM_POS_Y, 900),
		D3DXVECTOR3( 300, ITEM_POS_Y, 100),

	};	
	for(int cnt = 0; cnt < ITEM_NUM; cnt++)
	{
		SetItemBullet( tInitPos[cnt]);
	}
}
void InitStageItem1()
{
	const int ITEM_NUM = 3;
	D3DXVECTOR3 tInitPos[ITEM_NUM] = 
	{
		//1 - 3
		D3DXVECTOR3( -400, ITEM_POS_Y, -750),
		D3DXVECTOR3( -400, ITEM_POS_Y, -800),
		D3DXVECTOR3( -400, ITEM_POS_Y, -850),

	};	
	for(int cnt = 0; cnt < ITEM_NUM; cnt++)
	{
		SetItemBullet( tInitPos[cnt]);
	}
}
void InitStageItem2()
{
	const int ITEM_NUM = 3;
	D3DXVECTOR3 tInitPos[ITEM_NUM] = 
	{
		//1 - 3
		D3DXVECTOR3( 0, ITEM_POS_Y, 0),
		D3DXVECTOR3( 900, ITEM_POS_Y, -700),
		D3DXVECTOR3( -300, ITEM_POS_Y, 300),

	};	
	for(int cnt = 0; cnt < ITEM_NUM; cnt++)
	{
		SetItemBullet( tInitPos[cnt]);
	}
}
void InitStageItem3(){}

