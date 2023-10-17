#ifndef INC_APP_STATEMACHINE_H_
#define INC_APP_STATEMACHINE_H_

#include <inttypes.h>

#define NUM_PLAYERS 2
#define PLAYER_A_IDX 0
#define PLAYER_B_IDX 1

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

GameState App_StateMachine_GetGameState();
uint8_t App_StateMachine_GetWinScore();
uint8_t App_StateMachine_GetPlayerScore(uint8_t playerIdx);
void App_StateMachine_IncrementPlayerScore(uint8_t playerIdx);
void App_StateMachine_GameTick();

#endif /* INC_APP_STATEMACHINE_H_ */
