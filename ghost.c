#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"
#include "scene_game.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;

/* shared variables. */
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;
/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
Map* basic_map;


// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;

Ghost* ghost_create(int flag) {

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case Blinky:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break; 
	case Pinky:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y+1;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	case Inky:
		ghost->objData.Coord.x = cage_grid_x-1;
		ghost->objData.Coord.y = cage_grid_y+1;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	case Clyde:
		ghost->objData.Coord.x = cage_grid_x+1;
		ghost->objData.Coord.y = cage_grid_y+1;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_orange.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	default:
		ghost->objData.Coord.x = cage_grid_x+2;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}
void ghost_destory(Ghost* ghost) {
	/*
		[TODO]
		free ghost resource
*/
		al_destroy_bitmap(ghost->dead_sprite);
		al_destroy_bitmap(ghost->flee_sprite);
		al_destroy_bitmap(ghost->move_script);
		al_destroy_bitmap(ghost->move_sprite);
		free(ghost);
	
}
void ghost_draw(Ghost* ghost) {
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);

	//Draw default image


	/*
		[TODO]
		Draw ghost according to its status
		hint : use ghost->objData.moveCD value to determine which frame of the animation to draw.

			A not so good way is:

			if(ghost->objData.moveCD % 16 == 0){
				al_draw_scaled_bitmap(...);
			}
			else if(ghost->objData.moveCD % 16 == 1){
				al_draw_scaled_bitmap(...);
			}...

			since modulo operation is expensive, better avoid using it.
	*/
	if (ghost->objData.moveCD > 64 || ghost->objData.moveCD < 0) {
		al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
	}
	else if (ghost->status == FLEE) {
		if ((ghost->objData.moveCD >> 4) & 1) {
			al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
		else {
			al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
	}
	else if (ghost->status == READYTOFREE) {
		if (ghost->objData.moveCD > 48) {
			al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
		else if (ghost->objData.moveCD > 16) {
			al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
		else if (ghost->objData.moveCD > 32) {
			al_draw_scaled_bitmap(ghost->flee_sprite, 48, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
		else {
			al_draw_scaled_bitmap(ghost->flee_sprite, 32, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
		}
	}
	else if (ghost->status == GO_IN) {
		switch (ghost->objData.facing)
		{
		case RIGHT:
			al_draw_scaled_bitmap(ghost->dead_sprite, 0, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case LEFT:
			al_draw_scaled_bitmap(ghost->dead_sprite, 16, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case UP:
			al_draw_scaled_bitmap(ghost->dead_sprite, 32, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		case DOWN:
			al_draw_scaled_bitmap(ghost->dead_sprite, 48, 0, 16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0
			);
			break;
		}
	}
	else {
		if ((ghost->objData.moveCD >> 4) & 1) {
			switch (ghost->objData.facing)
			{
			case RIGHT:
				al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case LEFT:
				al_draw_scaled_bitmap(ghost->move_sprite, 32, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case UP:
				al_draw_scaled_bitmap(ghost->move_sprite, 64, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case DOWN:
				al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;

			default:
				break;
			}
		}
		else {
			switch (ghost->objData.facing)
			{
			case RIGHT:
				al_draw_scaled_bitmap(ghost->move_sprite, 16, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case LEFT:
				al_draw_scaled_bitmap(ghost->move_sprite, 48, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case UP:
				al_draw_scaled_bitmap(ghost->move_sprite, 80, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;
			case DOWN:
				al_draw_scaled_bitmap(ghost->move_sprite, 112, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0);
				break;

			default:
				break;
			}

		}
	}
}
	
	
	/*else if (ghost->objData.moveCD % 16 == 2) {
		al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);
	}
	else if (ghost->objData.moveCD % 16 == 3) {
		al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);
	}*/



	int bitmap_x_offset = 0;
	// [TODO] below is for animation usage, change the sprite you want to use.
	/*if (ghost->status == FLEE) {
		
		if ((ghost->objData.moveCD >> 4) & 1) {
			al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
		
		}
		else {
			al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
		}

		
	}
	else if (ghost->status == GO_IN) {
		
		switch (ghost->objData.facing)
		{
		case LEFT:
			al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0,
				16, 16,
				drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
				draw_region, draw_region, 0);
		}
			
	}
	else {
		/*
		switch (ghost->objData.facing)
		{
		case LEFT:
			...
		}
		
	}

}*/
void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}
void printGhostStatus(GhostStatus S) {

	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirect, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.

	
	int gx= ghost->objData.Coord.x, gy= ghost->objData.Coord.y;

	switch (targetDirect)
	{
	case UP:
		gy -= 1;
		break;
	case DOWN:
		gy += 1;
		break;
	case LEFT:
		gx -= 1;
		break;
	case RIGHT:
		gx += 1;
		break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}

	if (is_wall_block(M, gx, gy) || (room && is_room_block(M, gx, gy)))
		return false;
	

	return true;

}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {

		if(setFLEE){
			if(ghost->status == FREEDOM){
				ghost->status = FLEE;
				ghost->speed = 1;
			}
			game_log("Start to FLEE");
		}else{
			if (ghost->status == FLEE || ghost->status == READYTOFREE) {
				ghost->status = FREEDOM;
				ghost->speed = basic_speed;
				game_log("FLEEing");
			}
		}
	
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE|| ghost->status == READYTOFREE) {
		ghost->status = GO_IN;
		//game_main_Score += 1000;
		ghost->speed = 4;
		
		 //basic_map->score+= 10;
	}
	
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman* const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
	//static Directions proba[50] = { 0 };
	//static int cnt = 0;
	static Directions proba[4]; // possible movement
	int cnt = 0;


	for (Directions i = 1; i <= 4; i++) {
		if (ghost_movable(ghost, M, i, 1)) 	proba[cnt++] = i;

	}

	int directions = rand() % cnt;
	//int move = rand() % cnt;
	//game_log("%d", cnt);
	if (ghost->objData.preMove == UP) {
		if (cnt == 1) {
			ghost_NextMove(ghost, DOWN);
		}
		else if (cnt>=2) {
			
			if (proba[directions] == shortestDirection ) {
				while (proba[directions] == shortestDirection) {
						
						directions = rand() % cnt;
						
				}
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == DOWN) {
		if (cnt == 1) {
			ghost_NextMove(ghost, UP);
		}
		else if (cnt >= 2) {

			if (proba[directions] == shortestDirection) {
				while (proba[directions] == shortestDirection) {
					
						directions = rand() % cnt;
				
				}
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == RIGHT) {
		if (cnt == 1) {
			ghost_NextMove(ghost, LEFT);
		}
		else if (cnt >= 2) {
			if (proba[directions] == shortestDirection) {
				while (proba[directions] == shortestDirection) {
					
						directions = rand() % cnt;
					
				}
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == LEFT) {
		if (cnt == 1) {
			ghost_NextMove(ghost, RIGHT);
		}
		else if (cnt >= 2) {

			if (proba[directions] == shortestDirection ) {
				while (proba[directions] == shortestDirection) {
					
						directions = rand() % cnt;
					
				}
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
}

