/*******************************************************************************
*
* タイトル:		銃のモデル処理
* プログラム名:	gun.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "gun.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "hitCheck.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLANE		"data/MODEL/gun.x"					// 読み込むテクスチャファイル名

#define VALUE_MOVE		(5.0f)									// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// 回転量
#define DIVIDE_ROTATE	(30) 									// 回転量をいくつに分ける

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(30)
#define MODEL_POS_Z	(-50)

#define MAX_KEYFRAME	(4)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GUN g_gun;


D3DXVECTOR3 g_BulletOutAnime[MAX_KEYFRAME]={ 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(-30.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(0.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(-60.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 位置・回転・スケールの初期設定
	g_gun.pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
	g_gun.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_gun.scl = D3DXVECTOR3( 2.0f, 2.0f, 2.0f);
	//g_gun.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_gun.bShot = false;
	g_gun.bShake = false;
	g_gun.bBulletOut = false;

	// モデルに関する変数の初期化							
	g_gun.pD3DTexture = NULL;		// テクスチャへのポインタ
	g_gun.pD3DXMesh = NULL;		// メッシュ情報へのポインタ
	g_gun.pD3DXBuffMat = NULL;		// マテリアル情報へのポインタ
	g_gun.nNumMat = 0;				// マテリアル情報の数

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLANE,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,			// メッシュの作成オプションを指定
		pDevice,					// IDirect3DDevice9インターフェイスへのポインタ
		NULL,						// 隣接性データを含むバッファへのポインタ
		&g_gun.pD3DXBuffMat,		// マテリアルデータを含むバッファへのポインタ
		NULL,						// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_gun.nNumMat,			// D3DXMATERIAL構造体の数
		&g_gun.pD3DXMesh			// ID3DXMeshインターフェイスへのポインタのアドレス
		)))
	{
		return E_FAIL;
	}		

														
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGun(void)
{
	// テクスチャの開放

	// メッシュの開放
	if(g_gun.pD3DXMesh != NULL)
	{
		g_gun.pD3DXMesh->Release();
		g_gun.pD3DXMesh = NULL;
	}
	// マテリアルの開放
	if(g_gun.pD3DXBuffMat != NULL)
	{
		g_gun.pD3DXBuffMat->Release();
		g_gun.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGun(void)
{
	CAMERA *pCamera = GetCamera();

	//銃はカメラの右奥の何度のところに置く
	float angle = pCamera->rot.y + D3DXToRadian(15.0f);
	if( angle > D3DX_PI) angle -= D3DX_PI * 2.0f;

	//銃の位置を設定
	g_gun.pos.x = pCamera->posV.x + 2 * sinf(angle);
	g_gun.pos.z = pCamera->posV.z + 2 * cosf(angle);
	g_gun.pos.y = pCamera->posV.y - 1.68f;

	//見下ろす時、銃の位置
	g_gun.pos.y += sinf( D3DXToRadian(22.0f) - pCamera->rot.x ) * 2.0f ;
	if( D3DXToRadian(22.0f) - pCamera->rot.x < 0)
	{
		g_gun.pos.x = pCamera->posV.x + ( 2 +  (D3DXToRadian(22.0f) - pCamera->rot.x)*0.5f ) * sinf(angle);
		g_gun.pos.z = pCamera->posV.z + ( 2 +  (D3DXToRadian(22.0f) - pCamera->rot.x)*0.5f ) * cosf(angle);				
	}

	//銃の向き
	g_gun.rot = GetRotationAngle( g_gun.pos, pCamera->posR, g_gun.rot);	

	//反動
	if( g_gun.bShot)
	{
		g_gun.nShootFrame--;
		if( g_gun.nShootFrame == 0) //一発の発射完了 
		{
			g_gun.bShot = false;
		}
		else if( g_gun.nShootFrame > 50) //発射した瞬間、銃の反動
		{
			float angle = pCamera->rot.y + D3DXToRadian(170.0f); 
			if( angle > D3DX_PI) angle -= D3DX_PI * 2.0f;
			g_gun.posChild.x = 0.5f * sinf(angle);
			g_gun.posChild.z = 0.5f * cosf(angle);
		}
		else //銃を元に位置に戻す
		{
			g_gun.posChild.x -= g_gun.posChild.x * 0.05f;
			g_gun.posChild.z -= g_gun.posChild.z * 0.05f;
		}
		
		//親子の座標を足して、銃の位置を設定
		g_gun.pos.x += g_gun.posChild.x;
		g_gun.pos.z += g_gun.posChild.z;	
	}

	//揺れる
	const float shakeMove = 0.01f;
	if( g_gun.bShake)
	{

		float diff = g_gun.posChildTarget.y - g_gun.posChild.y;
		if( fabs(diff) <= shakeMove)
		{
			//上下反転
			g_gun.posChildTarget.y *= -1;
		}
		else if( diff > 0.0f)
		{
			g_gun.posChild.y += shakeMove;
		}
		else
		{
			g_gun.posChild.y -= shakeMove;
		}
		
		
		//親子の座標を足して、銃の位置を設定
		g_gun.pos += g_gun.posChild;		
	}

	//弾切れ
	if( g_gun.bBulletOut)
	{
		if(g_gun.nCheckFrame > 0){
			g_gun.nCheckFrame--;
		}
		else
		{
			g_gun.nCheckFrame = -1;

			//次の回転位置に到着したら
			if( fabs(g_gun.rotChild.x - g_gun.rotChildTarget.x) < VALUE_ROTATE &&
				fabs(g_gun.rotChild.y - g_gun.rotChildTarget.y) < VALUE_ROTATE &&
				fabs(g_gun.rotChild.z - g_gun.rotChildTarget.z) < VALUE_ROTATE
				)
			{

				g_gun.nKeyFrame++;
				if( g_gun.nKeyFrame < MAX_KEYFRAME)
				{
					g_gun.rotChildAngle =  Get2VecRotAngle( g_gun.rotChildTarget, g_BulletOutAnime[g_gun.nKeyFrame], DIVIDE_ROTATE, -1);
					g_gun.rotChildTarget = g_BulletOutAnime[g_gun.nKeyFrame];	
				} 
			}
			else
			{
				g_gun.rotChild += g_gun.rotChildAngle;
				g_gun.rotChildAngle += (-g_gun.rotChildAngle) * 0.001f;
			}
			
		
		}
		//親子の座標を足して、銃の位置を設定
		g_gun.rot += g_gun.rotChild;

		if( g_gun.nKeyFrame == MAX_KEYFRAME)
		{
			g_gun.bBulletOut = false;
		}
	}

	//PrintDebugProc("\nCameraの位置：(X:%f, Y:%f, Z:%f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	//PrintDebugProc("\n銃の位置：(X:%f, Y:%f, Z:%f)\n", g_gun.pos.x, g_gun.pos.y, g_gun.pos.z);
	//PrintDebugProc("銃の向き　：(X:%f, Y:%f, Z:%f)\n", g_gun.rot.x, g_gun.rot.y, g_gun.rot.z);
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_gun.mtxWorld);
	
	// スケールを反映
	D3DXMatrixScaling( &mtxScl, g_gun.scl.x, g_gun.scl.y, g_gun.scl.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxScl);
	
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_gun.rot.y, g_gun.rot.x, g_gun.rot.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxRot);
	
	// 移動を反映
	D3DXMatrixTranslation( &mtxTranslate, g_gun.pos.x, g_gun.pos.y, g_gun.pos.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxTranslate);
	
	// ワールドマトリックスの設定
	pDevice->SetTransform( D3DTS_WORLD, &g_gun.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL *)g_gun.pD3DXBuffMat->GetBufferPointer();
	for(int nCntMat = 0; nCntMat < (int)g_gun.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// テクスチャの設定
		pDevice->SetTexture( 0, g_gun.pD3DTexture);
		// 描画
		g_gun.pD3DXMesh->DrawSubset( nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial( &matDef);
}

//=============================================================================
// モデルの取得
//=============================================================================
GUN *GetGun(void)
{
	return &g_gun;
}

/*******************************************************************************
* 銃の反動アニメ
*******************************************************************************/
void SetGunShootAni(void)
{
	g_gun.bShot = true;
	g_gun.nShootFrame = 60;
	g_gun.posChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}
