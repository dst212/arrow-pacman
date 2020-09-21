/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//defines, typedefs and enumerations

#define DEFAULT_COLOR		0x0B	//black background and yellow foreground
#define DEFAULT_COLOR_ERROR	0x09	//black background and red foreground

#define MAX_STR_SIZE 32 //add + 1 to declaration, to leave space for '\0'
#define PACMAN_FILE_CHARS "AZaz09\r .,-+_!'" //allowed characters to be used in a file name

//MAP

typedef struct {
	int16_t x;
	int16_t y;
} Coord;

#define isSamePos(c1, c2) ((c1).x == (c2).x && (c1).y == (c2).y)

#define MAP_HEIGHT WIN_HEIGHT
#define MAP_WIDTH 79

typedef struct {
	wchar_t wall; //appearance
	uint8_t color;
	Coord portal; //coordinates which the map's cell leads to
} Map;

//SPRITES

enum Way_e {
	WEST	= 0b0001,	//left
	NORTH	= 0b0010,	//up
	EAST	= 0b0100,	//right
	SOUTH	= 0b1000	//down
};

#define INDEX_WAY(way) ( (way) ? ((int)log2((float)way)) : 0 )

typedef uint8_t Way;

//PLAYER

typedef struct {
	//shouldn't be changed during the game:
	wchar_t s[4];			//appearance (left (0), up (1), right (2), down (3))
	uint8_t color;			//
	wchar_t deathanim[4];	//death's animation
	Coord spawn;			//spawn coordinates
	//can be changed:
	Coord pos;				//current coordinates
	Way way;				//current way
	Way nextway;			//queueing (in line) way
	int8_t lives;			//
	uint32_t score;			//
} Player;

//GHOSTS

enum Ghost_e {
	GH_NORMAL	= 0,
	GH_SCARED	= 1,
	GH_EATEN	= 2,
	GH_HUNTING	= 3,	//the ghost found the pacman and is trying to reach it
	GH_STATUSES	= 4
};

#define GH_NAME_SIZE	16
#define LVL_MAX_GHOSTS	8

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
	Coord spawn;				//spawn coordinates
	//can be changed:
	Coord pos;					//current coordinates
	Way way;					//current way
	int8_t flag;				//ghost status: normal (0), scared (1), eaten (2)
} Ghost;

//LEVEL

typedef struct {
	int status;
	Coord data; //where's the problem
} Level_status;

typedef struct {
	short lives;
	short powerticks;
} Level_bak; //backup struct for level

typedef struct {
	Map map[MAP_HEIGHT][MAP_WIDTH];
	Player player;
	Ghost ghost[LVL_MAX_GHOSTS];
	int8_t ghosts;		//number of ghosts
	short powerticks;	//power-point duration in ticks
	//fields which are not saved when a new level is created they'll change during the game
	int delay;			//delay between "frames"
	int levels;			//
	int ticks;			//count of frames, one tick is one frame
	int checkpoint;		//checkpoint for powered points
	Level_bak initial;	//initial (backup) values
	int8_t i;			//the ghost which has eaten the pacman
} Level;

//END
