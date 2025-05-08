// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

Scene scene_main_create(void);
Scene Scene_lose_create(void);
Scene scene_win_create(void);
ALLEGRO_SAMPLE* NICE;
ALLEGRO_SAMPLE_ID NICE_ID;
ALLEGRO_SAMPLE* WOW;
ALLEGRO_SAMPLE_ID WOW_ID;
char buffer[100];
extern int game_main_Score;

#endif
