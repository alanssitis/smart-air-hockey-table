#ifndef INC_APP_STATEMACHINE_H_
#define INC_APP_STATEMACHINE_H_

#define INITIAL_GAMESTATE	GAMESTATE_IDLE
#define IDLE_SLEEP_TICKS	150000

typedef enum
{
	GAMESTATE_IDLE,			// Idle before start of the game
	GAMESTATE_SLEEP,		// Low-power mode after inactivity
	GAMESTATE_START,		// Set-up sequence for new game
	GAMESTATE_WAIT_A,		// Playing start animation for player A
	GAMESTATE_WAIT_B,		// Playing start animation for player B
	GAMESTATE_RUN_NORMAL,	// Game is in progress
	GAMESTATE_RUN_KOTH,		// Game is in progress
	GAMESTATE_RUN_PAINT,	// Game is in progress
	GAMESTATE_SCORE_A,		// Player A scored (not the winning shot)
	GAMESTATE_SCORE_B,		// Player B scored (not the winning shot)
	GAMESTATE_WIN_A,		// Player A won the game (winning shot)
	GAMESTATE_WIN_B,		// Player B won the game (winning shot)
	GAMESTATE_ERROR			// Error state
} GameState;

typedef enum
{
	GAMEMODE_NORMAL = 1,	// normal (no trail)
	GAMEMODE_KOTH = 2,		// king of the hill
	GAMEMODE_PAINT = 3		// king of the hill
} GameMode;

void App_StateMachine_Init();
void App_StateMachine_GameTick();
void App_StateMachine_SetState(GameState new_state);

#endif /* INC_APP_STATEMACHINE_H_ */
