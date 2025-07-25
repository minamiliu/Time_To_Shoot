/*******************************************************************************
*
* タイトル:		敵の処理
* プログラム名:	enemy.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "stageManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY	(30)							// 最大数

//*****************************************************************************
// カメラの構造体
//*****************************************************************************

//敵の種類
typedef enum
{
	ENEMY_WHITE,
	ENEMY_BLUE,
	ENEMY_RED,
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

//モデル情報
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture;	// テクスチャへのポインタ
	LPD3DXMESH  pD3DXMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER  pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD	nNumMat;					// マテリアル情報の数
	char filePath[MAX_PATH];
} ENEMY_MODEL;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;					// モデルの位置
	D3DXVECTOR3 posLSide;
	D3DXVECTOR3 posRSide;
	D3DXVECTOR3 rot;					// モデルの向き(回転)
	D3DXVECTOR3 scl;					// モデルの大きさ(スケール)
	D3DXVECTOR3 move;					// モデルの移動量
	D3DXVECTOR3 rotAngle;				// モデルの向き(回転)
	D3DXVECTOR3 rotTarget;				// モデルの向き(回転)

	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	int nIdxShadow;						//使用している影の番号
	bool bUse;							
	bool bShot;							//弾を撃ったか
	bool bEscaping;						//逃げる状態
	float nCoolDown;					//弾のクールダウン
	ENEMY_TYPE type;					//敵の種類

	//see
	bool bSeePlayer;					//プレイヤーを発見
	int nSeePFrame;						//プレイヤーを発見したフレイム数

	//Patrol
	bool bPatrol;						//パトロールか
	D3DXVECTOR3 posPatrol;				//パトロール中の座標
	D3DXVECTOR3 rotPatrol;				//パトロール中の向き
} ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(float fTimeSpeed);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
//敵の設置
void SetEnemy( D3DXVECTOR3 pos, float rotY, ENEMY_TYPE type);
#endif
