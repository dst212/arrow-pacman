

//license disclaimer

void pacmanWelcome(WINDOW*w, const short y, const short x, const short defColor) {
	mvncprint(w, y, x, "@P" _PACMAN_NAME " @0E" _PACMAN_VERSION "@P by @0A" _PACMAN_AUTHOR "@P\n", defColor, defColor, defColor);
}

void printLicense() {
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

//END
