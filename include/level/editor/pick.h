/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//pick-something "windows"

extern void levelPickSpawnpoint(WINDOW*, Level*, short*, short*, Coord*);
extern void levelPlacePortal(WINDOW*, Level*);
extern bool levelPickColor(WINDOW*, const short, const short, uint8_t*);
extern void levelPickWall(WINDOW*, Level*, const short, const short);
extern void levelPickSymbol(WINDOW*, wchar_t*, const uint8_t);
extern void levelChangeSprites(WINDOW*, const short, const short, const int, const int, wchar_t[][4], uint8_t[], const int);

// #include "../../../src/level/editor/pick.c"

//END
