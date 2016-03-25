/*******************************************************************************
*
* タイトル:		サウンド処理
* プログラム名:	sound.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// BGM0
	SOUND_LABEL_BGM_RANK,		// BGM1
	SOUND_LABEL_SE_ESHOT,		// 敵弾発射音
	SOUND_LABEL_SE_PSHOT,		// プレイヤー弾発射音
	SOUND_LABEL_SE_RELOAD,		// RELOAD
	SOUND_LABEL_SE_BULLET_OUT,	// 弾切れ
	SOUND_LABEL_SE_CLEAR,		// STAGE CLEAR
	SOUND_LABEL_SE_WATER,		// 噴水
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_HURT,		// HURT
	SOUND_LABEL_SE_RUN,			// RUN
	SOUND_LABEL_SE_SWITCH,		// SWITCH
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

//音の音量更新
void UpdateSoundVolume(SOUND_LABEL label, float percent);
#endif
