/*******************************************************************************
*
* タイトル:		ゲーム全体処理
* プログラム名:	game.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

//ゲームの状態
typedef enum
{
	STEP_PLAY = 0,
	STEP_SHOOT,
	STEP_PAUSE,
	STEP_DIE,
	STEP_RESET,
	STEP_CLEAR,
	STEP_NOTHING,
	STEP_MAX
}GAME_STEP;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UninitGame(void);

GAME_STEP GetGameStep(void);
void SetGameStep(GAME_STEP);
//時間流れのスピード調整
void ChangeTimeSpeed( float fTimeSpeed);
//ゲームクリアの判定
bool IsGameClear(void);
#endif