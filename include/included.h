/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
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
#include "license.h"				//info and license
#include "map/map.h"				//map-related stuff
#include "sprites/sprites.h"		//ghost and player's sprites
#include "settings.h"				//some settings, maybe will never be added
#include "scores.h"					//scores list and check
#include "level/level.h"			//level-related stuff
#include "level/status.h"			//status of a level, if it's bugged or out of standard values
#include "level/levelfiles.h"		//operations on levels' files

#ifndef _PACMAN_COMPILE_FOR_SERVER
#include "level/editor/editor.h"	//level editor, allows to create custom levels
#endif

#include "pacman.h"					//final header
//each of pacman's header needs the above header, except for the first

//END
