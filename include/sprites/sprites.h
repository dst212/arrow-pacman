/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

/*
* Ghosts and the pacman have different ways to move around the map: ghosts can
* bypass some walls, while pacman can travel portals.
* Ghosts have different status too (normal, scared ...), while pacman is always
* the same.
* As a result they need different structures (see def.h) and functions for each
* operation.
*/

extern int8_t ghostSpawn(const Level*, const short, const short);
extern int8_t ghostPos(const Level*, const short, const short);
extern int8_t ghostHit(const Level*, const short, const short, const Way);

extern void printSprites(WINDOW*, const Level*);
#define isSpawn(l, i, j) (ghostSpawn(l, i, j) != -1 || isPlayerSpawn(l, i, j))

//END
