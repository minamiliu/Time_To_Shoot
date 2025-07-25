/*******************************************************************************
*
* タイトル:		カメラ処理
* プログラム名:	camera.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
********************************************************************************

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X				(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y				(100.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z				(-100.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X				(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y				(0.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z				(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE				(D3DXToRadian(60.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z				(1.f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z				(2500.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA		(2.0f)											// カメラの移動量
#define	VALUE_ROTATE_CAMERA		(D3DX_PI * 0.01f)								// カメラの回転量
#define	VALUE_ADJUST_DISTANCE	(2.0f)											// 視点と注視点の距離調整量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA g_camera;		//カメラ情報
bool g_cameraMode;		//TPSとFPSモードの切り替え

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	float fVecX,fVecZ;

	// 視点初期化
	g_camera.posV = D3DXVECTOR3( CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	// 注視点初期化
	g_camera.posR = D3DXVECTOR3( CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	// 上方向ベクトル初期化
	g_camera.vecU = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	// カメラ向き（回転角）の初期化
	g_camera.rot = D3DXVECTOR3( (D3DXToRadian(22.0f)), 0.0f, 0.0f);
	// 視点と注視点の距離を求める
	fVecX = g_camera.posV.x - g_camera.posR.x;						// X方向（視点ー注視点）
	fVecZ = g_camera.posV.z - g_camera.posR.z;						// Z方向（視点ー注視点）
	// 視点と注視点の距離
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	g_cameraMode = 0;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	{//カメラ追従
		PLAYER *pPlayer = GetPlayer();
		
		//注視点
		g_camera.rot.y += GetMouseAxisX() * 0.0025f;
		g_camera.rot.x += GetMouseAxisY() * 0.0025f;

		if( GetMouseAxisX() != 0 || GetMouseAxisY() != 0)
		{
			ChangeTimeSpeed( 0.5f);
		}

		g_camera.rot.y += GetGamePadRightAxisX(0) * 0.0001f;
		g_camera.rot.x += GetGamePadRightAxisY(0) * 0.0001f;

		if( GetGamePadRightAxisX(0) != 0 || GetGamePadRightAxisY(0) != 0)
		{
			ChangeTimeSpeed( 0.5f);
		}

		//注視点角度修正
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = g_camera.rot.y - D3DX_PI*2;
		}
		else if(g_camera.rot.y < 0.0f - D3DX_PI)
		{
			g_camera.rot.y = g_camera.rot.y + D3DX_PI*2;
		}
		if(g_camera.rot.x >= 1.0f)
		{
			g_camera.rot.x = 1.0f;
		}
		else if(g_camera.rot.x <= -1.0f)
		{
			g_camera.rot.x = -1.0f;
		}

		//視点
		switch( g_cameraMode)
		{
		case 0:
			g_camera.posV = pPlayer->pos;
			break;
		case 1:
			g_camera.posV = pPlayer->pos;
			g_camera.posV.x += 150 * sinf(g_camera.rot.y+D3DXToRadian(180.0f));
			g_camera.posV.z += 150 * cosf(g_camera.rot.y+D3DXToRadian(180.0f));
			break;
		}

		//高さ
		g_camera.posV.y += 80;

		//水平
		g_camera.posR.x = g_camera.posV.x + g_camera.fDistance * sinf(g_camera.rot.y);
		g_camera.posR.z = g_camera.posV.z + g_camera.fDistance * cosf(g_camera.rot.y);
		//垂直
		g_camera.posR.y = g_camera.posV.y - g_camera.fDistance * sinf(g_camera.rot.x);
	}

	if(GetKeyboardTrigger(DIK_F1) && IsDebugMode())
	{
		g_cameraMode = 1 - g_cameraMode;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);
	
	
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH( &g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);
	
	// ビューマトリックスの設定
	pDevice->SetTransform( D3DTS_VIEW, &g_camera.mtxView);
	
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity( &g_camera.mtxProjection);
	
	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH( &g_camera.mtxProjection, VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);
	
	// プロジェクションマトリックスの設定(透視変換の設定)
	pDevice->SetTransform( D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}

