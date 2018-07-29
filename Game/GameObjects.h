#pragma once
#include "GameObject.h"

// 定数の定義 ==============================================================

// <ボール>
#define BALL_VEL_X_MAX (8*4)	// パドルが移動している状態でボールにあたった時の速度
#define BALL_VEL_X_MIN (6*4)	// パドルが静止している状態でボールにあたった時の速度
#define BALL_VEL_Y (4.5f*4)		// Yの最大速度

// <パドル>
#define PADDLE_VEL (8*4)		// パドルの速度

// 列挙型の定義 ============================================================

// <種類> --------------------------------------------------------------

typedef enum
{
	TYPE_UNKNOWN = 0,
	TYPE_PLAYER,
	TYPE_PLAYER_BULLET,
	TYPE_ENEMY1,
	TYPE_ENEMY2
} ObjectType;

// 関数の宣言 ==============================================================

// <<ボールオブジェクト>> ----------------------------------------------

// <ボールオブジェクト作成>
GameObject GameObject_Ball_Create(void);

// <ボールオブジェクト座標Xデフォルト>
void GameObject_Ball_SetPosXDefault(GameObject* obj, GameObject* field);

// <ボールオブジェクト座標Yデフォルト>
void GameObject_Ball_SetPosYDefault(GameObject* obj, GameObject* field);

// <ボールオブジェクト移動Xデフォルト>
void GameObject_Ball_SetVelXDefault(GameObject* obj);

// <ボールオブジェクト移動Yデフォルト>
void GameObject_Ball_SetVelYDefault(GameObject* obj);

// <<パドルオブジェクト>> ----------------------------------------------

// <パドルオブジェクト作成>
GameObject GameObject_Paddle_Create(void);

// <パドルオブジェクト座標Yデフォルト>
void GameObject_Paddle_SetPosYDefault(GameObject* obj, GameObject* field);

// <パドルオブジェクトボール衝突処理>
BOOL GameObject_Paddle_CollisionBall(GameObject* paddle, GameObject* ball);

// <<フィールドオブジェクト>> ------------------------------------------

// <フィールド描画>
void GameObject_Field_Render(const GameObject* field);
