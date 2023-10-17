#ifndef INC_APP_STATEMACHINE_H_
#define INC_APP_STATEMACHINE_H_

#include <inttypes.h>

typedef enum _GameState {
	GameState_SetUp,			// Initial set-up of the table
	GameState_Idle,				// Idle before start of the game
	GameState_Sleep,			// Low-power mode after inactivity
	GameState_StartGame,		// Set-up sequence for new game
	GameState_StartAnimationA,	// Playing start animation for player A
	GameState_StartAnimationB,	// Playing start animation for player B
	GameState_RunGame,			// Game is in progress
	GameState_ScoreAnimationA,	// Player A scored (not the winning shot)
	GameState_ScoreAnimationB,	// Player B scored (not the winning shot)
	GameState_WinAnimationA,	// Player A won the game (winning shot)
	GameState_WinAnimationB,	// Player B won the game (winning shot)
	GameState_Error,			// Error state
	GameState_Count				// State used for boundary checking
} GameState;

void App_StateMachine_GameTick();

#endif /* INC_APP_STATEMACHINE_H_ */
