/*******************************************************************************
* タイトル:		フィールド処理
* プログラム名:	field.cpp
* 作成者:		TH教務部　寄光
* 作成日:		2015/10/20
********************************************************************************
* 更新履歴:		- 2015/10/20	 寄光
*					- V1.00		Initial Version
*******************************************************************************/
/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#ifndef _ENEMYBOARD_H_
#define _ENEMYBOARD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#define MAX_ENEMYBOARD	(32)

typedef struct
{
	D3DXMATRIX mtxWorld;												// ワールドマトリックス

	D3DXVECTOR3 pos;													// 地面の位置
	D3DXVECTOR3 rot;													// 地面の向き(回転)
	D3DXVECTOR3 scl;													// 地面の大きさ(スケール)
	bool bUse;
}ENEMYBOARD;

HRESULT InitEnemyBoard(void);
void UninitEnemyBoard(void);
void UpdateEnemyBoard(void);
void DrawEnemyBoard(void);
bool HitCheckEnemyBoard( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, int nIdxBoard);

void SetVertexEnemyBoard(int nIdxBoard, float fSizeX, float fSizeZ);
int CreateEnemyBoard(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseEnemyBoard(int nIdxBoard);
void SetPositionEnemyBoard(int nIdxBoard, D3DXVECTOR3 pos);

#endif
