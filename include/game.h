/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//main game functions, the game "engine"

extern int pacmanCountDown(WINDOW*);
extern void refreshBar(WINDOW*, const Level*);
extern void pacmanKeys(WINDOW*, Level*, int);
extern void startPacMan(void);

#include "../src/game.c"

//END
