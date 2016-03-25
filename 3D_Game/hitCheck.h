/*******************************************************************************
*
* タイトル:		当たり判定の処理
* プログラム名:	hitCheck.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#ifndef _HITCHECK_H_
#define _HITCHECH_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

// 内積
float DotProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr);

// 外積
D3DXVECTOR3 CrossProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr );

// ポリゴンと線分との当たり判定
bool HitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

// 壁ずりベクトル
D3DXVECTOR3* GetWallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 &front, D3DXVECTOR3 &normal);

// 交点を取得
D3DXVECTOR3 GetHitPos(void);

// 丸の当たり判定
bool HitCheckBall( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2);

// 平面の円の当たり判定
bool HitCheckXZ( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2);

// ２点の向きベクトルを取得
D3DXVECTOR3 GetRotationAngle( D3DXVECTOR3 myPos, D3DXVECTOR3 target, D3DXVECTOR3 myAngle);

// 回転角度を取得
D3DXVECTOR3 Get2VecRotAngle( D3DXVECTOR3 rot, D3DXVECTOR3 rotTarget, float divide, float value_rot);

// 視野内の判定
bool HitCheckViewArea( D3DXVECTOR3 myPos, D3DXVECTOR3 tarPos, D3DXVECTOR3 myRot, float myView);

// ２点距離を取得

float GetDistanceXZ( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
#endif
