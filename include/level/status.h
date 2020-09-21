/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//functions to avoid errors while loading levels

/*
* Here below I decided to use 2's powers to identify errors.
* This is because a level may have more than one problem, and keep track of
* every single error OR-ing them is the easiest way.
*
* Note: actually I did not use this "bitmap-flags" to identify errors, but it
* could be useful in future.
*/

enum Level_status_returns_e {
	LVL_OK					= 0,
	LVL_OLD					= (1<<0),
	LVL_INVALID				= (1<<1),
	LVL_NOT_FOUND			= (1<<2),
	LVL_DENIED				= (1<<3),
	LVL_FILE_ERRORS			= (1<<3), //using this to have a better identification of the error
	LVL_BUSY_SPAWN			= (1<<4),
	LVL_WRONG_SPAWN			= (1<<5),
	LVL_WRONG_GHOSTS_COORDS	= (1<<6),
	LVL_GHOST_OUT_OF_MAP	= (1<<7),
	LVL_NO_GHOSTS			= (1<<8),
	LVL_WRONG_CELL			= (1<<9),
	LVL_WRONG_PLAYER_LIVES	= (1<<10)
};
#define LVL_OLD_MSG					"The level is written in an older (or newer) format.\n"
#define LVL_INVALID_MSG				"The level's file is not a valid format or is corrupted.\n"
#define LVL_NOT_FOUND_MSG			"The level's file could not be found.\n"
#define LVL_DENIED_MSG				"File's access denied.\n"
#define LVL_BUSY_SPAWN_MSG 			"Player's spawn point is filled with a wall, at "
#define LVL_WRONG_SPAWN_MSG 		"Player's spawn point is out of the map.\n"
#define LVL_WRONG_GHOSTS_COORDS_MSG	"A ghost's spawn point is filled with a wall at "
#define LVL_GHOST_OUT_OF_MAP_MSG	"A ghost's spawn point is out of the map: "
#define LVL_NO_GHOSTS_MSG			"No ghosts were found.\n"
#define LVL_WRONG_CELL_MSG			"Wrong map-cell's data at "
#define LVL_WRONG_PLAYER_LIVES_MSG	"Player has less than 1 lives.\n"
#define LVL_UNKNOWN_ERROR			"Unknown error.\n"

#define LVL_FORMAT		"dst212pacman0000"
#define LVL_FORMAT_SIZE	16

#define lvlAutoFix(w, msg, ...) do { waddstr(w, "[AUTOFIX] "); wprintw(w, msg __VA_OPT__(,) __VA_ARGS__); wrefresh(w); } while(0)
#define lvlAutoFixWait(w) do { ncpause(w, "Press any key to continue."); wclear(w); } while(0)

extern void lvlError(WINDOW*, const Level_status);
extern Level_status checkGhosts(const Map[MAP_HEIGHT][MAP_WIDTH], Ghost[LVL_MAX_GHOSTS], int8_t*, const bool);
extern Level_status checkPlayer(Map[MAP_HEIGHT][MAP_WIDTH], Player*, const bool);
extern Level_status checkMap(Map[MAP_HEIGHT][MAP_WIDTH], const bool);

#include "../../src/level/status.c"

//END
