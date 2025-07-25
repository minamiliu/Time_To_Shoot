/*******************************************************************************
*
* タイトル:		噴水のモデルの表示処理
* プログラム名:	fountainCircle.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _FOUNTAIN_CIRCLE_H_
#define _FOUNTAIN_CIRCLE_H_

#include "main.h"

#define MAX_D3D_MAT (5)

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture[MAX_D3D_MAT];	// テクスチャへのポインタ
	LPD3DXMESH  pD3DXMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER  pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD	nNumMat;					// マテリアル情報の数

	D3DXVECTOR3 pos;					// モデルの位置
	D3DXVECTOR3 rot;					// モデルの向き(回転)
	D3DXVECTOR3 scl;					// モデルの大きさ(スケール)
	D3DXVECTOR3 move;					// モデルの移動量
	D3DXVECTOR3 rotAngle;				// モデルの向き(回転)
	D3DXVECTOR3 rotTarget;				// モデルの向き(回転)

	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	float fRadius;						//半径
	int nIdxShadow;						//使用している影の番号
	bool bUse;
}FOUNTAIN_CIRCLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFountainCircle(void);
void UninitFountainCircle(void);
void UpdateFountainCircle(void);
void DrawFountainCircle(void);

FOUNTAIN_CIRCLE *GetFountainCircle(void);

#endif
