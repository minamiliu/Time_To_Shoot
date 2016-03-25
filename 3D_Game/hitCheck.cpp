/*******************************************************************************
*
* タイトル:		当たり判定の処理
* プログラム名:	gunSight.cpp
* 作成者:		HAL東京ゲーム学科　劉南宏
*
*******************************************************************************/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "hitCheck.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		NO_USE_D3DX	// 宣言をコメントにするとD3DXVec3Dot等を使用する
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// 回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 g_hitPos;

//=============================================================================
// 内積
//=============================================================================
float DotProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr)
{
	float ans;

#ifdef NO_USE_D3DX
	// ダイレクトＸでは、、、
	ans = (vl.x * vr.x) + (vl.y * vr.y) + (vl.z * vr.z);

#else


#endif

	return ans;

}


//=============================================================================
// 外積
//=============================================================================
D3DXVECTOR3 CrossProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr )
{
	D3DXVECTOR3 ret;

#ifdef NO_USE_D3DX
	// ダイレクトＸでは、、、
	ret.x = (vl.y * vr.z) - (vl.z*vr.y);
	ret.y = (vl.z * vr.x) - (vl.x*vr.z);
	ret.z = (vl.x * vr.y) - (vl.y*vr.x);
#else

#endif

	return ret;
}


//=============================================================================
// ポリゴンと線分との当たり判定
// p0  :ポリゴンの頂点1
// p1  :ポリゴンの頂点2
// p2  :ポリゴンの頂点3
// pos0:始点（移動前）
// pos1:終点（移動後）
// 
// 返り値：0は当たっていない  1:当たっている
//=============================================================================
bool HitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// ポリゴンの法線

	{	// ポリゴンの外積をとって法線を求める
		D3DXVECTOR3 vec01 = p1 - p0;
		D3DXVECTOR3 vec02 = p2 - p0;
		nor = CrossProduct( vec01, vec02);
		D3DXVec3Normalize( &nor, &nor);
	}

	{	// 平面と線分の内積とって衝突している可能性を調べる
		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		D3DXVECTOR3 vec1 = pos0 - p0;
		D3DXVECTOR3 vec2 = pos1 - p0;
		float a = DotProduct( vec1, nor);
		float b = DotProduct( vec2, nor);
		if( (a * b) > 0 ) return false;
	}

	{	// ポリゴンと線分の交点を求める
		D3DXVECTOR3 vec1 = pos0 - p0;
		D3DXVECTOR3 vec2 = pos1 - p0;
		float	d1 = fabs( DotProduct( nor, vec1));	// 内分比を求める為の点とポリゴンとの距離
		float	d2 = fabs( DotProduct( nor, vec2));	// 内分比を求める為の点とポリゴンとの距離
		float	a = d1 / (d1+d2);					// 内分比
		
		
		// pos0からpos1へのベクトル（移動ベクトル）
		D3DXVECTOR3 vec3 = pos1 -pos0; 

		// 交点
		D3DXVECTOR3 p3 = pos0 + vec3 * a;

		// 交点を渡す
		g_hitPos = p3;

		{	// 求めた交点がポリゴンの中にあるか調べる
			
			// ポリゴンの各辺のベクトル
			D3DXVECTOR3 v1 = p1 - p0;
			D3DXVECTOR3 v2 = p2 - p1;
			D3DXVECTOR3 v3 = p0 - p2;

			// 各頂点と交点とのベクトル
			D3DXVECTOR3 v4 = p3 - p1;
			D3DXVECTOR3 v5 = p3 - p2;
			D3DXVECTOR3 v6 = p3 - p0;

			// 各辺との外積で法線を求める
			D3DXVECTOR3 n1 = CrossProduct( v1, v4);
			D3DXVECTOR3 n2 = CrossProduct( v2, v5);
			D3DXVECTOR3 n3 = CrossProduct( v3, v6);

			// それぞれのベクトルの向きを内積でチェック
			if( DotProduct( n1, n2) < -0.02f ) return false;
			if( DotProduct( n1, n3) < -0.02f ) return false;
		}
	}

	return true;
}


//=============================================================================
// 壁ずりベクトル
//
// out : 正規化壁ずりベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
//
//=============================================================================
D3DXVECTOR3* GetWallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 &front, D3DXVECTOR3 &normal)
{
    D3DXVECTOR3 tNormal;
    D3DXVec3Normalize(&tNormal, &normal);

	*out = (front - DotProduct( front, tNormal) * tNormal);
	return out;
}

//*****************************************************************************
// 交点を取得
//*****************************************************************************
D3DXVECTOR3 GetHitPos(void)
{
	return g_hitPos;
}

