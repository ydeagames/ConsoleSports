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

typedef short SHORT;

// 構造体の定義 ============================================================

typedef struct {
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

typedef struct {
	ConsoleColor foreground;
	ConsoleColor background;
} ATTR;

// グローバル変数宣言 ======================================================

extern const ATTR DEFAULT_ATTR;

// 関数の宣言 ==============================================================

ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background);

void BufferedConsole_Initialize(void);

void Clear(void);

void Print(COORD coord, ATTR attributes, const char* format);

void BufferedConsole_Flush(void);
