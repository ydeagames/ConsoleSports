#include "CXLib.h"
#include "Game.h"
#include "BufferedConsole.h"
#include <string.h>
#include <math.h>
#include "GameUtils.h"

// ���F�h��Ԃ�
const ATTR ATTR_WHITE = CreateATTR(COLOR_BLACK, COLOR_WHITE);

// ���[���h���W���R���\�[�����W�ɕϊ�
float WorldToConsoleXF(float world_x, float size)
{
	return world_x * (SCREEN_RESOLUTION_X * size);
}

// ���[���h���W���R���\�[�����W�ɕϊ�
float WorldToConsoleYF(float world_y, float size)
{
	return world_y * (SCREEN_RESOLUTION_Y * size);
}

// ���[���h���W���R���\�[�����W�ɕϊ�
SHORT WorldToConsoleX(float world_x, float size)
{
	return (SHORT)WorldToConsoleXF(world_x, size);
}

// ���[���h���W���R���\�[�����W�ɕϊ�
SHORT WorldToConsoleY(float world_y, float size)
{
	return (SHORT)WorldToConsoleYF(world_y, size);
}

// �R���\�[�����W�����[���h���W�ɕϊ�
float ConsoleToWorldXF(float screen_x, float size)
{
	return screen_x / (SCREEN_RESOLUTION_X * size);
}

// �R���\�[�����W�����[���h���W�ɕϊ�
float ConsoleToWorldYF(float screen_y, float size)
{
	return screen_y / (SCREEN_RESOLUTION_Y * size);
}

// �R���\�[�����W�����[���h���W�ɕϊ�
float ConsoleToWorldX(SHORT screen_x, float size)
{
	return ConsoleToWorldXF((float)screen_x, size);
}

// �R���\�[�����W�����[���h���W�ɕϊ�
float ConsoleToWorldY(SHORT screen_y, float size)
{
	return ConsoleToWorldYF((float)screen_y, size);
}

// ��`�`��֐�
void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str)
{
	// ���[���h���W���R���\�[�����W�ɕϊ�
	SHORT cx1 = WorldToConsoleX(x1);
	SHORT cy1 = WorldToConsoleY(y1);
	SHORT cx2 = WorldToConsoleX(x2);
	SHORT cy2 = WorldToConsoleY(y2);

	// �����̒���
	int str_len = GetMax(1, (int)strlen(Str));

	// Y���[�v
	SHORT ix, iy;
	for (iy = cy1; iy <= cy2; iy++)
	{
		// X���[�v
		for (ix = cx1; ix <= cx2; ix += str_len)
		{
			// �h��Ԃ����A�܂��͉��̏ꍇ�`��
			if (FillFlag || (ix == cx1 || ix == cx2) || (iy == cy1 || iy == cy2))
				Print({ ix, iy }, Color, Str);
		}
	}
}

// �ȉ~�`��֐�
void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str)
{
	// ���[���h���W���R���\�[�����W�ɕϊ�
	SHORT cx = WorldToConsoleX(x);
	SHORT cy = WorldToConsoleY(y);
	SHORT crx = WorldToConsoleX(rx);
	SHORT cry = WorldToConsoleY(ry);
	float crxf = WorldToConsoleXF(rx);
	float cryf = WorldToConsoleYF(ry);

	// �����̒���
	int str_len = GetMax(1, (int)strlen(Str));

	// Y���[�v
	SHORT ix, iy;
	for (iy = -cry; iy <= cry; iy++)
	{
		// X���[�v
		for (ix = -crx; ix <= crx; ix += str_len)
		{
			// �h��Ԃ����A�܂��͉��̏ꍇ�`��
			float p = (ix*ix) / (crxf*crxf) + (iy*iy) / (cryf*cryf) - 1;
			if (FillFlag ? p <= .08f : -.25f < p && p < .08f)
				Print({ cx + ix, cy + iy }, Color, Str);
		}
	}
}

// �~�`��֐�
void DrawCircle(float x, float y, float r, ATTR Color, int FillFlag, const char* Str)
{
	// �ȉ~�`��
	DrawOval(x, y, r, r, Color, FillFlag, Str);
}

// ���`��֐�
void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str)
{
	// ���[���h���W���R���\�[�����W�ɕϊ�
	int cx1 = (int)WorldToConsoleX(x1);
	int cy1 = (int)WorldToConsoleY(y1);
	int cx2 = (int)WorldToConsoleX(x2);
	int cy2 = (int)WorldToConsoleY(y2);

	// �c�A���A�ǂ���̔䂪�傫����
	BOOL steep = GetAbs(cy2 - cy1) > GetAbs(cx2 - cx1);
	// �c���傫��������c�����]
	if (steep)
	{
		Swap(&cx1, &cy1);
		Swap(&cx2, &cy2);
	}

	{
		// �����擾
		int deltax = GetAbs(cx2 - cx1);
		int deltay = GetAbs(cy2 - cy1);
		// �Y�����v�Z
		int error = deltax / 2;

		// �C���N�������g
		int incx = (cx1 < cx2) ? 1 : -1;
		int incy = (cy1 < cy2) ? 1 : -1;

		// X���[�v
		SHORT x;
		SHORT y = cy1;
		for (x = cx1; x != cx2; x += incx)
		{
			// �`��
			Print({ steep ? y : x, steep ? x : y }, Color, Str);

			// �Y�����C��&�`�F�b�N
			if ((error -= deltay) < 0)
			{
				y += incy;
				error += deltax;
			}
		}
	}
}

// �j����`��
void DrawDashedLine(float x1, float y1, float x2, float y2, ATTR color, float length, const char* Str)
{
	// ��
	float vx = x2 - x1;
	float vy = y2 - y1;

	// �p�x
	float angle = atan2f(vy, vx);
	// �p�x��K�p������
	float dx = length*cosf(angle);
	float dy = length*sinf(angle);

	// �_���̐�
	int count = (int)(sqrtf(vx * vx + vy * vy) / length);

	// �������W
	float x = x1;
	float y = y1;
	
	// ���������J��Ԃ�
	int i;
	for (i = 0; i < count; i += 2)
	{
		// ����`��
		DrawLine(x, y, (x + dx), (y + dy), color, Str);
		// ���W���X�V
		x += dx * 2;
		y += dy * 2;
	}

	// �Ō�A�����ŏI�������
	if (count % 2 == 0)
	{
		DrawLine(x, y, x2, y2, color, Str);
	}
}
