//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   BufferedConsole.cpp
//!
//! @brief  �\�t�g�E�F�A�E�R���\�[���o�b�t�@
//!
//! @date   2018/07/29
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================

#include "BufferedConsole.h"
#include "Game.h"
#include <stdio.h>
#include <string.h>
#include "Console.h"
#include "GameUtils.h"

// �\���̂̒�` ============================================================

// ������I�u�W�F�N�g
typedef struct {
	// ������
	const char* str;
	// ����
	SHORT size;
} STR;

// �s�N�Z�����
typedef struct {
	// ������I�u�W�F�N�g
	STR str;
	// ����(�F)
	ATTR attributes;
} PIXEL, *PPIXEL;

// �֐��̐錾 ==============================================================

// ������I�u�W�F�N�g���쐬
static STR CreateSTR(const char* str);

// �O���[�o���ϐ��̒�` ====================================================

// �f�t�H���g�̑���(�F)
const ATTR DEFAULT_ATTR = CreateATTR(COLOR_WHITE, COLOR_BLACK);
// �f�t�H���g�̕�����I�u�W�F�N�g
const STR DEFAULT_STR = CreateSTR(" ");

// �f�t�H���g�̃s�N�Z�����
static const PIXEL default_pixel = { CreateSTR(" "), CreateATTR(COLOR_WHITE, COLOR_BLACK) };

// ���ݕ\������Ă�����
static PIXEL buffer1[SCREEN_HEIGHT*SCREEN_WIDTH];
// ���\��������
static PIXEL buffer2[SCREEN_HEIGHT*SCREEN_WIDTH];

// �Ō�ɕ`�悵���ʒu
static COORD last_coord;
// �Ō�ɕ`�悵������(�F)
static ATTR last_attributes;

// �֐��̒�` ==============================================================

// ����(�F)���쐬
ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background)
{
	return{ foreground, background };
}

// ������I�u�W�F�N�g���쐬
static STR CreateSTR(const char* str)
{
	return{ str, (SHORT)strlen(str) };
}

// �\�t�g�E�F�A�E�R���\�[���o�b�t�@������������
void BufferedConsole_Initialize(void)
{
	// �X�N���[���A�I�t�X�N���[�����f�t�H���g�s�N�Z���ŏ�����
	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		buffer1[i] = default_pixel;
		buffer2[i] = default_pixel;
	}

	// �Ō�ɕ`�悵���ʒu���X�V
	last_coord = { 0, 0 };
	// �Ō�ɕ`�悵������(�F)���X�V
	last_attributes = DEFAULT_ATTR;
}

// ���ݕ\������Ă�����
static PPIXEL GetScreen(void)
{
	return buffer1;
}

// ���\��������
static PPIXEL GetOffScreen(void)
{
	return buffer2;
}

// �s�N�Z�����擾
static PPIXEL GetPixel(const PPIXEL screen, COORD coord)
{
	return &screen[coord.Y*SCREEN_WIDTH + coord.X];
}

// ��ʂ�����������
void Clear(void)
{
	PPIXEL screen = GetOffScreen();

	// �I�t�X�N���[�����f�t�H���g�s�N�Z���ŏ�����
	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		screen[i] = default_pixel;
	}
}

