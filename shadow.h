/*******************************************************************************
*
* タイトル:		丸影処理
* プログラム名:	shadow.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
//void UpdateShadow(void);
void DrawShadow(void);

//影の生成
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
//影の開放
void ReleaseShadow(int nIdxShadow);
//影の座標設置
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

//影の色を変える
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);

#endif
