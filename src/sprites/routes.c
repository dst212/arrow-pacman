/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

Way ghostNextMove(Coord pos, uint16_t*bitmap) {
	Way r = bitmapWay(bitmap, pos);
	if(r != WEST && r != EAST && r != NORTH && r != SOUTH)
		r = 0;
	return r;
}

short ghostRoutes(Level*l, int8_t i, uint16_t*bitmap, Coord dest, Way way, short count) {
	//track routes on a bitmap (it was a bitmap but i changed it actually and it's no more a bitmap)
	Way moves = ghostMoves(dest, l->map);
	bool oneway = true;
	Coord next = dest;
	if(way != 0) {
		do { //follow the direction until a crossing comes out
			if(moves == 0 || moves == turnAround(way)) {
				return -1;
			} else switch(moves) {
				default:
					oneway = false;
					break;
				case 0b0101: //WEST & EAST
					dest.x += (way == WEST) ? -1 : 1;
					count++;
					break;
				case 0b1010: // NORTH & SOUTH
					dest.y += (way == NORTH) ? -1 : 1;
					count++;
					break;
				case 0b0011: // WEST & NORTH
					if(way == EAST)
						way = NORTH, dest.y--;
					else
						way = WEST, dest.x--;
					count++;
					break;
				case 0b1001: //WEST & SOUTH
					if(way == EAST)
						way = SOUTH, dest.y++;
					else
						way = WEST, dest.x--;
					count++;
					break;
				case 0b0110: //EAST & NORTH
					if(way == WEST)
						way = NORTH, dest.y--;
					else
						way = EAST, dest.x++;
					count++;
					break;
				case 0b1100: //EAST & SOUTH
					if(way == WEST)
						way = SOUTH, dest.y++;
					else
						way = EAST, dest.x++;
					count++;
					break;
			}
			if(oneway) //if it still has to go on, update moves
				moves = ghostMoves(dest, l->map);
		} while(oneway);
		//if the next move is not set yet OR
		//the next move's count is bigger than the current one
		if(ghostNextMove(dest, bitmap) == 0 || bitmapCount(bitmap, dest) > count)
			bitmapMark(bitmap, dest, turnAround(way), ++count);
		else
			return count;
	} else {
		bitmapMark(bitmap, dest, WEST, 0);
	}
	//can go West and it wasn't the previous way
	if((moves & WEST) && turnAround(way) != WEST) {
		next = dest, next.x--;
		ghostRoutes(l, i, bitmap, next, WEST, count);
	}
	//can go East and it wasn't the previous way
	if((moves & EAST) && turnAround(way) != EAST) {
		next = dest, next.x++;
		ghostRoutes(l, i, bitmap, next, EAST, count);
	}
	//can go North and it wasn't the previous way
	if((moves & NORTH) && turnAround(way) != NORTH) {
		next = dest, next.y--;
		ghostRoutes(l, i, bitmap, next, NORTH, count);
	}
	//can go South and it wasn't the previous way
	if((moves & SOUTH) && turnAround(way) != SOUTH) {
		next = dest, next.y++;
		ghostRoutes(l, i, bitmap, next, SOUTH, count);
	}
	return 0; //-1 == stuck, 0 == passing, gt 0 == completed
}

//END
