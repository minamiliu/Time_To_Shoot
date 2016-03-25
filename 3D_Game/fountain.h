/*******************************************************************************
*
* タイトル:		噴水の水パーティクル処理
* プログラム名:	fountain.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;							// ワールドマトリックス

	D3DXVECTOR3 pos;							// 地面の位置
	D3DXVECTOR3 rot;							// 地面の向き(回転)
	D3DXVECTOR3 scl;							// 地面の大きさ(スケール)

	D3DXVECTOR3 mov;
	float alpha;
	int nIdxShadow;
	float volume;
	float timer;
	bool bUse;
}FOUNTAIN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFountain(void);
void UninitFountain(void);
void UpdateFountain(float fTimeSpeed);
void DrawFountain(void);

void SetFountain(int timer, D3DXVECTOR3 pos);

#endif
