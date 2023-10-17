#ifndef INC_APP_STATEMACHINE_H_
#define INC_APP_STATEMACHINE_H_

#include <inttypes.h>

#include "driver_hall_effect.h"
#include "driver_relay.h"

#define INITIAL_GAMESTATE	GAMESTATE_RUN
#define IDLE_SLEEP_TIME		60000

typedef enum _GameState {
	GAMESTATE_IDLE,		// Idle before start of the game
	GAMESTATE_SLEEP,	// Low-power mode after inactivity
	GAMESTATE_START,	// Set-up sequence for new game
	GAMESTATE_WAITA,	// Playing start animation for player A
	GAMESTATE_WAITB,	// Playing start animation for player B
	GAMESTATE_RUN,		// Game is in progress
	GAMESTATE_SCOREA,	// Player A scored (not the winning shot)
	GAMESTATE_SCOREB,	// Player B scored (not the winning shot)
	GAMESTATE_WINA,		// Player A won the game (winning shot)
	GAMESTATE_WINB,		// Player B won the game (winning shot)
	GAMESTATE_ERROR		// Error state
} GameState;

void App_StateMachine_GameTick();

inline void App_StateMachine_SetState(GameState);

#endif /* INC_APP_STATEMACHINE_H_ */
