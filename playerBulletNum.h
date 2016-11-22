/*******************************************************************************
*
* タイトル:		テクスチャ プレイヤーの弾の数表示処理
* プログラム名:	playerBulletNum.h
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


#ifndef _NUM_BULLET_H_
#define _NUM_BULLET_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/


/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 size;	//サイズ
	D3DCOLOR col;		//カラー
	int num;			//数
	bool bDraw;			//描画するか
	int nCntFlash;		//点滅フレイム
}NUM_BULLET;

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitNumBullet();
void DrawNumBullet(void);		
void UninitNumBullet(void);
void UpdateNumBullet(void);

//弾の数を変更
void ChangeNumBullet(int nPlayerIdx, int value);	

#endif