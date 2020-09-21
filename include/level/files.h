/*
 * Arrow Pacman
 * Copyright (C) 2020 Emanuele De Stefani <212dst212@gmail.com>
 * Full warranty notice in /src/main.c, full license in /LICENSE
 */

//functions to write/read/check level files

extern Level_status loadLevel(WINDOW*, const char*, Level*, const bool);
extern bool writeLevel(const char*, const Level*);

extern bool levelExist(const char*);

#include "../../src/level/files.c"

//END
