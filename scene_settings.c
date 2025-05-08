// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

int flag;
int vol;
int volE;
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

//static Button btnSettings2;
//static ALLEGRO_BITMAP* gameButton = NULL;
static Button btnSettings2;
static Button btnSettingsV;
static Button btnSettingsVM;
static Button btnSettingsVEP;
static Button btnSettingsVEM;

static void init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	btnSettings2 = button_create(730, 25, 50, 50, "Assets/bb1.png", "Assets/bb2.png");
	btnSettingsV = button_create((SCREEN_W / 2) + 210, SCREEN_H - 510, 47, 47, "Assets/v1.png", "Assets/v2.png");
	btnSettingsVM = button_create((SCREEN_W / 2) -250, SCREEN_H - 510, 50, 50, "Assets/-1.png", "Assets/-2.png");
	btnSettingsVEP = button_create((SCREEN_W / 2) + 210, SCREEN_H - 410, 47, 47, "Assets/v1.png", "Assets/v2.png");
	btnSettingsVEM = button_create((SCREEN_W / 2) - 250, SCREEN_H - 410, 50, 50, "Assets/-1.png", "Assets/-2.png");
}



static void draw(void ){
	al_clear_to_color(al_map_rgb(51, 178, 177));
	drawButton(btnSettings2);
	drawButton(btnSettingsV);
	drawButton(btnSettingsVM);
	drawButton(btnSettingsVEP);
	drawButton(btnSettingsVEM);
	al_draw_text(
		settingFont,
		al_map_rgb(255, 255, 255),
		(SCREEN_W / 2)-50,
		SCREEN_H - 500,
		ALLEGRO_ALIGN_CENTER,
		"Music Volume"
	);
	al_draw_text(
		settingFont,
		al_map_rgb(255, 255, 255),
		(SCREEN_W / 2) - 30,
		SCREEN_H - 400,
		ALLEGRO_ALIGN_CENTER,
		"Effect Volume"
	);
	char buffer[100];

	vol = music_volume * 100;

	sprintf_s(buffer, sizeof(buffer), "%d", vol);

	al_draw_text(settingFont, al_map_rgb(255, 255, 255), (SCREEN_W / 2)+135, SCREEN_H - 500, 0, buffer);

	char buffer2[100];

	volE = effect_volume * 100;

	sprintf_s(buffer2, sizeof(buffer2), "%d", volE);

	al_draw_text(settingFont, al_map_rgb(255, 255, 255), (SCREEN_W / 2) + 135, SCREEN_H - 400, 0, buffer2);
}

static void destroy(void) {
	al_destroy_bitmap(btnSettings2.default_img);
	al_destroy_bitmap(btnSettings2.hovered_img);
	al_destroy_bitmap(btnSettingsV.default_img);
	al_destroy_bitmap(btnSettingsV.hovered_img);
	al_destroy_bitmap(btnSettingsVM.default_img);
	al_destroy_bitmap(btnSettingsVM.hovered_img);
	al_destroy_bitmap(btnSettingsVEP.hovered_img);
	al_destroy_bitmap(btnSettingsVEM.hovered_img);
}



static void on_key_down(keycode) {
	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_menu_create());
		break;
	case ALLEGRO_KEY_ESCAPE:
		game_change_scene(scene_menu_create());
		break;
	default:
		break;
	}
}



static void on_mouse_move(void) {
	btnSettings2.hovered = buttonHover(btnSettings2, mouse_x, mouse_y);
	btnSettingsV.hovered = buttonHover(btnSettingsV, mouse_x, mouse_y);
	btnSettingsVM.hovered = buttonHover(btnSettingsVM, mouse_x, mouse_y);
	btnSettingsVEP.hovered = buttonHover(btnSettingsVEP, mouse_x, mouse_y);
	btnSettingsVEM.hovered = buttonHover(btnSettingsVEM, mouse_x, mouse_y);
}



static void on_mouse_down(void) {
	if (btnSettings2.hovered) {
		if (flag == 1) {
			game_change_scene(scene_menu_create());
		}
		else if (flag == 2) {
			game_change_scene(scene_main_create());
		}
		else if (flag == 3) {
			game_change_scene(scene_win_create());
		}
		else if (flag == 4) {
			game_change_scene(Scene_lose_create());
		}
	}
	else if (btnSettingsV.hovered) 
	{
		if (music_volume + 0.1 > 1)
		{
			music_volume = 1;
		}
		else
		{
			music_volume += 0.1;
		}
		game_log("%f", music_volume);
	}
	else if (btnSettingsVM.hovered)
	{
		if (music_volume - 0.1 < 0)
		{
			music_volume = 0;
		}
		else
		{
			music_volume -= 0.1;
		}
	}
	else if (btnSettingsVEP.hovered) {
		if (effect_volume + 0.1 > 1)
		{
			effect_volume = 1;
		}
		else
		{
			effect_volume += 0.1;
		}
		
	}
	else if (btnSettingsVEM.hovered) {
		if (effect_volume - 0.1 < 0)
		{
			effect_volume = 0;
		}
		else
		{
			effect_volume -= 0.1;
		}

	}
	
}


// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
	
}

/*Scene scene_volume_create(void) {
	Scene v;
	memset(&v, 0, sizeof(Scene));
	v.name = "Volume";
	v.initialize = &init_v;
	v.draw = &drawVolume;
	v.destroy = &destroy_v;
	v.on_key_down = &on_key_down_v;
	v.on_mouse_move = &on_mouse_move_v;
	v.on_mouse_down = &on_mouse_down_v;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return v;
}*/

