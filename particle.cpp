/*******************************************************************************
*
* タイトル:		敵の血(パーティクル)処理
* プログラム名:	particle.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "particle.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE	(5)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/shadow000.jpg"					// 読み込むテクスチャファイル名


#define SIZE_X			(10.0f)											// 地面のサイズ(X方向)
#define SIZE_Y			(10.0f)											// 地面のサイズ(Z方向)

#define CREATE_RATE	(1)
																		

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetColorParticle(int nIdxPtc, int nIdxDot, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle;								// テクスチャへのポインタ

PARTICLE g_aParticle[10];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		g_aParticle[nCntPtc].bDraw = false;
		g_aParticle[nCntPtc].nNeedCreateNum = 0;
		g_aParticle[nCntPtc].nPtcCreateRate = CREATE_RATE;
		g_aParticle[nCntPtc].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		// 位置・回転・スケールの初期設定
		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
		{
			g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);

			g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha = 0;
			g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0;
			g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
		}	

		g_aParticle[nCntPtc].pD3DVtxBuffParticle = NULL;
	}

	
	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureParticle);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if(g_pD3DTextureParticle != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].pD3DVtxBuffParticle != NULL)
		{
			g_aParticle[nCntPtc].pD3DVtxBuffParticle->Release();
			g_aParticle[nCntPtc].pD3DVtxBuffParticle = NULL;
		}
		g_aParticle[nCntPtc].bDraw = false;

		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
		{
			g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
		}
		
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle( float fTimeSpeed)
{
	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].bDraw)
		{
			//時間により、生産率が変わる
			g_aParticle[nCntPtc].nPtcCreateRate = CREATE_RATE / fTimeSpeed;
			g_aParticle[nCntPtc].nPtcCreateCounter++;

			for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
			{
				if(!g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse && g_aParticle[nCntPtc].nPtcCreateCounter >= g_aParticle[nCntPtc].nPtcCreateRate && g_aParticle[nCntPtc].nNeedCreateNum > 0)
				{
					g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = true;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0.0f;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.x = (rand() % 100 - 50) * 0.03f;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.z = (rand() % 100 - 50) * 0.03f;

					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot = g_aParticle[nCntPtc].pos;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow = CreateShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot, 10.0f, 10.0f);
					g_aParticle[nCntPtc].nNeedCreateNum--;
					g_aParticle[nCntPtc].nPtcCreateCounter = 0;
					break;
				}
				else
				{
					g_aParticle[nCntPtc].aPtcDot[nCntDot].timer += 1.0f * fTimeSpeed;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x += g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.x * fTimeSpeed;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y = (g_aParticle[nCntPtc].pos.y + g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer - 0.5f * 0.2f * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer);
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.z += g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.z * fTimeSpeed;

					g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha += 2.0f * fTimeSpeed;
					SetColorParticle( nCntPtc, nCntDot, D3DCOLOR_RGBA(255, 0, 0, (int)g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha) );

					if(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y < -SIZE_Y/2)
					{
						g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y = g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y * 2.0f / 3.0f;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0.0f;
					}

					SetPositionShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot);
					SetColorShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow, D3DCOLOR_RGBA( 255, 255, 255, (int)((255 - g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha) * 0.3)) );

					if(g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha >= 255 && g_aParticle[nCntPtc].nNeedDeteNum > 0)
					{
						g_aParticle[nCntPtc].nNeedDeteNum--;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot =  g_aParticle[nCntPtc].pos;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);
						g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha = 0;
						ReleaseShadow(  g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow);
					}

				}

			}

			if( g_aParticle[nCntPtc].nNeedDeteNum <= 0 &&  g_aParticle[nCntPtc].nNeedCreateNum <= 0)
			{
				for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
				{
					ReleaseShadow(g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow);
					g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
				}

				g_aParticle[nCntPtc].bDraw = false;
			}
		}
	}







}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
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

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].bDraw)
		{		
			for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
			{
				if(g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse)
				{
					// ワールドマトリックスの初期化
					D3DXMatrixIdentity( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld);

					//ワールドマトリックスにカメラの行列を転置行列として設定する
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._11 = pCamera->mtxView._11;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._22 = pCamera->mtxView._22;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._33 = pCamera->mtxView._33;
				
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._12 = pCamera->mtxView._21;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._13 = pCamera->mtxView._31;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._23 = pCamera->mtxView._32;
					
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._21 = pCamera->mtxView._12;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._31 = pCamera->mtxView._13;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._32 = pCamera->mtxView._23;

					// スケールを反映
					D3DXMatrixScaling( &mtxScl, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxScl);
	
					// 回転を反映
					D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxRot);
	
					// 移動を反映
					D3DXMatrixTranslation( &mtxTranslate, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxTranslate);
	
					// ワールドマトリックスの設定
					pDevice->SetTransform( D3DTS_WORLD, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld);

					// 頂点バッファをデバイスのデータストリームにバインド
					pDevice->SetStreamSource(0,  g_aParticle[nCntPtc].pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);

					// テクスチャの設定
					pDevice->SetTexture(0, g_pD3DTextureParticle);

					//αテスト
					pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
					pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
					pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

					//ポリゴンの描画
					pDevice->DrawPrimitive(
						D3DPT_TRIANGLESTRIP,	//プリミティブの種類
						nCntDot*NUM_VERTEX,	//ロードする最初の頂点インデックス
						NUM_POLYGON				//ポリゴンの数
					);			
				}
			}


			
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
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{


	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D)*NUM_VERTEX*MAX_DOT,	//頂点データのバッファサイズ
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,					//頂点フォーマット
			D3DPOOL_MANAGED,
			&g_aParticle[nCntPtc].pD3DVtxBuffParticle,			//頂点バッファインターフェースのポインタ
			NULL)))
		{
			return E_FAIL;
		}


		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_aParticle[nCntPtc].pD3DVtxBuffParticle->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++, pVtx+=4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x - (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y + (SIZE_Y/2), 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x + (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y + (SIZE_Y/2), 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x - (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y - (SIZE_Y/2), 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x + (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y - (SIZE_Y/2), 0.0f);

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
		g_aParticle[nCntPtc].pD3DVtxBuffParticle->Unlock();
	}


	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxPtc, int nIdxDot, D3DXCOLOR col)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_aParticle[nIdxPtc].pD3DVtxBuffParticle->Lock( 0, 0, (void**)&pVtx, 0);

	// pVtxをインデックス分ズラす
	pVtx += nIdxDot * 4;

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点データをアンロックする
	g_aParticle[nIdxPtc].pD3DVtxBuffParticle->Unlock();	


}

//=============================================================================
// パーティクルの設置
//=============================================================================
void SetParticle(int needNum, D3DXVECTOR3 pos)
{
	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if( !g_aParticle[nCntPtc].bDraw)
		{
			g_aParticle[nCntPtc].bDraw = true;
			g_aParticle[nCntPtc].nNeedCreateNum = needNum;
			g_aParticle[nCntPtc].nNeedDeteNum = needNum;
			g_aParticle[nCntPtc].pos = pos;
			g_aParticle[nCntPtc].nPtcCreateCounter = 0;
			break;
		}
	}
}





