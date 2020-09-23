/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//player's sprite functions

#define PL_MAX_LIVES		7
#define PL_DEFAULT_LIVES	3

enum Pacman_status_e {
	PACMAN_EATEN = -2,
	PACMAN_GAMEOVER = -1,
	PACMAN_CONTINUE = 0,
	PACMAN_WIN = 1
};
enum Pacman_scores_e {
	PACMAN_POINT_SCORE = 1,
	PACMAN_PPOINT_SCORE = 5,
	PACMAN_GHOST_SCORE = 20
};

#define resetPlayer(s)			do { (s).pos = (s).spawn, (s).nextway = (s).way = WEST; } while(0);
#define playerCanMove(f, y, x)	(validCoord(y, x) && (isCrossable(f, y, x) || isPortal(f, y, x)))
#define isPlayerSpawn(l, i, j)	(i == (l)->player.spawn.y && j == (l)->player.spawn.x)

extern void printPlayer(WINDOW*, const Level*);
extern void playerDeath(WINDOW*, const Level*);

extern Way playerMoves(Coord, Map[MAP_HEIGHT][MAP_WIDTH]);
extern int8_t playerGotCaught(const Level*);

extern short movePlayer(Level*);

//END
