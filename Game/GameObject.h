#pragma once
#include "Game.h"
#include "CXLib.h"
#include "Vec2.h"

// 列挙型の定義 ============================================================

// <位置関係> ----------------------------------------------------------
typedef enum
{
	NONE = 0,
	CENTER_X = 1,				// X中央
	LEFT = -2,					// 左
	RIGHT = 2,					// 右
	CENTER_Y = -1,				// Y中央
	TOP = -3,					// 上
	BOTTOM = 3					// 下
} ObjectSide;

// <縁の位置> ----------------------------------------------------------
typedef enum
{
	EDGESIDE_CENTER = 0,		// 縁の上
	EDGESIDE_OUTER = -1,		// 縁の外側
	EDGESIDE_INNER = 1			// 縁の内側
} ObjectEdgeSide;

// <ワールドのつながり> ------------------------------------------------
typedef enum
{
	CONNECTION_NONE = 0,		// 繋がりなし、見えない空間に移動
	CONNECTION_BARRIER,			// 壁があり、進めない
	CONNECTION_LOOP				// 反対側から出てくる
} ObjectConnection;

// <オブジェクトの形> --------------------------------------------------
typedef enum
{
	SHAPE_BOX,					// 長方形
	SHAPE_CIRCLE,				// 円
} ObjectShape;

// 構造体の宣言 ============================================================

// <ゲームオブジェクト>
typedef struct
{
	Vec2 pos;					// <位置>
	Vec2 vel;					// <速度>
	Vec2 size;					// <大きさ>
	ObjectShape shape;			// <形>
	ATTR color;					// <色>
	ObjectConnection sprite_connection;	// <スプライトのつながり>
	BOOL alive;					// <表示状態>
	int state;					// <状態>
	int type;					// <タイプ>
} GameObject;

// 定数の定義 ==============================================================

// <テクスチャ>
#define TEXTURE_MISSING -1		// テクスチャが見つかりません
#define TEXTURE_NONE -2			// テクスチャなし

// 関数の宣言 ==============================================================

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos = Vec2_Create(), Vec2 vel = Vec2_Create(), Vec2 size = Vec2_Create());

// <オブジェクト削除>
void GameObject_Dispose(GameObject* obj);

// <オブジェクト確認>
BOOL GameObject_IsAlive(const GameObject* obj);

// <オブジェクト座標更新>
void GameObject_UpdatePosition(GameObject* obj);

// <オブジェクトXオフセット>
float GameObject_OffsetX(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <オブジェクトXオフセット>
float GameObject_OffsetY(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <オブジェクトX位置ゲット>
float GameObject_GetX(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <オブジェクトY位置ゲット>
float GameObject_GetY(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <オブジェクトの1フレーム速度X>
float GameObject_GetDeltaVelX(const GameObject* obj);

// <オブジェクトの1フレーム速度Y>
float GameObject_GetDeltaVelY(const GameObject* obj);

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2);

// <オブジェクト描画>
void GameObject_Render(const GameObject* obj, const Vec2* translate = &Vec2_Create());

// <<フィールドオブジェクト>> ------------------------------------------

// <フィールドオブジェクト作成>
GameObject GameObject_Field_Create(void);

// <フィールド上下衝突処理>
ObjectSide GameObject_Field_CollisionVertical(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <フィールド左右衝突処理>
ObjectSide GameObject_Field_CollisionHorizontal(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);