//*****************************************************************************
// 丸の当たり判定
//*****************************************************************************
bool HitCheckBall( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2)
{

	//バウンディングスフィア
	if( (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y) + (pos1.z - pos2.z)*(pos1.z - pos2.z) 
		< (radius1 + radius2)*(radius1 + radius2) )
	{
		return true;
	}	
		
	return false;
}
//*****************************************************************************
// 平面の円の当たり判定
//*****************************************************************************
bool HitCheckXZ( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2)
{

	//バウンディングスフィア
	if( (pos1.x - pos2.x)*(pos1.x - pos2.x)  + (pos1.z - pos2.z)*(pos1.z - pos2.z) 
		< (radius1 + radius2)*(radius1 + radius2) )
	{
		return true;
	}	
		
	return false;
}
//*****************************************************************************
// ２点の向きベクトルを取得
//*****************************************************************************
D3DXVECTOR3 GetRotationAngle( D3DXVECTOR3 myPos, D3DXVECTOR3 target, D3DXVECTOR3 myAngle)
{
	// 方向角度
	D3DXVECTOR3 dir = target - myPos;
	// Y軸角度
	float yAngle = atan2( dir.x, dir.z );

	//180度以上差があれば+-360度して逆回し
	if( yAngle - myAngle.y > D3DX_PI ) {
		yAngle -= D3DX_PI * 2.0f;
	} else if( myAngle.y - yAngle > D3DX_PI ) {
		yAngle += D3DX_PI * 2.0f;
	}

	//Y軸角度修正
	if(yAngle > D3DX_PI)
	{
		yAngle = yAngle - D3DX_PI*2;
	}
	else if(yAngle < 0.0f - D3DX_PI)
	{
		yAngle = yAngle + D3DX_PI*2;
	}

	// X軸角度
	float zxLen = sqrt( dir.x * dir.x + dir.z * dir.z );
	float xAngle = atan2( dir.y, zxLen );

	return D3DXVECTOR3( -xAngle, yAngle, 0.0f); 
}
//*****************************************************************************
// 回転角度を取得
//*****************************************************************************
D3DXVECTOR3 Get2VecRotAngle( D3DXVECTOR3 rot, D3DXVECTOR3 rotTarget, float divide, float value_rot)
{
	float tAngle[3];
	D3DXVECTOR3 re;

	tAngle[0] = rotTarget.x - rot.x;
	tAngle[1] = rotTarget.y - rot.y;
	tAngle[2] = rotTarget.z - rot.z;

	for(int cntXYZ = 0; cntXYZ < 3; cntXYZ++)
	{
		if(value_rot == -1)
		{
			if( tAngle[cntXYZ] > 0)
			{
				if(tAngle[cntXYZ] >= D3DX_PI)
				{
					//tAngle[cntXYZ] = -value_rot;
					tAngle[cntXYZ] = -fabs(tAngle[cntXYZ]/divide);
				}
				else
				{
					//tAngle[cntXYZ] = value_rot;
					tAngle[cntXYZ] = fabs(tAngle[cntXYZ]/divide);
				}
						
			}
			else
			{
				if(tAngle[cntXYZ] < -D3DX_PI)
				{
					//tAngle[cntXYZ] = value_rot;
					tAngle[cntXYZ] = fabs(tAngle[cntXYZ]/divide);
				}
				else
				{
					//tAngle[cntXYZ] = -value_rot;
					tAngle[cntXYZ] = -fabs(tAngle[cntXYZ]/divide);
				}
			}		
		}
		else
		{
			if( tAngle[cntXYZ] > 0)
			{
				if(tAngle[cntXYZ] >= D3DX_PI)
				{
					tAngle[cntXYZ] = -value_rot;
				}
				else
				{
					tAngle[cntXYZ] = value_rot;
				}
						
			}
			else
			{
				if(tAngle[cntXYZ] < -D3DX_PI)
				{
					tAngle[cntXYZ] = value_rot;
				}
				else
				{
					tAngle[cntXYZ] = -value_rot;
				}
			}		
		}
	}

	re.x = tAngle[0];
	re.y = tAngle[1];
	re.z = tAngle[2];

	return re;
}
//*****************************************************************************
// 視野内の判定
//*****************************************************************************
bool HitCheckViewArea( D3DXVECTOR3 myPos, D3DXVECTOR3 tarPos, D3DXVECTOR3 myRot, float myView)
{
	D3DXVECTOR3 rotAnlge = GetRotationAngle( myPos, tarPos, myRot); 
	float fDiff = rotAnlge.y - myRot.y;

	if( fDiff > D3DX_PI)
	{
		fDiff = D3DX_PI * 2 - fDiff;
	}
	else if( fDiff < 0.0f - D3DX_PI)
	{
		fDiff = D3DX_PI * 2 + fDiff;
	}

	if( fabs(fDiff) <= myView)
	{
		return true;
	}
	return false;
}

//*****************************************************************************
// ２点距離を取得
//*****************************************************************************
float GetDistanceXZ( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return sqrtf( (pos1.x - pos2.x)*(pos1.x - pos2.x)  + (pos1.z - pos2.z)*(pos1.z - pos2.z) );
}
