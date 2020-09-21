/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE.
 */

int8_t ghostSpawn(const Level*l, const short y, const short x) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(l->ghost[i].spawn.x == x && l->ghost[i].spawn.y == y)
			return i;
	return -1;
}
int8_t ghostPos(const Level*l, const short y, const short x) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y)
			return i;
	return -1;
}
int8_t ghostHit(const Level*l, const short y, const short x, const Way way) {
	int i;
	for(i = 0; i < l->ghosts; i++)
		if(
			turnAround(way) == l->ghost[i].way && (
				(l->ghost[i].pos.x == x - 1 && l->ghost[i].pos.y == y) ||
				(l->ghost[i].pos.x == x + 1 && l->ghost[i].pos.y == y) ||
				(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y - 1) ||
				(l->ghost[i].pos.x == x && l->ghost[i].pos.y == y + 1)
			)
		)	return i;
	return -1;
}

void printSprites(WINDOW*w, const Level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++)
		printGhost(w, l, i);
	printPlayer(w, l);
}
void unprintSprites(WINDOW*w, const Level*l) {
	int8_t i;
	for(i = 0; i < l->ghosts; i++)
		printMapCell(w, l, l->ghost[i].pos.y, l->ghost[i].pos.x);
	printMapCell(w, l, l->player.pos.y, l->player.pos.x);
}

//END
