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
#include <stdio.h>
#include "GameObject.h"
#include "GameObjects.h"
#include "GameControllers.h"
#include "GameScore.h"
#include "GameUtils.h"
#include "InputManager.h"


// 定数の定義 ==============================================================

// <点差ガイド> --------------------------------------------------------
#define SCORE_TO_GUID 2

// <サーブ待機> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// グローバル変数の定義 ====================================================

// <シーン> ------------------------------------------------------------
GameScene g_scene;

// <コントローラー> ----------------------------------------------------
GameControllers g_controllers;

// <フォント> ----------------------------------------------------------
CXFont g_font_pong;

// <ポーズ> ------------------------------------------------------------
BOOL g_paused;
int g_pause_select;


// 関数の宣言 ==============================================================

// <ゲームの更新処理:シーン> -------------------------------------------
void UpdateGameSceneDemo(void);
void UpdateGameSceneServe(void);
void UpdateGameScenePlay(void);

// <ゲームの描画処理> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

// <ゲームの更新処理:スコア加算>
void UpdateGameScore(ObjectSide side);
// <ゲームの初期化処理:ポーズ>
void InitializeGamePause(void);
// <ゲームの更新処理:ポーズ>
void UpdateGamePause(void);
// <ゲームの描画処理:ポーズ>
void RenderGamePause(void);

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
	g_scene.ball.color = CreateATTR(COLOR_BLACK, COLOR_YELLOW);
	GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetPosYDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetVelXDefault(&g_scene.ball);
	GameObject_Ball_SetVelYDefault(&g_scene.ball);

	// パドル1
	g_scene.paddle1 = GameObject_Paddle_Create();
	g_scene.paddle1.color = CreateATTR(COLOR_BLACK, COLOR_RED);
	g_scene.paddle1.pos.x = GameObject_OffsetX(&g_scene.paddle1, LEFT, GameObject_GetX(&g_scene.field, LEFT), -12);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
	g_controllers.paddle1 = GameController_Bot_Create(&g_scene.paddle1, &g_scene, &g_scene.paddle2);

	// パドル2
	g_scene.paddle2 = GameObject_Paddle_Create();
	g_scene.paddle2.color = CreateATTR(COLOR_BLACK, COLOR_BLUE);
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

