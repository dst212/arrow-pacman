/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

void pacmanWelcome(WINDOW*w, const short y, const short x, const short defColor) {
	mvncprint(w, y, x, "@P" _PACMAN_NAME " @0E" _PACMAN_VERSION "@P by @0A" _PACMAN_AUTHOR "@P\n", defColor, defColor, defColor);
}

void printLicense(void) {
	setbgcolor(stdscr, DEFAULT_COLOR);
	clear();
	redrawwin(stdscr);
	pacmanWelcome(stdscr, 0, 0, DEFAULT_COLOR);
	ncprint(stdscr, "@P\n\
******************************************************************************\n\
*                                                                            *\n\
*    Copyright (C) 2020 @0AEmanuele De Stefani@P                                  *\n\
*                                                                            *\n\
*    This program is free software: you can redistribute it and/or modify    *\n\
*    it under the terms of the GNU General Public License as published by    *\n\
*    the Free Software Foundation, either version 3 of the License, or       *\n\
*    (at your option) any later version.                                     *\n\
*                                                                            *\n\
*    This program is distributed in the hope that it will be useful,         *\n\
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *\n\
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *\n\
*    GNU General Public License for more details.                            *\n\
*                                                                            *\n\
*    You should have received a copy of the GNU General Public License       *\n\
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *\n\
*                                                                            *\n\
******************************************************************************\n",
	DEFAULT_COLOR, DEFAULT_COLOR);
	ncpause(stdscr, "\nPress any key to continue.");
	clear();

}

void mainMenu(WINDOW*mainwin) {
	#define PACMAN_PLAY 0
	#ifndef _PACMAN_COMPILE_FOR_SERVER
		#define PACMAN_LEVEL_EDITOR PACMAN_PLAY + 1
	#else
		#define PACMAN_LEVEL_EDITOR 0
	#endif
	#define PACMAN_SCORES PACMAN_LEVEL_EDITOR + 1
	#define PACMAN_ABOUT PACMAN_SCORES + 1
	#define PACMAN_QUIT PACMAN_ABOUT + 1

	int defColor = DEFAULT_COLOR, choice;
	const char*menu[] = {
		"* Play",
		#ifndef _PACMAN_COMPILE_FOR_SERVER
		"* Editor",
		#endif
		"* Scores",
		"* About",
		"* Quit"
	};
	do{
		pacmanWelcome(mainwin, 0, 0, defColor);
		waddwstr(mainwin, L"Use ↓↑ to move and ENTER to select.\n\n");
		wrefresh(mainwin);
		choice = ncmenu(getcury(mainwin), getcurx(mainwin) + 1, sizeof(menu) / sizeof(menu[0]), 15, menu, defColor, revcolor(defColor));
		switch(choice) {
			case PACMAN_PLAY:
				startPacMan();
				break;
			#ifndef _PACMAN_COMPILE_FOR_SERVER
			case PACMAN_LEVEL_EDITOR:
				levelEditor();
				break;
			#endif
			case PACMAN_SCORES:
				listScores();
				break;
			case PACMAN_ABOUT:
				printLicense();
		}
		wclear(mainwin);
	} while(choice != PACMAN_QUIT);
}

//END
