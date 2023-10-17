#include "app_statemachine.h"

static volatile GameState currGameState = GameState_Error;
// TODO: Make winScore modifiable by the player through the OLED menu, for now it is hard-coded to 7
static volatile uint8_t winScore = 7; // Number of points required for a player to win
static volatile uint8_t playerScoreA; // Current number of points Player A has scored
static volatile uint8_t playerScoreB; // Current number of points Player B has scored

GameState App_StateMachine_GetGameState()
{
	return currGameState;
}

uint8_t App_StateMachine_GetWinScore()
{
	return winScore;
}

// If an invalid PlayerId is provided, then UINT8_MAX is returned
uint8_t App_StateMachine_GetPlayerScore(PlayerId id)
{
	if (id == PlayerId_A) {
		return playerScoreA;
	}
	else if (id == PlayerId_B) {
		return playerScoreB;
	}

	return UINT8_MAX;
}

void App_StateMachine_IncrementPlayerScore(PlayerId id)
{
	if (id == PlayerId_A) {
		playerScoreA += 1;
	}
	else if (id == PlayerId_B) {
		playerScoreB += 1;
	}
}

// Super-loop that is called every TIM7 tick (1ms)
void App_StateMachine_GameTick()
{
	//  Coordinating switch statement, each case calls a function which will handle the current case and return the next case
	switch (currGameState)
	{
		case (GameState_SetUp):
			// TODO: Implement state handling function
			break;
		case (GameState_Idle):
			// TODO: Implement state handling function
			break;
		case (GameState_Sleep):
			// TODO: Implement state handling function
			break;
		case (GameState_StartGame):
			// TODO: Implement state handling function
			break;
		case (GameState_StartAnimationA):
			// TODO: Implement state handling function
			break;
		case (GameState_StartAnimationB):
			// TODO: Implement state handling function
			break;
		case (GameState_RunGame):
			// TODO: Implement state handling function
			break;
		case (GameState_ScoreAnimationA):
			// TODO: Implement state handling function
			break;
		case (GameState_ScoreAnimationB):
			// TODO: Implement state handling function
			break;
		case (GameState_WinAnimationA):
			// TODO: Implement state handling function
			break;
		case (GameState_WinAnimationB):
			// TODO: Implement state handling function
			break;
		default:
			// Error: default should never occur
			currGameState = GameState_Error;
			break;
	}
}
