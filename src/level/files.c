/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//functions to write/read/check level files

Level_status loadLevel(WINDOW*w, const char*filename, Level*l, const bool autoFix) {
	Level_status s;
	char lvlFormat[LVL_FORMAT_SIZE] = "", levelsPath[MAX_STR_SIZE*2 + 1] = S_LEVELSDIR;
	FILE*lvlFile = NULL;
	s.status = LVL_OK;
	s.data.x = -1;
	s.data.y = -1;
	lvlFile = fopen(strcat(levelsPath, filename), "rb");
	if(!lvlFile)
		s.status = LVL_NOT_FOUND;
	else {
		if(!fread(lvlFormat, sizeof(char), LVL_FORMAT_SIZE, lvlFile))
			s.status = LVL_DENIED; //file header's check. Actually is not the correct error but who cares
		else if(strncmp(lvlFormat, LVL_FORMAT, LVL_FORMAT_SIZE) != 0)
			s.status = LVL_OLD; //different format's check
		else if(
			!fread(l->map,				sizeof(Map),			MAP_HEIGHT * MAP_WIDTH,	lvlFile) ||
			!fread(&(l->player),		sizeof(Player),		1,						lvlFile) ||
			!fread(l->ghost,			sizeof(Ghost),		LVL_MAX_GHOSTS,			lvlFile) ||
			!fread(&(l->ghosts),		sizeof(l->ghosts),		1,						lvlFile) ||
			!fread(&(l->powerticks),	sizeof(l->powerticks),	1,						lvlFile)
		)
			s.status = LVL_INVALID; //the file was corrupted or wrong formatted
		// else if(!fread(l, sizeof(Level), 1, lvlFile))
			// 	s.status = LVL_INVALID; //the file was corrupted or wrong formatted
		else if(
			(s = checkMap(l->map, autoFix)).status == LVL_OK &&
			(s = checkGhosts(l->map, l->ghost, &(l->ghosts), autoFix)).status == LVL_OK &&
			(s = checkPlayer(l->map, &(l->player), autoFix)).status == LVL_OK
		) {
			//map can be preinted out
			printLevel(w, l, 0, 0, MAP_HEIGHT, MAP_WIDTH);
		}
		fclose(lvlFile);
	}
	return s;
}

bool writeLevel(const char*filename, const Level*l) {
	bool flag = true;
	FILE*lvlFile = NULL;
	const char*lvlFormat = LVL_FORMAT;
	char levelsPath[MAX_STR_SIZE*2 + 1] = S_LEVELSDIR;
	createPacmanDir();
	lvlFile = fopen(strcat(levelsPath, filename), "wb");
	if(lvlFile) {
		if(
			!fwrite(lvlFormat, sizeof(char), LVL_FORMAT_SIZE, lvlFile) ||
			!fwrite(l->map,				sizeof(Map),			MAP_HEIGHT * MAP_WIDTH,	lvlFile) ||
			!fwrite(&(l->player),		sizeof(Player),		1,						lvlFile) ||
			!fwrite(l->ghost,			sizeof(Ghost),		LVL_MAX_GHOSTS,			lvlFile) ||
			!fwrite(&(l->ghosts),		sizeof(l->ghosts),		1,						lvlFile) ||
			!fwrite(&(l->powerticks),	sizeof(l->powerticks),	1,						lvlFile)
		) flag = false;
		fclose(lvlFile);
	} else flag = false;
	return flag;
}

bool levelExist(const char*filename) {
	FILE*lvlFile = NULL;
	char levelsPath[MAX_STR_SIZE*2 + 1] = S_LEVELSDIR;
	lvlFile = fopen(strcat(levelsPath, filename), "rb");
	if(lvlFile != NULL) {
		fclose(lvlFile);
		return true;
	}
	return false;
}

//END
