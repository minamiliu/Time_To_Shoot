/*******************************************************************************
*
* タイトル:		プレイヤー処理
* プログラム名:	player.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER (1)
#define INI_NUM_LIFE (3)
#define INI_NUM_BULLET (3)

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
	D3DXVECTOR3 posLSide;				// モデルの左側の位置
	D3DXVECTOR3 posRSide;				// モデルの右側の位置
	D3DXVECTOR3 rot;					// モデルの向き(回転)
	D3DXVECTOR3 scl;					// モデルの大きさ(スケール)
	D3DXVECTOR3 move;					// モデルの移動量
	D3DXVECTOR3 rotAngle;				// モデルの向き(回転)
	D3DXVECTOR3 rotTarget;				// モデルの向き(回転)

	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	int nIdxShadow;						//使用している影の番号
	
	//jump
	bool inAir;							//ジャンプ中か
	int jumpTime;						//ジャンプ中のフレイム数

	//status
	bool bAlive;						//生きているか
	int nBullet;						//弾の数
	int nLife;							//ライフの数
	bool bRun;							//走っているか
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
//プレイヤーの死亡処理
void SetPlayerDeath(void);
//プレイヤーの設置
void SetPlayer( D3DXVECTOR3 pos, float rotY, int numBullet, int numLife);
#endif
