/*******************************************************************************
*
* タイトル:		ステージ管理
* プログラム名:	stageManager.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGE	(3)
#define STAGE_ROOF	(200)
#define STAGE_WALL_HEIGHT	(100)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

//ゲームの総ステージ
typedef enum
{
	STAGE0 = 0,
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE_MAX
}STAGE_MODE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitStageManager(bool bFirst, int playerLife);
void UpdateStageManager(float fTimeSpeed);
void DrawStageManager(void);
void UninitStageManager(void);

//ステージモードを取得
STAGE_MODE GetStageMode(void);
//ステージモードを設置
void SetStageMode(STAGE_MODE mode);
//次のステージへ
bool NextStage(void);
#endif