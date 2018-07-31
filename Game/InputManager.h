#pragma once
#include "Game.h"

// �֐��̒�` ==============================================================

// �L�[�X�V
void UpdateInputManager(void);

// ���L�[��������Ă��邩
BOOL IsRawKeyDown(int key);

// ���L�[��������Ă��邩
BOOL IsRawKeyUp(int key);

// ���L�[�����������ォ
BOOL IsRawKeyPressed(int key);

// ���L�[�𗣂������ォ
BOOL IsRawKeyReleased(int key);

// �L�[��������Ă��邩
BOOL IsKeyDown(int key);

// �L�[��������Ă��邩
BOOL IsKeyUp(int key);

// �L�[�����������ォ
BOOL IsKeyPressed(int key);

// �L�[�𗣂������ォ
BOOL IsKeyReleased(int key);
