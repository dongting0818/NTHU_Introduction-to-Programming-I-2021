#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include "scene_game.h"

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_BITMAP* gameWin = NULL;
static ALLEGRO_BITMAP* gameLose = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;
static int gameWinW;
static int gameWinH;
static int gameLoseW;
static int gameLoseH;
int flag = 0;

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
	static Button btnSettings;

static void init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	btnSettings = button_create(730, 25, 50, 50, "Assets/bb1.png", "Assets/bb2.png");

	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);

}

static void init_win() {
	gameWin = load_bitmap("Assets/you-win.png");
	gameWinW = al_get_bitmap_width(gameWin);
	gameWinH = al_get_bitmap_height(gameWin);
	stop_bgm(NICE_ID);
	NICE_ID = play_bgm(NICE, music_volume);
	//al_rest(2.2);
	//stop_bgm(NICE_ID);

}

static void init_lose() {
	gameLose = load_bitmap("Assets/you-lose.png");
	gameLoseW = al_get_bitmap_width(gameLose);
	gameLoseH = al_get_bitmap_height(gameLose);
	stop_bgm(WOW_ID);
	WOW_ID = play_bgm(WOW, music_volume );

}

static void draw() {
	
	al_clear_to_color(al_map_rgb(25, 153, 151));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);

		// [HACKATHON 3-3]
		// TODO: Draw button
		// Uncomment and fill the code below
		 drawButton(btnSettings);

}

static void draw_win() {

	al_clear_to_color(al_map_rgb(51, 178, 177));

	//const float scale = 0.7;
	//const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	//const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;
	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * 0.2 * gameWinW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * 0.2 * gameWinH;
	//draw title image
	al_draw_bitmap(
		gameWin,
		220,220,0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 175,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"SPACE\" TO REPLAY"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 130,
		ALLEGRO_ALIGN_CENTER,
		"OR"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 85,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\" TO CONTINUE"
	);
	char buffer2[100];
	sprintf_s(buffer2, sizeof(buffer2), "YOUR SCORE: %d", game_main_Score);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W/2, 100, ALLEGRO_ALIGN_CENTRE, buffer2);
	// [HACKATHON 3-3]
	// TODO: Draw button
	// Uncomment and fill the code below
	drawButton(btnSettings);

}

static void draw_lose() {

	al_clear_to_color(al_map_rgb(51, 178, 177));

	//const float scale = 0.7;
	//const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	//const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;
	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * 0.2 * gameLoseW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * 0.2 * gameLoseH;
	//draw title image
	al_draw_bitmap(
		gameLose,
		220, 220, 0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 175,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"SPACE\" TO REPLAY"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 130,
		ALLEGRO_ALIGN_CENTER,
		"OR"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 85,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\" TO CONTINUE"
	);
	char buffer3[100];
	sprintf_s(buffer3, sizeof(buffer3), "YOUR SCORE: %d", game_main_Score);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTRE, buffer3);
	// [HACKATHON 3-3]
	// TODO: Draw button
	// Uncomment and fill the code below
	drawButton(btnSettings);

}


static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
		 btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
		 
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnSettings.hovered) {
		game_change_scene(scene_settings_create());
		//game_change_scene(scene_volume_create());
	}
}


static void destroy() {
	stop_bgm(menuBGM);
	
	//stop_bgm(NICE_ID);
	//al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below
	
	//al_destroy_bitmap(...);
	al_destroy_bitmap(gameTitle);
	
}

static void destroyWin() {
	
	stop_bgm(NICE_ID);
	//stop_bgm(NICE_ID);
	//al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below
	
	al_destroy_bitmap(gameWin);
	
}


static void destroyLose() {
	stop_bgm(WOW_ID);
	al_destroy_bitmap(gameLose);
}

static void on_key_down(int keycode) {

	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_main_create());
		break;
		default:
			break;
	}
}

static void on_key_down_lose(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			break;
		case ALLEGRO_KEY_SPACE:
			game_change_scene(scene_main_create());
		default:
			break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {
	flag = 1;
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Menu scene created");
	return scene;
}

Scene scene_win_create(void) {
	flag = 3;
	Scene s;
	memset(&s, 0, sizeof(Scene));
	s.name = "Win";
	s.initialize = &init_win;
	s.draw = &draw_win;
	s.destroy = &destroyWin;
	s.on_key_down = &on_key_down_lose;
	s.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	s.on_mouse_down = &on_mouse_down;
	//NICE_ID = play_bgm(NICE, music_volume+10);
	//al_rest(2.2);
	//stop_bgm(NICE_ID);
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Win scene created");
	return s;
}
/*Scene win_create() {
	Scene s;
	memset(&s, 0, sizeof(Scene));
	scene.

}*/

Scene Scene_lose_create() {
	flag = 4;
	Scene l;
	memset(&l, 0, sizeof(Scene));
	l.name = "Lose";
	l.initialize = &init_lose;
	l.draw = &draw_lose;
	l.destroy = &destroyLose;
	l.on_key_down = &on_key_down_lose;
	l.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	l.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Lose scene created");
	return l;
}