#include "SpeedTest.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "GameMain.h"
#include "Console.h"

// スピードテスト
#define TIMER_START(C)			(C = clock())
#define TIMER_STOP(C)			(C = clock() - C)
#define TIMER_RESULT(C, NAME)	printf(#NAME"("#C")の速度: %d\n", (int)C)

void TestSpeed(void)
{
	int ix, iy;
	clock_t c1, c2, c3, c4, c5;

	char str[SCREEN_WIDTH*SCREEN_HEIGHT + 1];

	// for文の速度
	TIMER_START(c1);
	{
		for (iy = 0; iy < SCREEN_HEIGHT; iy++)
		{
			for (ix = 0; ix < SCREEN_WIDTH; ix++)
			{
				str[iy*SCREEN_WIDTH + ix] = 'x';
			}
		}
		str[SCREEN_WIDTH*SCREEN_HEIGHT] = '\0';
	}
	TIMER_STOP(c1);

	// 複数行printfの速度
	TIMER_START(c2);
	{
		SetCursorPosition(0, 0);
		printf(str);
	}
	TIMER_STOP(c2);

	// 複数回printfの速度
	TIMER_START(c3);
	{
		SetCursorPosition(0, 0);
		for (iy = 0; iy < SCREEN_HEIGHT; iy++)
		{
			for (ix = 0; ix < SCREEN_WIDTH; ix++)
			{
				printf("x");
			}
		}
	}
	TIMER_STOP(c3);

	// SetCursorPositionの速度
	TIMER_START(c4);
	{
		SetCursorPosition(0, 0);
		for (iy = 0; iy < SCREEN_HEIGHT; iy++)
		{
			for (ix = 0; ix < SCREEN_WIDTH; ix++)
			{
				SetCursorPosition(ix, iy);
			}
		}
	}
	TIMER_STOP(c4);

	// SetColor系の速度
	TIMER_START(c5);
	{
		SetCursorPosition(0, 0);
		for (iy = 0; iy < SCREEN_HEIGHT; iy++)
		{
			for (ix = 0; ix < SCREEN_WIDTH; ix++)
			{
				SetTextColor(COLOR_WHITE);
				SetBackColor(COLOR_BLACK);
			}
		}
	}
	TIMER_STOP(c5);

	ClearScreen();
	TIMER_RESULT(c1, "for文の速度");
	TIMER_RESULT(c2, "複数行printfの速度");
	TIMER_RESULT(c3, "複数回printfの速度");
	TIMER_RESULT(c4, "SetCursorPositionの速度");
	TIMER_RESULT(c5, "SetColor系の速度");
}
