/******************************************************************************
 * Project:	Arrow Pacman                                                      *
 * Author:	Emanuele De Stefani <212dst212@gmail.com>                         *
 *                                                                            *
 * See meson.build file for further information.                              *
 *                                                                            *
 * Pacman-like CLI game with a sort of level editor, based on ncurses.        *
 *                                                                            *
 *    Copyright (C) 2020 Emanuele De Stefani                                  *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
 ******************************************************************************/

//window's size
#define WIN_HEIGHT 24
#define WIN_WIDTH 80

#include "include/included.h"

int main(void) {
	WINDOW *mainwin;
	int r = 0; //return value, acutally not used except for line 40
	srand(time(NULL));
	setlocale(LC_ALL, "");
	//init ncurses
	initscr();
	noecho();
	cbreak();
	// use_default_colors();
	ncflags(stdscr);
	//check console's size
	if(getmaxy(stdscr) < WIN_HEIGHT || getmaxx(stdscr) < WIN_WIDTH) {
		wprintw(stdscr, "Resize your terminal to %dx%d (current is %dx%d), then restart the application.\n", WIN_WIDTH, WIN_HEIGHT, getmaxx(stdscr), getmaxy(stdscr));
		wrefresh(stdscr);
		getch();
	} else {
		nccolors();
		mainwin = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
		ncflags(mainwin);
		//everything is set:
		mainMenu(mainwin);
		wclear(mainwin);
		// ncpause(mainwin, "Press any key to exit.");
		delwin(mainwin);
	}
	endwin();
	return r;
}

//END
