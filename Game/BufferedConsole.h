//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   BufferedConsole.h
//!
//! @brief  ソフトウェア・コンソールバッファ
//!
//! @date   2018/07/29
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================

#include "Console.h"

// 型の定義 ================================================================

// コンソール座標の要素
typedef short SHORT;

// 構造体の定義 ============================================================

// コンソール座標
typedef struct {
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

// 属性(色)
typedef struct {
	ConsoleColor foreground;
	ConsoleColor background;
} ATTR;

// グローバル変数宣言 ======================================================

// デフォルト属性(色)
extern const ATTR DEFAULT_ATTR;

// 関数の宣言 ==============================================================

// 属性(色)を作成
ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background);

// ソフトウェア・コンソールバッファを初期化する
void BufferedConsole_Initialize(void);

// 画面を初期化する
void Clear(void);

// 指定座標に指定文字を指定属性(色)で表示
void Print(COORD coord, ATTR attributes, const char* format);

// スクリーンを反映
void BufferedConsole_Flush(void);
