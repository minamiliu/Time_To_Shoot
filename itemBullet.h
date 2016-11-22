/*******************************************************************************
*
* タイトル:		アイテム(弾)の処理
* プログラム名:	itemBullet.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

#ifndef _ITEM_BULLET_H_
#define _ITEM_BULLET_H_

#include "main.h"

#define MAX_ITEM_BULLET (3)

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


	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	int nIdxShadow;						//使用している影の番号
	bool bUse;
}ITEM_BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItemBullet(void);
void UninitItemBullet(void);
void UpdateItemBullet(void);
void DrawItemBullet(void);

//アイテム弾の設置
void SetItemBullet( D3DXVECTOR3 pos);
#endif
