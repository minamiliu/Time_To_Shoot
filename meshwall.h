/*******************************************************************************
*
* タイトル:		メッシュウォール処理
* プログラム名:	meshwall.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
//void UpdateMeshWall(void);
void DrawMeshWall(void);

//壁との当たり判定
bool HitCheckMeshWall( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, D3DXVECTOR3 *wall_nor, D3DXVECTOR3 *HitPoint);

#endif
