/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//functions about the level's struct

extern void printLevel(WINDOW*, const Level*, const short, const short, const short, const short);

extern void levelUp(Level*l);
extern void initLevel(Level*l);
extern void restartLevel(Level*l);

extern void beginPowerPoint(Level*l);
extern void endPowerPoint(Level*l);

//END
