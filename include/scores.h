/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//scores functions, not implemented yet

typedef struct {
	char name[MAX_STR_SIZE + 1];
	int score;
} score;

extern void listScores(void);

#include "../src/scores.c"

//END
