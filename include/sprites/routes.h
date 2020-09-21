/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//functions needed for ghosts' paths-map

#define bitmapMark(m, pos, way, count)	((validCoord((pos).y, (pos).x)) ? (				(m)[(pos).y * MAP_HEIGHT + (pos).x] = ((count) << 4) | ((way) & 0b1111)) : 0)
#define bitmapUnark(m, pos)				((validCoord((pos).y, (pos).x)) ? (				(m)[(pos).y * MAP_HEIGHT + (pos).x] = 0) : 0)
#define bitmapWay(m, pos)				((validCoord((pos).y, (pos).x)) ? (Way)(		(m)[(pos).y * MAP_HEIGHT + (pos).x] & (0b1111)) : 0U)
#define bitmapCount(m, pos)				((validCoord((pos).y, (pos).x)) ? (uint16_t)(	(m)[(pos).y * MAP_HEIGHT + (pos).x] >> 4) : 0U)

extern Way ghostNextMove(Coord, uint16_t*);
extern short ghostRoutes(Level*, int8_t, uint16_t*, Coord, Way, short);

#include "../src/sprites/routes.c"

//END
