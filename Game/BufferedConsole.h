#pragma once
#include "Console.h"

typedef short SHORT;

typedef struct {
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

typedef struct {
	ConsoleColor foreground;
	ConsoleColor background;
} ATTR;

extern const ATTR DEFAULT_ATTRIBUTES;

void BufferedConsole_Initialize(void);

void Clear(void);

void Print(COORD coord, ATTR attributes, const char* format);

void BufferedConsole_Flush(void);
