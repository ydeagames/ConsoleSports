//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  �Q�[���֘A�̃w�b�_�t�@�C��
//!
//! @date   2018/07/18
//!
//! @author GF1 26 �R�� ����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once




// �萔�̒�` ==============================================================

// <��{�^> ------------------------------------------------------------

// �^�U�l�A�^�U�^��`
#define TRUE	1								// �^
#define FALSE	0								// �U
typedef int		BOOL;							// �^�U�l

// <���[�e�B���e�B> ----------------------------------------------------

// �ő�l�A�ŏ��l
#define MAX(a, b)	(a > b ? a : b)				// �ő�l
#define MIN(a, b)	(a < b ? a : b)				// �ŏ��l

// <�V�X�e��> ----------------------------------------------------------

#define GAME_TITLE "Sample Game"				// �Q�[���^�C�g��


// <���> --------------------------------------------------------------

#define DEFAULT_SCREEN_WIDTH		(120)																	// �f�t�H���g�̉�ʂ̍���[em]
#define DEFAULT_SCREEN_HEIGHT		(30)																	// �f�t�H���g�̉�ʂ̕�[em]
#define DEFAULT_SCREEN_FONT_SIZE	(16)																	// �f�t�H���g�̕����̑傫��[pt]

#define SCREEN_FONT_SIZE			(7)																		// �����̑傫��[pt]
#define SCREEN_WIDTH				(DEFAULT_SCREEN_WIDTH * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// ��ʂ̕�[em]
#define SCREEN_HEIGHT				(DEFAULT_SCREEN_HEIGHT * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// ��ʂ̍���[em]

#define SCREEN_TOP					(0)																		// ��ʂ̏�[
#define SCREEN_BOTTOM				(SCREEN_HEIGHT - SCREEN_TOP)											// ��ʂ̉��[
#define SCREEN_LEFT					(0)																		// ��ʂ̍��[
#define SCREEN_RIGHT				(SCREEN_WIDTH - SCREEN_LEFT)											// ��ʂ̉E�[

#define SCREEN_CENTER_X				(SCREEN_WIDTH  / 2)														// ��ʂ̒���(X���W)
#define SCREEN_CENTER_Y				(SCREEN_HEIGHT / 2)														// ��ʂ̒���(Y���W)




// �O���[�o���ϐ��̐錾 ====================================================

// TODO InputManager�쐬
extern int last_key;
extern int current_key;




// �֐��̐錾 ==============================================================

// �Q�[���̏���������
void InitializeGame(void);

// �Q�[���̍X�V����
void UpdateGame(void);

// �Q�[���̕`�揈��
void RenderGame(void);

// �Q�[���̏I������
void FinalizeGame(void);
