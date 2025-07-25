/*******************************************************************************
*
* タイトル:		メッシュドーム処理
* プログラム名:	meshdome.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nNumBlockH, int nNumBlockV);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif
