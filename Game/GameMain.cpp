//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  ゲーム関連のソースファイル
//!
//! @date   2018/07/18
//!
//! @author GF1 26 山口 寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "GameMain.h"
#include "BufferedConsole.h"
#include "InputManager.h"




// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================
float g_vel_x;
float g_vel_y;
float g_pos_x;
float g_pos_y;



// 関数の宣言 ==============================================================

void InitializeGame(void);  // ゲームの初期化処理
void UpdateGame(void);      // ゲームの更新処理
void RenderGame(void);      // ゲームの描画処理
void FinalizeGame(void);    // ゲームの終了処理




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
	g_vel_x = 0;
	g_vel_y = 0;
	g_pos_x = WORLD_CENTER_X;
	g_pos_y = WORLD_CENTER_Y;
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
	if (IsKeyDown(KEY_LEFT))
		g_vel_x += -.001f;
	if (IsKeyDown(KEY_RIGHT))
		g_vel_x += .001f;
	if (IsKeyDown(KEY_UP))
		g_vel_y += -.001f;
	if (IsKeyDown(KEY_DOWN))
		g_vel_y += .001f;

	g_vel_x *= 0.9998f;
	g_vel_y *= 0.9998f;

	g_pos_x += g_vel_x;
	g_pos_y += g_vel_y;

	if (g_pos_x < WORLD_LEFT || WORLD_RIGHT <= g_pos_x)
		g_vel_x *= -1;
	if (g_pos_y < WORLD_TOP || WORLD_BOTTOM <= g_pos_y)
		g_vel_y *= -1;
	g_pos_x = CLAMP(g_pos_x, WORLD_LEFT, WORLD_RIGHT);
	g_pos_y = CLAMP(g_pos_y, WORLD_TOP, WORLD_BOTTOM);
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
	Print({ (SHORT)(g_pos_x * SCREEN_RESOLUTION_X), (SHORT)(g_pos_y * SCREEN_RESOLUTION_Y) }, DEFAULT_ATTRIBUTES, /*"**\n**"*/"abcdefg\nhijklmn\ndkuyrgca\nauycgfbag");
	Print({ 12, 22 }, DEFAULT_ATTRIBUTES, "↑↓←→キーで操作");
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
