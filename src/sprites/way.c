/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

void shiftSprite(Coord*pos, Way way) {
	switch(way) {
		case WEST:
			pos->x--;
			break;
		case EAST:
			pos->x++;
			break;
		case NORTH:
			pos->y--;
			break;
		case SOUTH:
			pos->y++;
			break;
	}
}

Way randomMove(const Way moves, Way way) {
	//for each of these if, the sprite MIGHT go back, but it won't until it's the last choice
	Way left = turnLeft(way), right = turnRight(way);
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

//END
