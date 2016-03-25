/*******************************************************************************
*
* タイトル:		銃のモデル処理
* プログラム名:	gun.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _GUN_H_
#define _GUN_H_

#include "main.h"

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

	//キーフレイム
	D3DXVECTOR3 posChild;				//子座標
	D3DXVECTOR3 posChildTarget;			//子座標の目標
	D3DXVECTOR3 rotChild;				//子座標の向き
	D3DXVECTOR3 rotChildAngle;			//子座標の毎回の回転量
	D3DXVECTOR3 rotChildTarget;			//子座標の向きの目標

	D3DXMATRIX mtxWorld;				// ワールドマトリックス

	bool bShot;							//弾を撃ったか
	bool bShake;						//移動しているか
	bool bBulletOut;					//弾切れになったか
	int nShootFrame;					//弾を撃つアニメ必要なフレイム数
	int nCheckFrame;					//弾切れのチェックフレイム数
	int nKeyFrame;						//弾切れのキーフレイム数
} GUN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGun(void);
void UninitGun(void);
void UpdateGun(void);
void DrawGun(void);

GUN *GetGun(void);
void SetGunShootAni(void);					//弾を撃つアニメ設置
void SetGunShakeAni(bool bShake);			//移動のアニメ設置
void SetGunBulletOutAni(bool bBulletOut);	//弾切れのアニメ設置
bool IsGunReady(void);						//銃のアニメ終わったか
#endif
