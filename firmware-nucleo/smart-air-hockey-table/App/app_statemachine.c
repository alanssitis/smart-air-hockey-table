#include "app_statemachine.h"
#include "driver_hall_effect.h"
#include "driver_relay.h"

volatile struct {
	uint8_t currGameState;
	uint32_t timeInState; // Time (in ticks) in a given state
	uint8_t winScore; // Number of points required for a player to win
	uint8_t playerScoreA; // Current number of points Player A has scored
	uint8_t playerScoreB; // Current number of points Player B has scored
} GameInfo;

void App_StateMachine_Init() {
	GameInfo.currGameState = INITIAL_GAMESTATE;
	// TODO: Make winScore modifiable by the player through the OLED menu, for now it is hard-coded to 7
	GameInfo.winScore = 7;
	GameInfo.playerScoreA = 0;
	GameInfo.playerScoreB = 0;
}

// Super-loop that is called every TIM7 tick (1ms)
void App_StateMachine_GameTick()
{
	GameInfo.timeInState++; // add 1 tick count to timer

	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
	{
		case (GAMESTATE_IDLE):
		{
			// TODO: Implement state handling function
			// TODO switch states on encoder button flag

			if (GameInfo.timeInState > IDLE_SLEEP_TIME)
			{
				App_StateMachine_SetState(GAMESTATE_SLEEP);
			}
			break;
		}

		case (GAMESTATE_SLEEP):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_START):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_WAITA):
		{
			// TODO: Implement state handling function

			if (1) // TODO only do this once the puck is in correct location
			{
				Driver_Relay_TurnOn();
			}
			break;
		}

		case (GAMESTATE_WAITB):
		{
			// TODO: Implement state handling function

			if (1) // TODO only do this once the puck is in correct location
			{
				Driver_Relay_TurnOn();
			}
			break;
		}

		case (GAMESTATE_RUN):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_SCOREA):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_SCOREB):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_WINA):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_WINB):
		{
			// TODO: Implement state handling function
			break;
		}

		default:
		{
			// Error: default should never occur
			GameInfo.currGameState = GAMESTATE_IDLE;
			break;
		}
	}
}

// Set desired state
inline void App_StateMachine_SetState(GameState newstate)
{
	// Clear globals
	GameInfo.timeInState = 0;

	// Switch states
	GameInfo.currGameState = newstate;
}
