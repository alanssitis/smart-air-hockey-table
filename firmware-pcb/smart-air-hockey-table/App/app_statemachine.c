#include "app_statemachine.h"

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
	uint_fast8_t winScore; // Number of points required for a player to win
	uint_fast8_t playerScoreA; // Current number of points Player A has scored
	uint_fast8_t playerScoreB; // Current number of points Player B has scored
	uint_fast8_t miscData; // for random data within a given state
} GameInfo;

static char bars[13][24] = {
		"    |\t\t            |",
		"    |\t \t           |",
		"    |\t  \t          |",
		"    |\t   \t         |",
		"    |\t    \t        |",
		"    |\t     \t       |",
		"    |\t      \t      |",
		"    |\t       \t     |",
		"    |\t        \t    |",
		"    |\t         \t   |",
		"    |\t          \t  |",
		"    |\t           \t |",
		"    |\t            \t|",
};

void App_StateMachine_Init()
{
	GameInfo.currGameState = INITIAL_GAMESTATE;
	// TODO: Make winScore modifiable by the player through the OLED menu, for now it is hard-coded to 7
	GameInfo.winScore = 7;
	GameInfo.playerScoreA = 0;
	GameInfo.playerScoreB = 0;
	GameInfo.miscData = 0;
}

// Super-loop that is called every TIM7 tick (1ms)
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
			// TODO: Implement state handling function
			// TODO add menu functionality
			// TODO clear OLED
			Driver_Relay_TurnOff();
			GameInfo.playerScoreA = 0;
			GameInfo.playerScoreB = 0;

			// Background
			for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
				for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
					Driver_LED_SetColor(col, row, (Color) {0x2f, 0x2f, 0x2f});
				}
			}

			brightness_idx += Driver_Encoder_PollRotation();
			if (brightness_idx < 0) {
				brightness_idx = 0;
			} else if (brightness_idx > 12) {
				brightness_idx = 12;
			}

			for (int i = 2; i < 6; i++) {
				Driver_Display_Print(DISPLAY_0, i, 0, bars[brightness_idx]);
			}

			int col_pos = 0, row_pos = 0;
			int num_col_on = __builtin_popcount(halleffect_cols);
			int num_row_on = __builtin_popcount(halleffect_rows);

			if (num_col_on > 0 && num_row_on > 0) {
				for (int i = 0; i < LED_MATRIX_COL_NUM; i++) {
					if (halleffect_cols & 1 << i) {
						col_pos += i;
					}
					if (i < LED_MATRIX_ROW_NUM && halleffect_rows & 1 << i) {
						row_pos += i;
					}

				}
				col_pos /= num_col_on;
				row_pos /= num_row_on;
			} else {
				col_pos = -1;
				row_pos = -1;
			}

			if (col_pos != -1 && row_pos != -1) {
				for (int i = 0; i < highlighted_area_size[col_pos][row_pos]; i++) {
					Driver_LED_SetColor(highlighted_area[col_pos][row_pos][i].col, highlighted_area[col_pos][row_pos][i].row, (Color) { 0xff, 0, 0 });
				}
			}

			if (Driver_Encoder_PollButton())
			{
				srand(GameInfo.ticksInState);
				App_StateMachine_SetState(GAMESTATE_START);
			}
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
				Driver_LED_SetColor(0, 0, (Color) {0xff, 0x00, 0x00});
			}
			else
			{
				Driver_LED_SetColor(0, 0, (Color) {0x00, 0x00, 0x00});
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
				Driver_LED_SetColor(0, 0, (Color) {0x00, 0x00, 0xff});
			}
			else
			{
				Driver_LED_SetColor(0, 0, (Color) {0x00, 0x00, 0x00});
			}
			break;
		}

		case (GAMESTATE_RUN):
		{
			Driver_LED_SetColor(0, 0, (Color) {0x00, 0xff, 0x00});
			Driver_Relay_TurnOn();

			// TODO fix logic here, done for testing
			if (!(ldr1_goal || ldr2_goal))
			{
				GameInfo.playerScoreA++;
				App_StateMachine_SetState(GAMESTATE_SCORE_A);
			}
			if (!(ldr3_goal || ldr4_goal))
			{
				GameInfo.playerScoreB++;
				App_StateMachine_SetState(GAMESTATE_SCORE_B);
			}

			break;
		}

		case (GAMESTATE_SCORE_A):
		{
			// TODO animation and OLED
			Driver_LED_SetColor(0, 0, (Color) {0xff, 0xff, 0x00});

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

				Driver_LED_SetColor(0, 0, (Color) {0xff, 0x00, 0x00});
			}
			break;
		}

		case (GAMESTATE_SCORE_B):
		{
			// TODO animation and OLED
			Driver_LED_SetColor(0, 0, (Color) {0xff, 0x00, 0xff});

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

				Driver_LED_SetColor(0, 0, (Color) {0x00, 0x00, 0xff});
			}
			break;
		}

		case (GAMESTATE_WIN_A):
		{
			// TODO animation
			Driver_LED_SetColor(0, 0, (Color) {0x00, 0xff, 0xff});

			if (GameInfo.ticksInState > 2000)
			{
				App_StateMachine_SetState(GAMESTATE_IDLE);
			}
			break;
		}

		case (GAMESTATE_WIN_B):
		{
			// TODO animation
			Driver_LED_SetColor(0, 0, (Color) {0x00, 0xff, 0xff});

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
	GameInfo.miscData = 0;

	// Switch states
	GameInfo.currGameState = new_state;
}