// <ゲームの初期化処理:ポーズ>
void InitializeGamePause(void)
{
	g_paused = TRUE;
	g_pause_select = 0;
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
	// Escを押されたら終了
	if (IsKeyDown(KEY_ESC))
		ExitGame();

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
	// Escを押されたらポーズ
	if (IsKeyDown(KEY_ESC))
		InitializeGamePause();
	// ポーズ中ならポーズ処理
	if (g_paused)
	{
		UpdateGamePause();
		return;
	}

	// 待機&初期化
	g_scene.counter++;

	// 時間経過で
	if (g_scene.counter >= SERVE_WAIT_TIME)
	{
		// X座標を画面中央へ戻す
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// シーンをプレイに変更
		g_scene.game_state = STATE_PLAY;

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
	// Escを押されたらポーズ
	if (IsKeyDown(KEY_ESC))
		InitializeGamePause();
	// ポーズ中ならポーズ処理
	if (g_paused)
	{
		UpdateGamePause();
		return;
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

// <ゲームの更新処理:ポーズ>
void UpdateGamePause(void)
{
	if (IsKeyPressed(KEY_UP))
		g_pause_select = GetLoop(g_pause_select - 1, 3);
	if (IsKeyPressed(KEY_DOWN))
		g_pause_select = GetLoop(g_pause_select + 1, 3);
	if (IsKeyPressed(KEY_SPACE))
	{
		switch (g_pause_select)
		{
		default:
		case 0:
			g_paused = FALSE;
			break;
		case 1:
			g_paused = FALSE;
			// 終了処理
			{
				GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
				GameObject_Ball_SetVelXDefault(&g_scene.ball);
				GameObject_Ball_SetVelYDefault(&g_scene.ball);

				// シーンをデモに変更
				g_scene.game_state = STATE_DEMO;
			}
			break;
		case 2:
			ExitGame();
			break;
		}
	}
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
	}
	else
	{
		// シーンをサーブに変更
		g_scene.game_state = STATE_SERVE;
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
		//DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) - 9, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 7, CreateATTR(COLOR_BLACK, COLOR_BLACK), TRUE);
		DrawLine(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 5, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 5, CreateATTR(COLOR_BLACK, COLOR_DARK_GREEN));
		DrawStringToHandle(GameObject_GetX(&g_scene.field, CENTER_X) - width / 2 - 10, GameObject_GetY(&g_scene.field, CENTER_Y) - 7, "Pong Game", CreateATTR(COLOR_BLACK, COLOR_CYAN), &g_font_pong);
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) + 20, GameObject_GetY(&g_scene.field, CENTER_Y) + 7, "～レトロな卓球スポーツゲーム～", CreateATTR(COLOR_YELLOW, COLOR_BLACK));

		DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 14, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 28, CreateATTR(COLOR_BLACK, COLOR_BLACK), TRUE);
		DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 14, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 28, CreateATTR(COLOR_BLACK, COLOR_DARK_MAGENTA), FALSE);

		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 10, GameObject_GetY(&g_scene.field, CENTER_Y) + 17, "スペースキーを押してゲームを始めよう！", CreateATTR(COLOR_YELLOW, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 11, GameObject_GetY(&g_scene.field, CENTER_Y) + 19, "↑↓キーで右パドルを操作してCPUを倒そう！", CreateATTR(COLOR_MAGENTA, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 11, GameObject_GetY(&g_scene.field, CENTER_Y) + 21, "忙しいあなたのために5点マッチにしました！", CreateATTR(COLOR_GREEN, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 10, GameObject_GetY(&g_scene.field, CENTER_Y) + 23, "2点差がつくと、ガイドが表示されるYO！", CreateATTR(COLOR_DARK_BLUE, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 14, GameObject_GetY(&g_scene.field, CENTER_Y) + 25, "Pong Gameだって卓球という立派なスポーツゲームダゾ！", CreateATTR(COLOR_DARK_GRAY, COLOR_BLACK));

		DrawString(GameObject_GetX(&g_scene.field, RIGHT, -17), GameObject_GetY(&g_scene.field, BOTTOM, -3), "Copyrights 2018 YdeaGames", CreateATTR(COLOR_GRAY, COLOR_BLACK));
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

	// ポーズ中ならポーズ処理
	if (g_paused)
		RenderGamePause();
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

	// ポーズ中ならポーズ処理
	if (g_paused)
		RenderGamePause();
}

// <ゲームの描画処理:ポーズ>
void RenderGamePause(void)
{
	char str[20];
	snprintf(str, 20, "[ %d ]", g_pause_select);
	DrawString(2, 2, str, DEFAULT_ATTR);
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
	// 情報取得
	CXFont font = { CXFONT_DEFAULT, SCREEN_FONT_SIZE };
	float widththanks = GetDrawStringWidthToHandle("Thank You!", &g_font_pong);
	float widthmsg = GetDrawStringWidthToHandle("プレイいただきありがとうございました。", &font);
	float widthwait = GetDrawStringWidthToHandle("続行するには何かキーを押してください . . .", &font);
	// 画面をクリア
	Clear();
	// 終了画面
	DrawStringToHandle(WORLD_CENTER_X - widththanks / 2 - 10, WORLD_CENTER_Y - 10, "Thank You!", CreateATTR(COLOR_BLACK, COLOR_CYAN), &g_font_pong);
	DrawStringToHandle(WORLD_CENTER_X - widthmsg / 2, WORLD_CENTER_Y + 5, "プレイいただきありがとうございました。", CreateATTR(COLOR_YELLOW, COLOR_BLACK), &font);
	BufferedConsole_Flush();
	// 後始末
	SetTextColor(COLOR_DARK_GRAY);
	SetBackColor(COLOR_BLACK);
	SetCursorPosition(SCREEN_RIGHT - WorldToConsoleX(widthwait) - 2, SCREEN_BOTTOM - 2);
}