/*******************************************************************************
* 銃の移動アニメ
*******************************************************************************/
void SetGunShakeAni(bool bShake)
{
	const float shakeMax = 0.05f;
	g_gun.bShake = bShake;
	g_gun.posChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	//揺れるY範囲を設定
	g_gun.posChildTarget = D3DXVECTOR3( 0.0f, shakeMax, 0.0f);
}
/*******************************************************************************
* 弾切れのアニメ
*******************************************************************************/
void SetGunBulletOutAni(bool bBulletOut)
{
	g_gun.bBulletOut = bBulletOut;
	g_gun.rotChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_gun.nCheckFrame = 30;

	//弾切れの時、銃の角度
	g_gun.nKeyFrame = 0;
	g_gun.rotChildTarget = g_BulletOutAnime[0];


	//時計回り、または逆時計回りを決める
	g_gun.rotChildAngle =  Get2VecRotAngle( g_gun.rot, g_gun.rotChildTarget + g_gun.rot, DIVIDE_ROTATE, -1);
	g_gun.rotChild += g_gun.rotChildAngle;
}

/*******************************************************************************
* 銃の発射準備はどうか
*******************************************************************************/
bool IsGunReady(void)
{
	if(g_gun.bBulletOut || g_gun.bShot)
	{
		return false;
	}
	return true;
}
