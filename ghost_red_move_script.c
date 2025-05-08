
#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
/* Shared variables */
#define GO_OUT_TIME 256
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M);
static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M);

static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M) {
	// [HACKATHON 2-4]
	// Uncomment the following code and finish pacman picking random direction.

	
	static Directions proba[4]; // possible movement
	int cnt = 0;
	
	
	for (Directions i = 1; i <= 4; i++) {
		if (ghost_movable(ghost, M, i, 1)) 	proba[cnt++] = i;

	}

	int directions = rand() % cnt;
	//int move = rand() % cnt;
	//game_log("%d", cnt);
	if (ghost->objData.preMove == UP) {
		if (proba[directions] != DOWN) {
			ghost_NextMove(ghost, proba[directions]);
		}
		else if (cnt == 1) {
			ghost_NextMove(ghost, DOWN);
		}
		else if (proba[directions] == DOWN) {
			while (proba[directions] == DOWN) {
				directions = rand() % cnt;
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == DOWN) {
		if (proba[directions] != UP) {
			ghost_NextMove(ghost, proba[directions]);
		}
		else if (cnt == 1) {
			ghost_NextMove(ghost, UP);
		}
		else if (proba[directions] == UP) {
			while (proba[directions] == UP) {
				directions = rand() % cnt;
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == RIGHT) {
		if (proba[directions] != LEFT) {
			ghost_NextMove(ghost, proba[directions]);
		}
		else if (cnt == 1) {
			ghost_NextMove(ghost, LEFT);
		}
		else if (proba[directions] == LEFT) {
			while (proba[directions] == LEFT) {																	
				directions = rand() % cnt;
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	else if (ghost->objData.preMove == LEFT) {
		if (proba[directions] != RIGHT) {
			ghost_NextMove(ghost, proba[directions]);
		}
		else if (cnt == 1) {
			ghost_NextMove(ghost, RIGHT);
		}
		else if (proba[directions] == RIGHT) {
			while (proba[directions] == RIGHT) {
				directions = rand() % cnt;
			}
			ghost_NextMove(ghost, proba[directions]);
		}
	}
	

	// [TODO] (Not in Hackathon) 
	// Description:
	// For red(Blinky) ghost, we ask you to implement an random strategy ghost, 
	// which means moving in random direction.
	// But your random strategy have to designed carefully so that ghost won't walk back and forth.
	// (The code here DO perform walking back and forth.)
	
}

static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case UP:
		if (ghost->objData.Coord.y == 10)
			ghost_NextMove(ghost, DOWN);
		else
			ghost_NextMove(ghost, UP);
		break;
	case DOWN:
		if (ghost->objData.Coord.y == 12)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	default:
		ghost_NextMove(ghost, UP);
		break;
	}
}

void ghost_red_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
		switch (ghost->status)
		{
		case BLOCKED:
			ghost_red_move_script_BLOCKED(ghost, M);
			if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
				ghost->status = GO_OUT;
			break;
		case FREEDOM:
			ghost_red_move_script_FREEDOM(ghost, M);
			break;
		case GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				ghost->status = GO_OUT;
				ghost->speed = 2;
			}
			break;
		case FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		case READYTOFREE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}