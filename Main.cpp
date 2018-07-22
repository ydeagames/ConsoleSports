//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Main.cpp
//!
//! @brief  ゲームプログラムのエントリーポイントのソースファイル
//!
//! @date   2018/07/18
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include <stdio.h>
#include <time.h>
#include "Game\Console.h"
#include "Game\GameMain.h"
#include "Game\BufferedConsole.h"
#include "Game\SpeedTest.h"



// グローバル変数定義 ======================================================

int last_key;			// 前回のキー入力
int current_key;		// 今回のキー入力



// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	last_key = current_key;
	current_key = GetKeyInput();

	return FALSE;
}

// キーチェック
static int CheckHitKey(int keycode)
{
	return current_key == keycode;
}

// 裏画面切り替え
static int ScreenFlip(void)
{
	BufferedConsole_Flush();

	return TRUE;
}

//----------------------------------------------------------------------
//! @brief プログラムのエントリーポイント
//!
//! @retval  0 正常終了
//! @retval -1 異常終了
//----------------------------------------------------------------------
int main(void)
{
	// 初期状態の画面モードの設定
	SetFontSize(SCREEN_FONT_SIZE);
	SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetCursorVisibility(CURSOR_INVISIBLE);

	BufferedConsole_Initialize();

	/*
	// スピードテスト (デバッグ)
	{
		TestSpeed();
		return 0;
	}
	/**/

	// ゲームの処理
	InitializeGame();			// ゲームの初期化処理

	while (!ProcessMessage() && !CheckHitKey(KEY_ESC))
	{
		UpdateGame();			// ゲームの更新処理
		RenderGame();			// ゲームの描画処理

		ScreenFlip();			// 裏画面の内容を表画面に反映
		Clear();				// 裏画面の消去
	}

	FinalizeGame();				// ゲームの終了処理

	return 0;					// 正常終了
}
