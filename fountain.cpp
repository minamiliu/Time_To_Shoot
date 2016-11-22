/*******************************************************************************
*
* タイトル:		噴水の水パーティクル処理
* プログラム名:	fountain.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "fountain.h"
#include "camera.h"
#include "shadow.h"
#include "sound.h"
#include "hitCheck.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE	(128)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/shadow000.jpg"					// 読み込むテクスチャファイル名


#define VALUE_MOVE		(5.0f)											// 移動量
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 								// 回転量

#define SIZE_X			(20.0f)											// 地面のサイズ(X方向)
#define SIZE_Y			(20.0f)											// 地面のサイズ(Z方向)
																		
#define CREATE_RATE	(1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFountain(LPDIRECT3DDEVICE9 pDevice);
void SetColorFountain(int nIdxFountain, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
FOUNTAIN g_aFountain[MAX_PARTICLE];
LPDIRECT3DTEXTURE9		g_pD3DTextureFountain;								// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFountain;								// 頂点バッファインターフェースへのポインタ
bool g_bDraw;
int g_bDrawTimer;
D3DXVECTOR3 g_posPtc;
int g_nPtcCreateRate;
int g_nPtcCreateCounter;
float g_fDisToPlayer = 0;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFountain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_bDraw = false;
	g_bDrawTimer = 0;
	g_posPtc = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	
	// 位置・回転・スケールの初期設定
	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		g_aFountain[nCntBoard].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aFountain[nCntBoard].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aFountain[nCntBoard].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_aFountain[nCntBoard].mov = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);

		g_aFountain[nCntBoard].volume = 1.0f;
		g_aFountain[nCntBoard].alpha = 0;
		g_aFountain[nCntBoard].timer = 0;
		g_aFountain[nCntBoard].bUse = false;
	}

	g_pD3DVtxBuffFountain = NULL;

	
	// 頂点情報の作成
	MakeVertexFountain(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureFountain);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFountain(void)
{
	if(g_pD3DTextureFountain != NULL)
	{// テクスチャの開放
		g_pD3DTextureFountain->Release();
		g_pD3DTextureFountain = NULL;
	}


	if(g_pD3DVtxBuffFountain != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffFountain->Release();
		g_pD3DVtxBuffFountain = NULL;
	}	


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFountain(float fTimeSpeed)
{
	if(!g_bDraw) return;

	//時間により、生産率が変わる
	g_nPtcCreateRate = CREATE_RATE / fTimeSpeed;
	g_nPtcCreateCounter++;

	//sound volume
	PLAYER *pPlayer = GetPlayer();
	float distance = GetDistanceXZ(pPlayer->pos, g_posPtc);
	if( distance > 1000.0f || fTimeSpeed <= 0.01f)
	{
		UpdateSoundVolume( SOUND_LABEL_SE_WATER, 0.0f);
	}
	else
	{
		float volume = (1000 - (int)distance % 1000)  * 0.001f;
		UpdateSoundVolume( SOUND_LABEL_SE_WATER, volume);		
	}

	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		if(!g_aFountain[nCntBoard].bUse && g_nPtcCreateCounter >= g_nPtcCreateRate)
		{
			g_aFountain[nCntBoard].bUse = true;
			g_aFountain[nCntBoard].timer = 0;
			g_aFountain[nCntBoard].pos = g_posPtc;
			g_aFountain[nCntBoard].mov.x = (rand() % 100 - 50) * 0.03f;
			g_aFountain[nCntBoard].mov.z = (rand() % 100 - 50) * 0.03f;
			g_aFountain[nCntBoard].mov.y = 7.0f;
			g_aFountain[nCntBoard].nIdxShadow = CreateShadow( g_aFountain[nCntBoard].pos, 10.0f, 10.0f);
			g_nPtcCreateCounter = 0;
			break;
		}
		else
		{
			g_aFountain[nCntBoard].timer += 1.0f * fTimeSpeed;
			g_aFountain[nCntBoard].pos.x += g_aFountain[nCntBoard].mov.x * fTimeSpeed;
			g_aFountain[nCntBoard].pos.y = g_posPtc.y + g_aFountain[nCntBoard].mov.y * g_aFountain[nCntBoard].timer - 0.5f * 0.2f * g_aFountain[nCntBoard].timer * g_aFountain[nCntBoard].timer;
			g_aFountain[nCntBoard].pos.z += g_aFountain[nCntBoard].mov.z * fTimeSpeed;

			g_aFountain[nCntBoard].alpha += 2.0f * fTimeSpeed;
			SetColorFountain( nCntBoard, D3DCOLOR_RGBA(255, 255, 255, (int)g_aFountain[nCntBoard].alpha) );

			if(g_aFountain[nCntBoard].pos.y < -SIZE_Y/2)
			{
				g_aFountain[nCntBoard].mov.y = g_aFountain[nCntBoard].mov.y * 2.0f / 3.0f;
				g_aFountain[nCntBoard].timer = 0;
			}

			SetPositionShadow( g_aFountain[nCntBoard].nIdxShadow, g_aFountain[nCntBoard].pos);
			SetColorShadow( g_aFountain[nCntBoard].nIdxShadow, D3DCOLOR_RGBA( 255, 255, 255, (int)((255 - g_aFountain[nCntBoard].alpha) * 0.3)) );

			if(g_aFountain[nCntBoard].alpha >= 255)
			{
				g_aFountain[nCntBoard].bUse = false;
				g_aFountain[nCntBoard].pos = g_posPtc;
				g_aFountain[nCntBoard].mov = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);
				g_aFountain[nCntBoard].alpha = 0;
				ReleaseShadow( g_aFountain[nCntBoard].nIdxShadow);
			}

		}

	}

	//setparticle timer
	if(g_bDrawTimer > 0)
	{
		g_bDrawTimer--;
		for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
		{
			ReleaseShadow(g_aFountain[nCntBoard].nIdxShadow);
		}
		
	}
	else if(g_bDrawTimer == -1)
	{
		//無限ループ
	}
	else
	{
		g_bDraw = false;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFountain(void)
{
	if(!g_bDraw) return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	CAMERA *pCamera = GetCamera();

	// 加算合成
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	// ライティングモードをOFF
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
		
	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		if(g_aFountain[nCntBoard].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_aFountain[nCntBoard].mtxWorld);

			//ワールドマトリックスにカメラの行列を転置行列として設定する
			g_aFountain[nCntBoard].mtxWorld._11 = pCamera->mtxView._11;
			g_aFountain[nCntBoard].mtxWorld._22 = pCamera->mtxView._22;
			g_aFountain[nCntBoard].mtxWorld._33 = pCamera->mtxView._33;

			g_aFountain[nCntBoard].mtxWorld._12 = pCamera->mtxView._21;
			g_aFountain[nCntBoard].mtxWorld._13 = pCamera->mtxView._31;
			g_aFountain[nCntBoard].mtxWorld._23 = pCamera->mtxView._32;

			g_aFountain[nCntBoard].mtxWorld._21 = pCamera->mtxView._12;
			g_aFountain[nCntBoard].mtxWorld._31 = pCamera->mtxView._13;
			g_aFountain[nCntBoard].mtxWorld._32 = pCamera->mtxView._23;

			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_aFountain[nCntBoard].scl.x, g_aFountain[nCntBoard].scl.y, g_aFountain[nCntBoard].scl.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxScl);
	
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aFountain[nCntBoard].rot.y, g_aFountain[nCntBoard].rot.x, g_aFountain[nCntBoard].rot.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxRot);
	
			// 移動を反映
			D3DXMatrixTranslation( &mtxTranslate, g_aFountain[nCntBoard].pos.x, g_aFountain[nCntBoard].pos.y, g_aFountain[nCntBoard].pos.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxTranslate);
	
			// ワールドマトリックスの設定
			pDevice->SetTransform( D3DTS_WORLD, &g_aFountain[nCntBoard].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffFountain, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureFountain);

			//αテスト
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBoard*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);			
		}
	}


	// ライティングモードをON
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 通常ブレンド 
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z比較あり
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFountain(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_PARTICLE,	//頂点データのバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffFountain,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFountain->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++, pVtx+=4)
	{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x - (SIZE_X/2), g_aFountain[nCntBoard].pos.y + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x + (SIZE_X/2), g_aFountain[nCntBoard].pos.y + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x - (SIZE_X/2), g_aFountain[nCntBoard].pos.y - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x + (SIZE_X/2), g_aFountain[nCntBoard].pos.y - (SIZE_Y/2), 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffFountain->Unlock();

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorFountain(int nIdxFountain, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFountain->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtxをインデックス分ズラす
		pVtx += nIdxFountain * 4;

		// 頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffFountain->Unlock();
	}
}

/*******************************************************************************
* 噴水の設置
*******************************************************************************/
void SetFountain(int drawTime, D3DXVECTOR3 pos)
{
	g_bDraw = true;
	g_bDrawTimer = drawTime;
	g_posPtc = pos;
}





