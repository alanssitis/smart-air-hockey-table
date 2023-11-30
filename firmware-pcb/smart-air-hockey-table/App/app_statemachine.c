#include "app_statemachine.h"

#include <inttypes.h>
#include "driver_led.h"
#include "driver_display.h"
#include "driver_encoder.h"
#include "driver_relay.h"
#include "driver_halleffect.h"
#include "precomputed_highlightpos.h"

static struct
{
	GameState currGameState;
	uint_fast32_t ticksInState; // Ticks in a given state
	uint_fast8_t winScore; // Number of points required for a player to win
	uint_fast8_t playerScoreA; // Current number of points Player A has scored
	uint_fast8_t playerScoreB; // Current number of points Player B has scored
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
}

// Super-loop that is called every TIM7 tick (1ms)
void App_StateMachine_GameTick()
{
	GameInfo.ticksInState++; // Add 1 tick to counter
	Driver_HallEffect_PollInputs();
	Driver_LED_Tick();

	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
	{
		case (GAMESTATE_IDLE):
		{
			// TODO: Implement state handling function
			// TODO switch states on encoder button flag

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

	// Switch states
	GameInfo.currGameState = new_state;
}
