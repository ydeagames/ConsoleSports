#include "GameUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>

// 整数aとbを入れ替え
void Swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// 小数aとbを入れ替え
void SwapF(float* a, float* b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

// 0≦x≦max の整数ランダム生成
int GetRand(int max)
{
	return rand() % (max + 1);
}

// 0.0≦x≦max の小数ランダム生成
float GetRandF(float max)
{
	return GetRandomF() * max;
}

// 0.0≦x≦1.0 の小数ランダム生成
float GetRandomF(void)
{
	return (float)GetRand(RAND_MAX) / RAND_MAX;
}

// min≦x≦max の整数ランダム生成
int GetRandRange(int min, int max)
{
	if (max < min)
		Swap(&min, &max);
	return GetRand(max - min) + min;
}

// min≦x≦max の小数ランダム生成
float GetRandRangeF(float min, float max)
{
	if (max < min)
		SwapF(&min, &max);
	return GetRandF(max - min) + min;
}

// 0〜w の整数ループ
int GetLoop(int x, int w)
{
	return ((x % w) + w) % w;
}

// 0〜w の小数ループ
float GetLoopF(float x, float w)
{
	return fmodf((fmodf(x, w) + w), w);
}

// min〜max の整数ループ
int GetLoopRange(int x, int min, int max)
{
	if (max < min)
		Swap(&min, &max);
	return GetLoop(x - min, max - min) + min;
}

// min〜max の小数ループ
float GetLoopRangeF(float x, float min, float max)
{
	if (max < min)
		SwapF(&min, &max);
	return GetLoopF(x - min, max - min) + min;
}

// 整数絶対値
int GetAbs(int a)
{
	return a > 0 ? a : -a;
}

// 小数絶対値
float GetAbsF(float a)
{
	return a > 0 ? a : -a;
}

// 整数最小値
int GetMin(int a, int b)
{
	return a < b ? a : b;
}

// 小数最小値
float GetMinF(float a, float b)
{
	return a < b ? a : b;
}

// 整数最大値
int GetMax(int a, int b)
{
	return a > b ? a : b;
}

// 整数最大値
float GetMaxF(float a, float b)
{
	return a > b ? a : b;
}

// 整数を範囲内に収める
int GetClamp(int x, int min, int max)
{
	return GetMin(GetMax(x, min), max);
}

// 小数を範囲内に収める
float ClampF(float x, float min, float max)
{
	return GetMinF(GetMaxF(x, min), max);
}

// 角度の単位変換([度]→[ラジアン])
float ToRadians(float degree)
{
	return degree * (float)M_PI / 180.0f;
}

// 角度の単位変換([ラジアン]→[度])
float ToDegrees(float radian)
{
	return radian * 180.0f / (float)M_PI;
}

// 0〜maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentage(float x, float max)
{
	float n = ClampF(x, 0, max);
	return n / max;
}

// min〜maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentageRange(float x, float min, float max)
{
	if (max < min)
	{
		x = max - x + min;
		SwapF(&min, &max);
	}
	return GetPercentage(x - min, max - min);
}

// 0〜maxの範囲でのpercent%の位置
float GetPercentValue(float percent, float max)
{
	return max * percent;
}

// min〜maxの範囲でのpercent%の位置
float GetPercentValueRange(float percent, float min, float max)
{
	if (max < min)
	{
		percent = 1 - percent;
		SwapF(&min, &max);
	}
	return GetPercentValue(percent, max - min) + min;
}
