#include "app_statemachine.h"

static volatile struct {
	GameState currGameState;
	uint8_t winScore; // Number of points required for a player to win
	uint8_t playerScoreA; // Current number of points Player A has scored
	uint8_t playerScoreB; // Current number of points Player B has scored
} GameInfo;

void App_StateMachine_Init() {
	GameInfo.currGameState = GameState_SetUp;
	// TODO: Make winScore modifiable by the player through the OLED menu, for now it is hard-coded to 7
	GameInfo.winScore = 7;
	GameInfo.playerScoreA = 0;
	GameInfo.playerScoreB = 0;
}

// Super-loop that is called every TIM7 tick (1ms)
void App_StateMachine_GameTick()
{
	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
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
			GameInfo.currGameState = GameState_Error;
			break;
	}
}
