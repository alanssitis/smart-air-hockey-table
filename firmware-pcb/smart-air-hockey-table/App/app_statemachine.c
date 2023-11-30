#include "app_statemachine.h"

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "driver_led.h"
#include "driver_display.h"
#include "driver_encoder.h"
#include "driver_relay.h"
#include "driver_halleffect.h"
#include "driver_goal.h"
#include "precomputed_highlightpos.h"

static struct
{
	GameState currGameState;
	uint_fast32_t ticksInState; // Ticks in a given state
	int_fast8_t winScore; // Number of points required for a player to win
	int_fast8_t brightness; // User-configurable brightness
	uint_fast8_t playerScoreA; // Current number of points Player A has scored
	uint_fast8_t playerScoreB; // Current number of points Player B has scored
	uint_fast8_t currFrame; // current animation frame
	uint_fast8_t miscData; // for random data within a given
} GameInfo;

void App_StateMachine_Init()
{
	GameInfo.currGameState = INITIAL_GAMESTATE;
	GameInfo.winScore = 7;
	GameInfo.brightness = LED_BRIGHTNESS_LEVELS - 1;
	GameInfo.miscData = 0;
}

// Super-loop that is called every TIM7 tick
void App_StateMachine_GameTick()
{
	GameInfo.ticksInState++; // Add 1 tick to counter
	Driver_HallEffect_PollInputs();
	Driver_LED_Tick();
	Driver_Goal_Poll();

	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
	{
		case (GAMESTATE_IDLE):
		{
			if (GameInfo.ticksInState == 1) // Runs on first tick only
			{
				Driver_Encoder_SetActive(true);

				Driver_Display_Print(DISPLAY_0, 0, 0, "\tSmart Air Hockey Menu");
				Driver_Display_Print(DISPLAY_0, 2, 0, "> Score Limit: %2"PRIuFAST8, GameInfo.winScore);
				Driver_Display_Print(DISPLAY_0, 3, 2, "Brightness: %3"PRIuFAST8, GameInfo.brightness);
				Driver_Display_Print(DISPLAY_0, 4, 2, "Start Game");
			}

			static bool modifying = false;
			static int_fast8_t selection = 0;

			if (Driver_Encoder_PollButton())
			{
				if (selection == 2)
				{
					Driver_Display_Clear(DISPLAY_0);
					Driver_Encoder_SetActive(false);
					brightness_idx = GameInfo.brightness; // Tells the LED driver the chosen brightness
					srand(GameInfo.ticksInState);
					App_StateMachine_SetState(GAMESTATE_START);
					break;
				}
				else
				{
					modifying = !modifying;
					if (!modifying)
					{
						Driver_Display_Print(DISPLAY_0, 2 + selection, 0, "> ");
					}
				}
			}

			int_fast8_t rotation = Driver_Encoder_PollRotation();
			if (rotation != 0)
			{
				if (modifying) // Modify selected value
				{
					if (selection == 0)
					{
						GameInfo.winScore += rotation;
						if (GameInfo.winScore < 1) GameInfo.winScore = 1;
						else if (GameInfo.winScore > 99) GameInfo.winScore = 99;
						Driver_Display_Print(DISPLAY_0, 2, 15, "%2"PRIuFAST8, GameInfo.winScore);
					}
					else if (selection == 1)
					{
						GameInfo.brightness += rotation;
						if (GameInfo.brightness < 0) GameInfo.brightness = 0;
						else if (GameInfo.brightness > LED_BRIGHTNESS_LEVELS - 1) GameInfo.brightness = LED_BRIGHTNESS_LEVELS - 1;
						Driver_Display_Print(DISPLAY_0, 3, 14, "%3"PRIuFAST8, GameInfo.brightness);
					}
				}
				else // Change selection
				{
					int_fast8_t prevSelection = selection;
					selection += rotation;
					if (selection < 0) selection = 0;
					else if (selection > 2) selection = 2;

					if (selection != prevSelection)
					{
						Driver_Display_Print(DISPLAY_0, 2 + prevSelection, 0, "  ");
						Driver_Display_Print(DISPLAY_0, 2 + selection, 0, "> ");
					}
				}
			}

			if (modifying)
			{
				if (GameInfo.ticksInState % 32 == 0) {
					if (GameInfo.ticksInState % 64 == 0)
					{
						Driver_Display_Print(DISPLAY_0, 2 + selection, 0, "  ");
					}
					else if (GameInfo.ticksInState % 64 == 32)
					{
						Driver_Display_Print(DISPLAY_0, 2 + selection, 0, "> ");
					}
				}
			}

			// TODO: Rainbow LED matrix

			/*if (Driver_Encoder_PollButton())
			{

			}*/

			// TODO sleep state
//			if (GameInfo.ticksInState > IDLE_SLEEP_TICKS)
//			{
//				App_StateMachine_SetState(GAMESTATE_SLEEP);
//			}
			break;
		}

		case (GAMESTATE_SLEEP):
		{
			// TODO: Get tf out of here
			Driver_LED_Clear();
			Driver_Display_Clear(DISPLAY_ALL);
			break;
		}

		case (GAMESTATE_START):
		{
			// TODO animation
			Driver_Display_Clear(DISPLAY_ALL);
			Driver_Display_ShowScore(DISPLAY_ALL, GameInfo.playerScoreA, GameInfo.playerScoreB);

			if (rand() & 0x1)
			{
				// A gets puck
				App_StateMachine_SetState(GAMESTATE_WAIT_A);
			}
			else
			{
				// B gets puck
				App_StateMachine_SetState(GAMESTATE_WAIT_B);
			}
			break;
		}

		case (GAMESTATE_WAIT_A):
		{
			// TODO animation
			// toggle miscData every 127 ticks (about 2 Hz)
			if (GameInfo.ticksInState & 0x7F)
			{
				GameInfo.miscData = ~GameInfo.miscData;
			}

			// check if we should leave state
			if (halleffect_cols & 0x0000FFFF)
			{
				App_StateMachine_SetState(GAMESTATE_RUN);
			}

			if (GameInfo.miscData)
			{
				for (int col = 0; col < LED_MATRIX_COL_NUM / 2; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0xff, 0x00, 0x00});
					}
				}
			}
			else
			{
				for (int col = 0; col < LED_MATRIX_COL_NUM / 2; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0x00});
					}
				}
			}
			break;
		}

		case (GAMESTATE_WAIT_B):
		{
			// TODO animation
			// toggle miscData every 127 ticks (about 2 Hz)
			if (GameInfo.ticksInState & 0x7F)
			{
				GameInfo.miscData = ~GameInfo.miscData;
			}

			// check if we should leave state
			if (halleffect_cols & 0xFFFF0000)
			{
				App_StateMachine_SetState(GAMESTATE_RUN);
			}

			if (GameInfo.miscData)
			{
				for (int col = LED_MATRIX_COL_NUM / 2; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0xff});
					}
				}
			}
			else
			{
				for (int col = LED_MATRIX_COL_NUM / 2; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0x00});
					}
				}
			}
			break;
		}

		case (GAMESTATE_RUN):
		{
			Driver_LED_SetColor(0, 0, (Color) {0x00, 0xff, 0x00});
			Driver_Relay_TurnOn();

			// TODO fix logic here, done for testing
//			if (!(ldr1_goal || ldr2_goal))
//			{
//				GameInfo.playerScoreA++;
//				App_StateMachine_SetState(GAMESTATE_SCORE_A);
//			}
//			if (!(ldr3_goal || ldr4_goal))
//			{
//				GameInfo.playerScoreB++;
//				App_StateMachine_SetState(GAMESTATE_SCORE_B);
//			}
			if (Driver_Encoder_PollButton())
			{
				GameInfo.playerScoreB++;
				Driver_Display_Clear(DISPLAY_ALL);
				Driver_Display_ShowScore(DISPLAY_ALL, GameInfo.playerScoreA, GameInfo.playerScoreB);
				App_StateMachine_SetState(GAMESTATE_SCORE_B);
			}
//			if (ldr1_goal)
//			{
//				Driver_LED_SetColor(0, 0, (Color) {0xff, 0xff, 0x00});
//			}
//			else
//			{
//				Driver_LED_SetColor(0, 0, (Color) {0x00, 0x00, 0x00});
//			}
//
//			if (ldr2_goal)
//			{
//				Driver_LED_SetColor(0, 1, (Color) {0xff, 0xff, 0x00});
//			}
//			else
//			{
//				Driver_LED_SetColor(0, 1, (Color) {0x00, 0x00, 0x00});
//			}
//
//			if (ldr3_goal)
//			{
//				Driver_LED_SetColor(0, 2, (Color) {0xff, 0xff, 0x00});
//			}
//			else
//			{
//				Driver_LED_SetColor(0, 2, (Color) {0x00, 0x00, 0x00});
//			}
//
//			if (ldr4_goal)
//			{
//				Driver_LED_SetColor(0, 3, (Color) {0xff, 0xff, 0x00});
//			}
//			else
//			{
//				Driver_LED_SetColor(0, 3, (Color) {0x00, 0x00, 0x00});
//			}

			break;
		}

		case (GAMESTATE_SCORE_A):
		{
			// TODO animation and OLED
			if (GameInfo.playerScoreA >= GameInfo.winScore)
			{
				App_StateMachine_SetState(GAMESTATE_WIN_A);
			}
			else
			{
				// check if we should leave state
				if (GameInfo.ticksInState > 1000)
				{
					App_StateMachine_SetState(GAMESTATE_WAIT_B);
				}

				for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0xff, 0x00, 0x00});
					}
				}
			}
			break;
		}

		case (GAMESTATE_SCORE_B):
		{
			// TODO animation and OLED
			if (GameInfo.playerScoreB >= GameInfo.winScore)
			{
				App_StateMachine_SetState(GAMESTATE_WIN_B);
			}
			else
			{
				// check if we should leave state
				if (GameInfo.ticksInState > 1000)
				{
					App_StateMachine_SetState(GAMESTATE_WAIT_A);
				}

				for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0xff});
					}
				}
			}
			break;
		}

		case (GAMESTATE_WIN_A):
		{
			// TODO animation

			if (GameInfo.ticksInState & 0xF)
			{
				GameInfo.currFrame++;
			}

			for (int row = 0; row < LED_MATRIX_ROW_NUM; row++)
			{
				Driver_LED_SetColor(GameInfo.currFrame, row, (Color) {0xff, 0x00, 0x00});
			}

			if (GameInfo.ticksInState > 2000)
			{
				App_StateMachine_SetState(GAMESTATE_IDLE);
			}
			break;
		}

		case (GAMESTATE_WIN_B):
		{
			// TODO animation
			if (GameInfo.ticksInState & 0xF)
			{
				GameInfo.currFrame++;
			}

			for (int row = 0; row < LED_MATRIX_ROW_NUM; row++)
			{
				Driver_LED_SetColor(LED_MATRIX_ROW_NUM - GameInfo.currFrame, row, (Color) {0x00, 0x00, 0xff});
			}

			if (GameInfo.ticksInState > 2000)
			{
				App_StateMachine_SetState(GAMESTATE_IDLE);
			}
			break;
		}

		default:
		{
			// Error: default should never occur
			App_StateMachine_SetState(GAMESTATE_ERROR);
			break;
		}
	}
}

// Set desired state
void App_StateMachine_SetState(GameState new_state)
{
	// Clear globals
	GameInfo.ticksInState = 0;
	GameInfo.currFrame = 0;
	GameInfo.miscData = 0;

	// Switch states
	GameInfo.currGameState = new_state;
}
