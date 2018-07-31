#pragma once
#include "Game.h"

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void);

// 生キーが押されているか
BOOL IsRawKeyDown(int key);

// 生キーが離されているか
BOOL IsRawKeyUp(int key);

// 生キーを押した直後か
BOOL IsRawKeyPressed(int key);

// 生キーを離した直後か
BOOL IsRawKeyReleased(int key);

// キーが押されているか
BOOL IsKeyDown(int key);

// キーが離されているか
BOOL IsKeyUp(int key);

// キーを押した直後か
BOOL IsKeyPressed(int key);

// キーを離した直後か
BOOL IsKeyReleased(int key);
