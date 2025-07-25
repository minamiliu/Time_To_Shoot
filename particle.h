/*******************************************************************************
*
* タイトル:		敵の血(パーティクル)処理
* プログラム名:	particle.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

#define MAX_DOT	(64)

//一枚のドットの構造体
typedef struct
{
	D3DXMATRIX mtxWorld;								// ワールドマトリックス

	D3DXVECTOR3 posDot;									// 一枚ドットの位置
	D3DXVECTOR3 rotDot;									// 一枚ドットの向き(回転)
	D3DXVECTOR3 sclDot;									// 一枚ドットの大きさ(スケール)
	D3DXVECTOR3 movDot;									// 一枚ドットの移動量

	float alpha;										//アルファ値
	int nIdxShadow;										//使用している影の番号
	float timer;										//フレイムを数える
	bool bUse;
}PARTICLE_DOT;

//一つのパーティクルの構造体
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffParticle;		// 頂点バッファインターフェースへのポインタ
	PARTICLE_DOT aPtcDot[MAX_DOT];						//ドット数
	bool bDraw;											//表示するか
	int nNeedCreateNum;									//生成する数
	int nNeedDeteNum;									//消えていく数
	int nPtcCreateRate;									//ドット生成率
	int nPtcCreateCounter;								//ドット生成のクールダウン
	D3DXVECTOR3 pos;									//パーティクルの位置
}PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle( float fTimeSpeed);
void DrawParticle(void);

//パーティクルの設置
void SetParticle(int needNum, D3DXVECTOR3 pos);

#endif