// �w����W�Ɏw�蕶�����w�葮��(�F)�ŕ\��
void Print(COORD coord, ATTR attributes, const char* format)
{
	// �E����荶��Ȃ�
	if (coord.X < SCREEN_RIGHT && coord.Y < SCREEN_BOTTOM)
	{
		// �I�t�X�N���[����
		PPIXEL screen = GetOffScreen();

		// ���ɃI�[�o�[���Ă��鋗��
		SHORT negative = GetMax(0, -coord.X);

		// Y���[�v
		SHORT iy;
		for (iy = coord.Y; iy < SCREEN_HEIGHT; iy++)
		{
			// ���s�܂ł̋���
			const char* enter = strchr(format, '\n');

			// �����̒���
			SHORT size;
			// ���s���Ȃ����\0�܂ŁA�����łȂ���Ή��s�܂ł̒���
			if (enter == NULL)
				size = (SHORT)strlen(format);
			else
				size = (SHORT)(enter - format);

			// ���Ə�ɃI�[�o�[�������Č����Ȃ��Ȃ��Ă��Ȃ����`�F�b�N
			if (size - negative > 0 && iy >= SCREEN_TOP)
			{
				// ���߂荞�ݏC��
				SHORT i;
				for (i = 0; i < size - negative; i++)
				{
					// ���߂荞�ݍ��W
					SHORT neg = negative + i;
					// �����̕`����WX
					SHORT ix = coord.X + neg;
					// �`�悷�ׂ�������
					SHORT width = (iy >= SCREEN_BOTTOM - 1) ? SCREEN_WIDTH - 1 : SCREEN_WIDTH;
					// �����̕`����WX���`�悷�ׂ���������菬������Ε`��
					if (ix < width)
						*GetPixel(screen, { ix, iy }) = { {format + neg, (SHORT)GetMin(size - neg, width - ix) }, attributes };
				}
			}

			// ���s���Ȃ���΂����ŏI��
			if (enter == NULL)
				return;
			// ���s������Ή��s�̂��Ƃ܂ŃJ�[�\�������炵�đ��s
			else
				format = enter + 1;
		}
	}
}

// �s�N�Z���𔽉f
static void FlushPixel(COORD coord, PPIXEL pixel_before, PPIXEL pixel_after)
{
	// �X�V�����邩
	BOOL modified = FALSE;
	// �w�i�F�A�e�L�X�g�F�A�����̂����ꂩ���X�V����Ă��Ȃ���
	if (pixel_before->attributes.background != pixel_after->attributes.background ||
		pixel_before->attributes.foreground != pixel_after->attributes.foreground ||
		pixel_before->str.size != pixel_after->str.size || strcmp(pixel_before->str.str, pixel_after->str.str) != 0)
		modified = TRUE;

	// �X�V����Ă����ꍇ
	if (modified)
	{
		// ���W���A�����Ă��Ȃ��ꍇ�͍��W���Z�b�g
		if (coord.X != last_coord.X || coord.Y != last_coord.Y)
			SetCursorPosition(coord.X, coord.Y);
		// �Ō�ɕ`�悵���w�i�F���X�V����Ă�����w�i�F��ύX����
		if (last_attributes.background != pixel_after->attributes.background)
		{
			SetBackColor(pixel_after->attributes.background);
			last_attributes.background = pixel_after->attributes.background;
		}
		// �Ō�ɕ`�悵���e�L�X�g�F���X�V����Ă�����e�L�X�g�F��ύX����
		if (last_attributes.foreground != pixel_after->attributes.foreground)
		{
			SetTextColor(pixel_after->attributes.foreground);
			last_attributes.foreground = pixel_after->attributes.foreground;
		}
		// �������w�肵�������ŕ`�悷��
		printf("%.*s", pixel_after->str.size, pixel_after->str.str);
		// ���̍��W���Z�b�g����(�Ō�ɕ`�悵�����W+1�̍��W)
		last_coord = { coord.X + pixel_after->str.size, coord.Y };

		// �X�N���[���̏����I�t�X�N���[���̏��ōX�V
		{
			int i;
			for (i = 0; i < pixel_after->str.size; i++)
				pixel_before[i] = pixel_after[i];
		}
	}
}

// �X�N���[���𔽉f
void BufferedConsole_Flush(void)
{
	// ���ݕ\������Ă�����
	PPIXEL screen_before = GetScreen();
	// ���\��������
	PPIXEL screen_after = GetOffScreen();

	// Y���[�v
	SHORT ix, iy;
	for (iy = 0; iy < SCREEN_HEIGHT; iy++)
	{
		// X���[�v
		for (ix = 0; ix < SCREEN_WIDTH;)
		{
			// ���݂̍��W
			COORD coord = { ix, iy };

			// ���ݕ\������Ă���s�N�Z��
			PPIXEL pixel_before = GetPixel(screen_before, coord);
			// ���\������s�N�Z��
			PPIXEL pixel_after = GetPixel(screen_after, coord);

			// �s�N�Z���𔽉f
			FlushPixel(coord, pixel_before, pixel_after);

			// �����̒������J�[�\����i�߂�
			ix += GetMax(1, pixel_after->str.size);
		}
	}
}
