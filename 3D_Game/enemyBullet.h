/*******************************************************************************
*
* タイトル:		敵の弾の処理
* プログラム名:	enemyBullet.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_

#include "main.h"

#define MAX_ENEMY_BULLET	(16)

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
	D3DXVECTOR3 front;					//弾の向き
	int nJustShoot;						//撃ったばかりの弾の移動フレイム
} ENEMY_BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet( float fTimeSpeed);
void DrawEnemyBullet(void);

//弾の設置
void SetEnemyBullet( D3DXVECTOR3 pos, D3DXVECTOR3 target);
#endif
