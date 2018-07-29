//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong Online! オリジナル課題
//!
//! @date   2018/06/13
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "Game.h"
#include "GameObject.h"
#include "GameObjects.h"
#include "GameControllers.h"
#include "GameScore.h"
#include "InputManager.h"


// 定数の定義 ==============================================================

// <点差ガイド> --------------------------------------------------------
#define SCORE_TO_GUID 6

// <サーブ待機> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// グローバル変数の宣言 ====================================================

// <シーン> ------------------------------------------------------------
GameScene g_scene;

// <コントローラー> ----------------------------------------------------
GameControllers g_controllers;

// <フォント> ----------------------------------------------------------
CXFont g_font_pong;


// 関数の宣言 ==============================================================

// <ゲームの更新処理:シーン> -------------------------------------------
void UpdateGameSceneDemo(void);
void UpdateGameSceneServe(void);
void UpdateGameScenePlay(void);

// <ゲームの描画処理> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

void UpdateGameScore(ObjectSide side);

// 関数の定義 ==============================================================

//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeGame(void)
{
	// シーン状態
	g_scene.game_state = STATE_DEMO;

	// フィールド
	g_scene.field = GameObject_Field_Create();

	// ボール
	g_scene.ball = GameObject_Ball_Create();
	GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetPosYDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetVelXDefault(&g_scene.ball);
	GameObject_Ball_SetVelYDefault(&g_scene.ball);

	// パドル1
	g_scene.paddle1 = GameObject_Paddle_Create();
	g_scene.paddle1.pos.x = GameObject_OffsetX(&g_scene.paddle1, LEFT, GameObject_GetX(&g_scene.field, LEFT), -12);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
	g_controllers.paddle1 = GameController_Bot_Create(&g_scene.paddle1, &g_scene, &g_scene.paddle2);

	// パドル2
	g_scene.paddle2 = GameObject_Paddle_Create();
	g_scene.paddle2.pos.x = GameObject_OffsetX(&g_scene.paddle2, RIGHT, GameObject_GetX(&g_scene.field, RIGHT), -12);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);
	g_controllers.paddle2 = GameController_Player_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1, KEY_UP, KEY_DOWN);
	//g_controllers.paddle2 = GameController_Bot_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1);

	// フォント
	g_font_pong = CreateFontToHandle(CXFONT_PONG, 10);

	// 得点
	g_scene.score = GameScore_Create();

	// サーブ待機
	g_scene.counter = 0;
}


//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateGame(void)
{
	switch (g_scene.game_state)
	{
	case STATE_DEMO:
		UpdateGameSceneDemo();
		break;
	case STATE_SERVE:
		UpdateGameSceneServe();
		break;
	case STATE_PLAY:
		UpdateGameScenePlay();
		break;
	}
}

// <ゲームの更新処理:シーン:デモ> --------------------------------------
void UpdateGameSceneDemo(void)
{
	// 待機&初期化
	{
		// 入力されたら
		if (IsKeyDown(KEY_SPACE))
		{
			// 点数リセット
			GameScore_Clear(&g_scene.score);

			// X座標を画面中央へ戻す
			GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

			// パドルを初期位置へ
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);

			// シーンをプレイに変更
			g_scene.game_state = STATE_PLAY;
			g_scene.packet = PACKET_START;
		}
	}

	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);

	// 当たり判定
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	if (GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.x *= -1;
}

// <ゲームの更新処理:シーン:サーブ> ------------------------------------
void UpdateGameSceneServe(void)
{
	// 待機&初期化
	g_scene.counter++;

	// 時間経過で
	if (g_scene.counter >= SERVE_WAIT_TIME)
	{
		// X座標を画面中央へ戻す
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// シーンをプレイに変更
		g_scene.game_state = STATE_PLAY;
		g_scene.packet = PACKET_SERVE;

		g_scene.counter = 0;
	}

	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 操作
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// 当たり判定
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
}

// <ゲームの更新処理:シーン:プレイ> ------------------------------------
void UpdateGameScenePlay(void)
{
	// コントローラー更新
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// 操作
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// 座標更新
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// 当たり判定
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
	{
		g_scene.ball.vel.y *= -1;
	}
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_NONE, EDGESIDE_OUTER);
		if (side)
		{
			UpdateGameScore(side);
		}
	}
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);

	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
}

// <ゲームの更新処理:スコア加算>
void UpdateGameScore(ObjectSide side)
{
	// 得点処理
	GameScore_Add(&g_scene.score, side);

	if (GameScore_IsFinished(&g_scene.score))
	{
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
		GameObject_Ball_SetVelXDefault(&g_scene.ball);
		GameObject_Ball_SetVelYDefault(&g_scene.ball);

		// シーンをデモに変更
		g_scene.game_state = STATE_DEMO;
		g_scene.packet = PACKET_END;
	}
	else
	{
		// シーンをサーブに変更
		g_scene.game_state = STATE_SERVE;
		g_scene.packet = PACKET_SCORE;
	}
}

//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderGame(void)
{
	switch (g_scene.game_state)
	{
	case STATE_DEMO:
		RenderGameSceneDemo();
		break;
	case STATE_SERVE:
		RenderGameSceneServe();
		break;
	case STATE_PLAY:
		RenderGameScenePlay();
		break;
	}
}

// <ゲームの描画処理:シーン:デモ> ---------------------------------------------
void RenderGameSceneDemo(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_scene.field);
	// ボール描画
	GameObject_Render(&g_scene.ball);
	// メニュー描画
	{
		float width = GetDrawStringWidthToHandle("Pong Game", &g_font_pong);
		DrawStringToHandle(GameObject_GetX(&g_scene.field, CENTER_X) - width / 2 - 10, GameObject_GetY(&g_scene.field, CENTER_Y), "Pong Game", ATTR_WHITE, &g_font_pong);
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 10, GameObject_GetY(&g_scene.field, CENTER_Y) + 18, "スペースキーを押してゲームを始めよう！", CreateATTR(COLOR_YELLOW, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 11, GameObject_GetY(&g_scene.field, CENTER_Y) + 20, "↑↓キーで右パドルを操作してCPUを倒そう！", CreateATTR(COLOR_MAGENTA, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 12, GameObject_GetY(&g_scene.field, CENTER_Y) + 22, "ゲームが早く終わるように5点マッチにしました！", CreateATTR(COLOR_GREEN, COLOR_BLACK));
	}
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
}

// <ゲームの描画処理:シーン:サーブ> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_scene.field);
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
	// パドル描画
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// ボール描画
	GameObject_Render(&g_scene.ball);
}

// <ゲームの描画処理:シーン:プレイ> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <オブジェクト描画>
	// フィールド描画
	GameObject_Field_Render(&g_scene.field);
	// スコア描画
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
	// ガイド描画
	if (g_scene.score.score2 - g_scene.score.score1 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle1);
	if (g_scene.score.score1 - g_scene.score.score2 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle2);
	// パドル描画
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// ボール描画
	GameObject_Render(&g_scene.ball);
}

//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeGame(void)
{
}
