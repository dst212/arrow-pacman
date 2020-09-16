/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in main.c at root directory.
 */

//settings functions for future use, not implemented yet

#define S_SETTINGS	"settings.dat"
#define S_SCORES	"scores.dat"
#define S_DIRPATH	"./arrow-pacman-data/"
#define S_LEVELSDIR	"" S_DIRPATH "levels/"

#define S_MAX_FILE_LIST WIN_HEIGHT

void pickLevel(const char*filename) { //will be completed one day
	DIR*dir;
	struct dirent*de[S_MAX_FILE_LIST];
	int i;
	if((dir = opendir(S_DIRPATH)) != NULL) {
		i = 0;
		while(i < S_MAX_FILE_LIST && ((de[i] = readdir(dir)) != NULL))
			i++;
	}
	//INCOMPLETE
}

void createPacmanDir() {
	struct stat s;
	if(stat(S_DIRPATH, &s) != 0)
		mkdir(S_DIRPATH, 0755);
	else if(!S_ISDIR(s.st_mode)) {
		rename(S_DIRPATH, S_DIRPATH ".old");
		mkdir(S_DIRPATH, 0755);
	}
	if(stat(S_LEVELSDIR, &s) != 0)
		mkdir(S_LEVELSDIR, 0755);
	else if(!S_ISDIR(s.st_mode)) {
		rename(S_LEVELSDIR, S_LEVELSDIR ".old");
		mkdir(S_LEVELSDIR, 0755);
	}
}

//END
