/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//list of needed headers

#define _XOPEN_SOURCE_EXTENDED 1 //needed for ncurses wide char to work

//cstuff headers
#include <ncstuff.h>
#include <chstuff.h>
// #include <miscstuff.h> //in ncstuff.h

#include <time.h>		//time() function
// #include <stdlib.h>	//in miscstuff.h
// #include <math.h>	//in miscstuff.h
// #include <ncurses.h>	//in ncstuff.h
// #include <string.h>	//in strstuff.h, in ncstuff.h
// #include <locale.h>	//in chstuff.h
// #include <wchar.h>	//in chstuff.h

#include <stdint.h>		//int types
#include <stdbool.h>	//bool type
#include <dirent.h>		//get list of files
#include <sys/stat.h>	//file existance

//pacman's headers in order
#include "def.h"					//defines and structures

#include "map/map.h"				//map-related stuff
#include "sprites/sprites.h"		//ghost and player's sprites
#include "sprites/way.h"			//ways-related stuff
#include "sprites/player.h"			//player-related stuff
#include "sprites/ghost.h"			//ghosts-related stuff

#include "settings.h"				//some settings, maybe will never be added
#include "scores.h"					//scores list and check

#include "level/level.h"			//level-related stuff
#include "level/status.h"			//status of a level, if it's bugged or out of standard values
#include "level/files.h"			//operations on levels' files

//source files (loaded later because some of them need each other)
#include "../src/map/map.c"
#include "../src/sprites/sprites.c"
#include "../src/sprites/way.c"
#include "../src/sprites/player.c"
#include "../src/sprites/ghost.c"
#include "../src/level/level.c"

//header and source for level editor
#ifndef _PACMAN_COMPILE_FOR_SERVER
	#include "level/editor/init.h"			//init structures
	#include "level/editor/sidebar.h"		//left-side bar
	#include "level/editor/pick.h"			//pick stuff
	#include "level/editor/editsprites.h"	//edit ghosts and the player's sprites
	#include "level/editor/editor.h"		//level editor, allows to create custom levels

	#include "../src/level/editor/init.c"
	#include "../src/level/editor/sidebar.c"
	#include "../src/level/editor/pick.c"
	#include "../src/level/editor/editsprites.c"
	#include "../src/level/editor/editor.c"
#endif

//final headers
#include "game.h"					//game "engine"
#include "mainmenu.h"				//menu with options
//each of pacman's header needs the above header, except for the first

//END
