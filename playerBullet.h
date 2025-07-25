/*******************************************************************************
*
* タイトル:		プレイヤーの弾の処理
* プログラム名:	playerBullet.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _PLAYER_BULLET_H_
#define _PLAYER_BULLET_H_

#include "main.h"

#define MAX_PLAYER_BULLET	(16)

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture;	// テクスチャへのポインタ
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

	int nIdxShadow;						//使用している影の番号
	bool bUse;
	D3DXVECTOR3 front;					//弾の移動ベクトル
} PLAYER_BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayerBullet(void);
void UninitPlayerBullet(void);
void UpdatePlayerBullet( float fTimeSpeed);
void DrawPlayerBullet(void);

void SetPlayerBullet( D3DXVECTOR3 pos, D3DXVECTOR3 target);
PLAYER_BULLET *GetPlayerBullet(void);
#endif
