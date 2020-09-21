/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//settings functions for future use, not implemented yet

#define S_SETTINGS	"settings.dat"
#define S_SCORES	"scores.dat"
#define S_DIRPATH	"./arrow-pacman-data/"
#define S_LEVELSDIR	"" S_DIRPATH "levels/"

#define S_MAX_FILE_LIST WIN_HEIGHT

extern void pickLevel(const char*);
extern void createPacmanDir(void);

#include "../src/settings.c"

//END
