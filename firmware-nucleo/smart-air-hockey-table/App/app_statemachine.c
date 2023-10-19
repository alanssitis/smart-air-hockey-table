#include "app_statemachine.h"

#include "driver_led.h"
#include "driver_display.h"
#include "driver_encoder.h"
#include "driver_relay.h"
#include "driver_halleffect.h"

volatile struct {
	GameState currGameState;
	uint32_t ticksInState; // Ticks in a given state
	uint8_t winScore; // Number of points required for a player to win
	uint8_t playerScoreA; // Current number of points Player A has scored
	uint8_t playerScoreB; // Current number of points Player B has scored
} GameInfo;

void App_StateMachine_Init()
{
	GameInfo.currGameState = INITIAL_GAMESTATE;
	// TODO: Make winScore modifiable by the player through the OLED menu, for now it is hard-coded to 7
	GameInfo.winScore = 7;
	GameInfo.playerScoreA = 0;
	GameInfo.playerScoreB = 0;
}

// Super-loop that is called every TIM7 tick (1ms)
void App_StateMachine_GameTick()
{
	GameInfo.ticksInState++; // Add 1 tick to counter

	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
	{
		case (GAMESTATE_IDLE):
		{
			// TODO: Implement state handling function
			// TODO switch states on encoder button flag

			if (GameInfo.ticksInState > IDLE_SLEEP_TICKS)
			{
				App_StateMachine_SetState(GAMESTATE_SLEEP);
			}
			break;
		}

		case (GAMESTATE_SLEEP):
		{
			// TODO: Implement state handling function
			Driver_LED_Clear();
			Driver_Display_Clear(DISPLAY_ALL);
			break;
		}

		case (GAMESTATE_START):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_WAIT_A):
		{
			// TODO: Implement state handling function

			if (1) // TODO only do this once the puck is in correct location
			{
				Driver_Relay_TurnOn();
			}
			break;
		}

		case (GAMESTATE_WAIT_B):
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
			Driver_HallEffect_PollInputs();
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_SCORE_A):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_SCORE_B):
		{
			// TODO: Implement state handling function
			break;
		}

		case (GAMESTATE_WIN_A):
		{
			// TODO: Implement state handling function
			Driver_Relay_TurnOff();
			break;
		}

		case (GAMESTATE_WIN_B):
		{
			// TODO: Implement state handling function
			Driver_Relay_TurnOff();
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
void App_StateMachine_SetState(GameState new_state)
{
	// Clear globals
	GameInfo.ticksInState = 0;

	// Switch states
	GameInfo.currGameState = new_state;
}
