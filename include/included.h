/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//list of needed headers

#define _XOPEN_SOURCE_EXTENDED 1 //needed for ncurses wide char to work

//cstuff headers
// #include "../../cstuff/ncstuff.h"
// #include "../../cstuff/chstuff.h"
// #include "../../cstuff/mathstuff.h"
#include <ncstuff.h>
#include <chstuff.h>
#include <mathstuff.h>

#include <time.h>		//time() function
// #include <stdlib.h>	//in mathstuff.h
// #include <ncurses.h>	//in ncstuff.h
// #include <string.h>	//in strstuff.h, in ncstuff.h
// #include <locale.h>	//in chstuff.h
// #include <wchar.h>	//in chstuff.h
// #include <math.h>	//in mathstuff.h
//#include <unistd.h>	//usleep() function, in ncstuff.h

#include <stdint.h>		//int types
#include <stdbool.h>	//bool type
#include <dirent.h>		//get list of files
#include <sys/stat.h>	//file existance

//pacman's headers in order
#include "def.h"					//defines and structures
#include "license.h"				//info and license
#include "map/map.h"				//map-related stuff
#include "sprites/sprites.h"		//ghost and player's sprites
#include "settings.h"				//some settings, maybe will never be added
#include "scores.h"					//scores list and check
#include "level/level.h"			//level-related stuff
#include "level/status.h"			//status of a level, if it's bugged or out of standard values
#include "level/levelfiles.h"		//operations on levels' files

/* To compile the code for the server-side version, add
*	-D _PACMAN_COMPILE_FOR_SERVER
* to the compiler options. The server version doesn't include the level editor,
* to avoid random users edit server's levels.
*/
#ifndef _PACMAN_COMPILE_FOR_SERVER
#include "level/editor/editor.h"	//level editor, allows to create custom levels
#endif

#include "pacman.h"					//final header
//each of pacman's header needs the above header, except for the first

//END
