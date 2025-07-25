/*******************************************************************************
*
* タイトル:		メッシュフィールド処理
* プログラム名:	meshfield.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshField(void);
//void UpdateMeshField(void);
void DrawMeshField(void);

#endif
