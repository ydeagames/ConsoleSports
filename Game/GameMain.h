//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  ゲーム関連のヘッダファイル
//!
//! @date   2018/07/18
//!
//! @author GF1 26 山口 寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once




// 定数の定義 ==============================================================

// <基本型> ------------------------------------------------------------

// 真偽値、真偽型定義
#define TRUE 1
#define FALSE 0
typedef int BOOL;


// <システム> ----------------------------------------------------------

#define GAME_TITLE "Sample Game"				// ゲームタイトル


// <画面> --------------------------------------------------------------

#define DEFAULT_SCREEN_WIDTH		(120)																	// デフォルトの画面の高さ[em]
#define DEFAULT_SCREEN_HEIGHT		(30)																	// デフォルトの画面の幅[em]
#define DEFAULT_SCREEN_FONT_SIZE	(16)																	// デフォルトの文字の大きさ[pt]

#define SCREEN_FONT_SIZE			(5)																		// 画面の高さ[em]
#define SCREEN_WIDTH				(DEFAULT_SCREEN_WIDTH * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// 画面の幅[em]
#define SCREEN_HEIGHT				(DEFAULT_SCREEN_HEIGHT * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// 文字の大きさ[pt]

#define SCREEN_TOP					(0)																		// 画面の上端
#define SCREEN_BOTTOM				(SCREEN_HEIGHT)															// 画面の下端
#define SCREEN_LEFT					(0)																		// 画面の左端
#define SCREEN_RIGHT				(SCREEN_WIDTH)															// 画面の右端

#define SCREEN_CENTER_X				(SCREEN_WIDTH  / 2)														// 画面の中央(X座標)
#define SCREEN_CENTER_Y				(SCREEN_HEIGHT / 2)														// 画面の中央(Y座標)




// 関数の宣言 ==============================================================

// ゲームの初期化処理
void InitializeGame(void);

// ゲームの更新処理
void UpdateGame(void);

// ゲームの描画処理
void RenderGame(void);

// ゲームの終了処理
void FinalizeGame(void);
