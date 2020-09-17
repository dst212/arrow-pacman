/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//defines, typedefs and some essential functions I didn't know where to put

#define DEFAULT_COLOR		0x0B	//black background and yellow foreground
#define DEFAULT_COLOR_ERROR	0x09	//black background and red foreground

#define MAX_STR_SIZE 32 //add + 1 to declaration, to leave space for '\0'
#define PACMAN_FILE_CHARS "AZaz09\r .,-+_!'"

//MAP

typedef struct {
	int16_t x;
	int16_t y;
} coord;

#define isSamePos(c1, c2) ((c1).x == (c2).x && (c1).y == (c2).y)

#define MAP_HEIGHT WIN_HEIGHT
#define MAP_WIDTH 79

typedef struct {
	wchar_t wall; //appearance
	uint8_t color;
	coord portal; //coordinates which the map's cell leads to
} map_t;

//SPRITES

#define WEST	0b0001	//left
#define NORTH	0b0010	//up
#define EAST	0b0100	//right
#define SOUTH	0b1000	//down

#define INDEX_WAY(way) ( (way) ? ((int)log2((float)way)) : 0 )

typedef uint8_t way_t;

#define turnAround(way)	((way & 0b1100) ? (way) >> 2 : (way) << 2)
#define turnLeft(way)	((way & 0b0001) ? (way) << 3 : (way) >> 1)
#define turnRight(way)	((way & 0b1000) ? (way) >> 3 : (way) << 1)
#define shiftSprite(s, way) do { \
	switch(way) { \
		case WEST: \
			(s).pos.x--; \
			break; \
		case EAST: \
			(s).pos.x++; \
			break; \
		case NORTH: \
			(s).pos.y--; \
			break; \
		case SOUTH: \
			(s).pos.y++; \
			break; \
	} \
} while(0);

way_t randomMove(way_t moves, way_t way) {
	//for each of these if, the sprite MIGHT go back, but it won't until it's the last choice
	way_t left = turnLeft(way), right = turnRight(way);
	if(moves & way) {
		if(moves & left) {
			if(moves & right) { //it can go on, turn left or turn right
				way = randomChoice(3, way, left, right);
			} else { //it can go on or turn left
				way = randomChoice(2, way, left);
			}
		} else if(moves & right) { //it can go on or turn right
			way = randomChoice(2, way, right);
		}/* else { //it can only go on
			way = way;
		}*/
	} else {
		if(moves & left) {
			if(moves & right) { //it can turn left or turn right
				way = randomChoice(2, left, right);
			} else { //it can only turn left
				way = left;
			}
		} else if(moves & right) { //it can only turn right
			way = right;
		} else { //it can only go back
			way = turnAround(way);
		}
	}
	return way;
}

//PLAYER

typedef struct {
	//shouldn't be changed during the game:
	wchar_t s[4];			//appearance (left (0), up (1), right (2), down (3))
	uint8_t color;			//
	wchar_t deathanim[4];	//death's animation
	coord spawn;			//spawn coordinates
	//can be changed:
	coord pos;				//current coordinates
	way_t way;				//current way
	way_t nextway;			//queueing (in line) way
	int8_t lives;			//
	uint32_t score;			//
} player_t;

//GHOSTS

#define GH_NORMAL		0
#define GH_SCARED		1
#define GH_EATEN		2
#define GH_HUNTING		3	//the ghost found the pacman and is trying to reach it
#define GH_STATUSES		4
#define GH_NAME_SIZE	16

#define LVL_MAX_GHOSTS 8

typedef struct {
	//shouldn't be changed during the game:
	char name[GH_NAME_SIZE + 1];	//ghost's name. Will appear when player is eaten
	wchar_t s[GH_STATUSES][4];		//appearance (one per status)
	uint8_t color[GH_STATUSES];		//color, one per status
	int8_t ai;					/*
	^ uses shortest path or not to reach the pacman:
	When value is:
	- 0: it is stupid and blind
	- more than 0: it tries to reach it trough the shortest path (.ai is the
	"ray of sight" of the ghost)
	- less than 0: it tries to reach pacman by x or y match (.ai absolute value
	is the maximum distance which triggers the ghost)

	Yeah, I know it's not an AI at all, but it was just to have something quick
	to type and "smartness" or "algorithm" weren't an option. Plus "AI" is cool.

	Actually AI > 0 is not implemented yet, so ghosts randomly chase the PacMan,
	just following coordinates. So if there is a wall between the ghost and the
	PacMan, the ghost will repeatedly bang its head on the wall.
	*/
	int8_t ticks;				//ticks after which the ghost can bypass walls
	coord spawn;				//spawn coordinates
	//can be changed:
	coord pos;					//current coordinates
	way_t way;					//current way
	int8_t flag;				//ghost status: normal (0), scared (1), eaten (2)
} ghost_t;

//LEVEL

typedef struct {
	int status;
	coord data; //where's the problem
} levelStatus;

typedef struct {
	short lives;
	short powerticks;
} level_bak; //backup struct for level

typedef struct {
	map_t map[MAP_HEIGHT][MAP_WIDTH];
	player_t player;
	ghost_t ghost[LVL_MAX_GHOSTS];
	int8_t ghosts;		//number of ghosts
	short powerticks;	//power-point duration in ticks
	//fields which are not saved when a new level is created they'll change during the game
	int delay;			//delay between "frames"
	int levels;			//
	int ticks;			//count of frames, one tick is one frame
	int checkpoint;		//checkpoint for powered points
	level_bak initial;	//initial (backup) values
	int8_t i;			//the ghost which has eaten the pacman
} level;

//END
