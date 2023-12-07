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
#include "driver_eeprom.h"
#include "precomputed_highlightpos.h"
#include "precomputed_rainbow.h"

static struct
{
	GameState currGameState;
	GameMode currGameMode;
	uint_fast32_t ticksInState; // Ticks in a given state
	int_fast8_t winScore; // Number of points required for a player to win
	int_fast8_t brightness; // User-configurable brightness
	uint_fast8_t playerScoreA; // Current number of points Player A has scored
	uint_fast8_t playerScoreB; // Current number of points Player B has scored
	uint_fast8_t currFrame; // current animation frame
	uint_fast8_t miscData; // for random data within a given
	uint_fast32_t miscData2; // for random data within a given
} GameInfo;

static Color trail_color[32] = {
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x1e},
    {0xff, 0x00, 0x3c},
    {0xff, 0x00, 0x5a},
    {0xff, 0x00, 0x78},
    {0xff, 0x00, 0x96},
    {0xff, 0x00, 0xb4},
    {0xff, 0x00, 0xd2},
    {0xff, 0x00, 0xf0},
    {0xf0, 0x00, 0xff},
    {0xd2, 0x00, 0xff},
    {0xb4, 0x00, 0xff},
    {0x96, 0x00, 0xff},
    {0x78, 0x00, 0xff},
    {0x5a, 0x00, 0xff},
    {0x3c, 0x00, 0xff},
    {0x1e, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0xff},
};

#define TRAIL_SIZE 192
static Coordinate trail[TRAIL_SIZE] = {0};
static bool trail_on[TRAIL_SIZE] = {false};
static int32_t trail_start = 0;

void App_StateMachine_Init()
{
	GameInfo.currGameState = INITIAL_GAMESTATE;
//	GameInfo.winScore = 7;
//	GameInfo.brightness = LED_BRIGHTNESS_LEVELS / 2;
	uint32_t load_data[2] = { 0, 0 };
	Driver_Eeprom_Read_Data(0x4000, load_data, 2);
	GameInfo.winScore = (uint8_t) load_data[0];
	GameInfo.brightness = (uint8_t) load_data[1];
	GameInfo.miscData = 0;
	GameInfo.currGameMode = 1;
	Driver_LED_Clear();
}

// Super-loop that is called every TIM7 tick
void App_StateMachine_GameTick()
{
	GameInfo.ticksInState++; // Add 1 tick to counter
	Driver_HallEffect_PollInputs();
	if (GameInfo.ticksInState & 1) Driver_LED_Tick();
	Driver_Goal_Poll();

	//  Coordinating switch statement, each case calls a function which will handle currGameInfo
	switch (GameInfo.currGameState)
	{
		case (GAMESTATE_IDLE):
		{
			GameInfo.miscData++;
			static int_fast8_t selection;
			static bool modifying = false;

			if (GameInfo.ticksInState == 1) // Runs on first tick only
			{
				selection = 0;
				Driver_Encoder_PollButton();
				Driver_Encoder_SetActive(true);

				Driver_Display_Print(DISPLAY_0, 0, 0, "\tSmart Air Hockey Menu");
				Driver_Display_Print(DISPLAY_0, 2, 0, "> Score Limit: %2"PRIuFAST8, GameInfo.winScore);
				Driver_Display_Print(DISPLAY_0, 3, 2, "Brightness: %3"PRIuFAST8, GameInfo.brightness);
				Driver_Display_Print(DISPLAY_0, 4, 2, "Gamemode: ");
				if (GameInfo.currGameMode == GAMEMODE_NORMAL)
				{
					Driver_Display_Print(DISPLAY_0, 4, 12, "Normal  ");
				}
				else if (GameInfo.currGameMode == GAMEMODE_KOTH)
				{
					Driver_Display_Print(DISPLAY_0, 4, 12, "K.O.T.H.");
				}
				Driver_Display_Print(DISPLAY_0, 5, 2, "Start Game");

//				Driver_LED_Clear();
				brightness_idx = GameInfo.brightness;
			}

			if (Driver_Encoder_PollButton())
			{
				GameInfo.miscData = 0;
				if (selection == 3)
				{
					Driver_Display_Clear(DISPLAY_0);
					Driver_Display_Print(DISPLAY_ALL, 0, 0, "\t      Game Score      ");
					Driver_Encoder_SetActive(false);
					srand(GameInfo.ticksInState);
					uint32_t save_data[2] = { (uint32_t) GameInfo.winScore, (uint32_t) GameInfo.brightness };
					Driver_Eeprom_Write_Page(0x4000, save_data, 2);
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
				GameInfo.miscData = 0;
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
						brightness_idx = GameInfo.brightness; // Mirror the brightness to the LED driver
						Driver_Display_Print(DISPLAY_0, 3, 14, "%3"PRIuFAST8, GameInfo.brightness);
					}
					else if (selection == 2)
					{
						GameInfo.currGameMode += rotation;
						if (GameInfo.currGameMode > 2) GameInfo.currGameMode = 1;
						if (GameInfo.currGameMode < 1) GameInfo.currGameMode = 2;

						if (GameInfo.currGameMode == GAMEMODE_NORMAL)
						{
							Driver_Display_Print(DISPLAY_0, 4, 12, "Normal  ");
						}
						else if (GameInfo.currGameMode == GAMEMODE_KOTH)
						{
							Driver_Display_Print(DISPLAY_0, 4, 12, "K.O.T.H.");
						}
					}
				}
				else // Change selection
				{
					int_fast8_t prevSelection = selection;
					selection += rotation;
					if (selection < 0) selection = 0;
					else if (selection > 3) selection = 3;

					if (selection != prevSelection)
					{
						Driver_Display_Print(DISPLAY_0, 2 + prevSelection, 0, "  ");
						Driver_Display_Print(DISPLAY_0, 2 + selection, 0, "> ");
					}
				}
			}

			if (modifying)
			{
				GameInfo.miscData = 0;
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

			for (uint_fast8_t col = 0; col < LED_MATRIX_COL_NUM; col++)
			{
				for (uint_fast8_t row = 0; row < LED_MATRIX_ROW_NUM; row++)
				{
					uint_fast32_t i = (GameInfo.ticksInState + (col + row) * 16) % (sizeof(rainbow) / sizeof(Color));
					Driver_LED_SetColor(col, row, rainbow[i]);
				}
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
                trail[(trail_start - 1) % TRAIL_SIZE].col = col_pos;
                trail[(trail_start - 1) % TRAIL_SIZE].row = row_pos;
                trail_on[(trail_start - 1) % TRAIL_SIZE] = true;
			} else {
                trail_on[(trail_start - 1) % TRAIL_SIZE] = false;
			}

            for (int i = 0; i < TRAIL_SIZE; i++) {
            	int idx = (trail_start + i) % TRAIL_SIZE;
            	if (!trail_on[idx]) {
            		continue;
            	}

            	uint8_t col = trail[idx].col;
            	uint8_t row = trail[idx].row;

				for (int j = 0; j < highlighted_area_size[col][row]; j++) {
					uint_fast32_t k = (GameInfo.ticksInState + (highlighted_area[col][row][j].col + highlighted_area[col][row][j].row) * 16) % (sizeof(rainbow) / sizeof(Color));
					Color color = rainbow[k];
					Driver_LED_SetColor(
						highlighted_area[col][row][j].col,
						highlighted_area[col][row][j].row,
						(Color) {
							color.red + i * (0xff - color.red) / (TRAIL_SIZE - 1),
							color.green + i * (0xff - color.green) / (TRAIL_SIZE - 1),
							color.blue + i * (0xff - color.blue) / (TRAIL_SIZE - 1),
						});
				}
            }
            trail_start = (trail_start + 1) % TRAIL_SIZE;

			if (GameInfo.miscData > IDLE_SLEEP_TICKS)
			{
				Driver_LED_Clear();
				App_StateMachine_SetState(GAMESTATE_SLEEP);
			}
			break;
		}

		case (GAMESTATE_SLEEP):
		{
			// TODO: Get tf out of here
//			Driver_LED_Clear();
//			Driver_Display_Clear(DISPLAY_ALL);

			if (Driver_Encoder_PollButton())
			{
				// exit sleep state
				App_StateMachine_SetState(GAMESTATE_IDLE);
			}
			break;
		}

		case (GAMESTATE_START):
		{
			// TODO animation
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
			if (GameInfo.ticksInState > 750)
			{
				if (GameInfo.currGameMode == GAMEMODE_NORMAL)
				{
					App_StateMachine_SetState(GAMESTATE_RUN_NORMAL);
				}
				else
				{
					App_StateMachine_SetState(GAMESTATE_RUN_KOTH);
				}
				break;
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
			if (GameInfo.ticksInState > 750)
			{
				if (GameInfo.currGameMode == GAMEMODE_NORMAL)
				{
					App_StateMachine_SetState(GAMESTATE_RUN_NORMAL);
				}
				else
				{
					App_StateMachine_SetState(GAMESTATE_RUN_KOTH);
				}
				break;
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

		case (GAMESTATE_RUN_NORMAL):
		{
//			Driver_LED_SetColor(0, 0, (Color) {0x00, 0xff, 0x00});
			Driver_Relay_TurnOn();

			// Background
			for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
				for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
					Driver_LED_SetColor(col, row, (Color) {0x2f, 0x2f, 0x2f});
				}
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
                trail[(trail_start - 1) % TRAIL_SIZE].col = col_pos;
                trail[(trail_start - 1) % TRAIL_SIZE].row = row_pos;
                trail_on[(trail_start - 1) % TRAIL_SIZE] = true;
			} else {
                trail_on[(trail_start - 1) % TRAIL_SIZE] = false;
			}

            for (int i = 0; i < TRAIL_SIZE; i++) {
            	int idx = (trail_start + i) % TRAIL_SIZE;
            	if (!trail_on[idx]) {
            		continue;
            	}

            	uint8_t col = trail[idx].col;
            	uint8_t row = trail[idx].row;

				for (int j = 0; j < highlighted_area_size[col][row]; j++) {
					Color color = trail_color[col];
					Driver_LED_SetColor(
						highlighted_area[col][row][j].col,
						highlighted_area[col][row][j].row,
						(Color) {
							0x2f + i * (color.red - 0x2f) / (TRAIL_SIZE - 1),
							0x2f + i * (color.green - 0x2f) / (TRAIL_SIZE - 1),
							0x2f + i * (color.blue - 0x2f) / (TRAIL_SIZE - 1),
						});
				}
            }
            trail_start = (trail_start + 1) % TRAIL_SIZE;

			if (ldr1_goal || ldr2_goal)
			{
				GameInfo.playerScoreA++;
				App_StateMachine_SetState(GAMESTATE_SCORE_A);
			}
			if (ldr3_goal || ldr4_goal)
			{
				GameInfo.playerScoreB++;
				App_StateMachine_SetState(GAMESTATE_SCORE_B);
			}

			break;
		}

		case (GAMESTATE_RUN_KOTH):
		{
			Driver_Relay_TurnOn();

			if (GameInfo.ticksInState == 1)
			{
				GameInfo.playerScoreA = 256;
				GameInfo.miscData2 = 0x33;
			}

			if (halleffect_cols & 0xFFFF0000)
			{
				// on red side
				GameInfo.miscData = 1;
			}
			else if (halleffect_cols & 0x0000FFFF)
			{
				// on blue side
				GameInfo.miscData = 2;
			}

			if ((GameInfo.ticksInState % 250) == 0)
			{
				if (GameInfo.miscData2 > 1) GameInfo.miscData2 -= 1;
			}

			if ((GameInfo.ticksInState % GameInfo.miscData2) == 0)
			{
				// update score
				if (GameInfo.miscData == 1)
				{
					GameInfo.playerScoreA += 1;
				}
				else if (GameInfo.miscData == 2)
				{
					GameInfo.playerScoreA -= 1;
				}
			}

			// update table display
			uint8_t num_cols_red = GameInfo.playerScoreA >> 4;
			uint8_t num_rows_red = GameInfo.playerScoreA & 15;
//			uint8_t num_cols_blue = GameInfo.playerScoreB >> 4;
//			uint8_t num_rows_blue = GameInfo.playerScoreB & 15;

//			if ((GameInfo.ticksInState & 0x7F) == 0x7F)
//			{
//				// update blinking LED
//				GameInfo.miscData = ~GameInfo.miscData;
//			}

			for (int col = 0; col < num_cols_red; col++) {
				for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
					Driver_LED_SetColor(col, row, (Color) {0xff, 0x00, 0x00});
				}
			}

			for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
				if (row < num_rows_red)
				{
					Driver_LED_SetColor(num_cols_red, row, (Color) {0xff, 0x00, 0x00});
				}
				else
				{
					Driver_LED_SetColor(num_cols_red, row, (Color) {0x00, 0x00, 0xff});
				}
			}

			for (int col = num_cols_red + 1; col < LED_MATRIX_COL_NUM; col++) {
				for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
					Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0xff});
				}
			}

			if (GameInfo.playerScoreA == 512)
			{
				App_StateMachine_SetState(GAMESTATE_WIN_A);
			}
			else if (GameInfo.playerScoreA == 0)
			{
				App_StateMachine_SetState(GAMESTATE_WIN_B);
			}

			break;
		}

		case (GAMESTATE_SCORE_A):
		{
			// TODO animation and OLED
			if (GameInfo.playerScoreA >= GameInfo.winScore)
			{
				App_StateMachine_SetState(GAMESTATE_WIN_A);
				break;
			}
			else
			{
				for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0xff, 0x00, 0x00});
					}
				}

				// check if we should leave state
				if (GameInfo.ticksInState > 1000)
				{
					App_StateMachine_SetState(GAMESTATE_RUN_NORMAL);
					break;
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
				break;
			}
			else
			{
				for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
					for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
						Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0xff});
					}
				}

				// check if we should leave state
				if (GameInfo.ticksInState > 1000)
				{
					App_StateMachine_SetState(GAMESTATE_RUN_NORMAL);
					break;
				}
			}
			break;
		}

		case (GAMESTATE_WIN_A):
		{
			// TODO animation

			uint_fast32_t sparkleIndex = rand();
			for (uint_fast8_t i = 0; i < 4; i++) {
				sparkleIndex += LED_MATRIX_PIXEL_COUNT / 3;
				sparkleIndex %= LED_MATRIX_PIXEL_COUNT;
				Color color;

				uint_fast8_t colorIndex = rand() % 4;
				if (colorIndex == 0) color = (Color) {0x3F, 0x00, 0x00};
				else if (colorIndex == 1 || colorIndex == 2) color = (Color) {0xFF, 0x00, 0x00};
				else color = (Color) {0x00, 0x00, 0x00};

				Driver_LED_SetColor(sparkleIndex % LED_MATRIX_COL_NUM, sparkleIndex / LED_MATRIX_COL_NUM, color);
			}

			if (GameInfo.ticksInState > 2000)
			{
				GameInfo.playerScoreA = 0;
				GameInfo.playerScoreB = 0;
				App_StateMachine_SetState(GAMESTATE_IDLE);
				break;
			}
			break;
		}

		case (GAMESTATE_WIN_B):
		{
			uint_fast32_t sparkleIndex = rand();
			for (uint_fast8_t i = 0; i < 4; i++) {
				sparkleIndex += LED_MATRIX_PIXEL_COUNT / 3;
				sparkleIndex %= LED_MATRIX_PIXEL_COUNT;
				Color color;

				uint_fast8_t colorIndex = rand() % 4;
				if (colorIndex == 0) color = (Color) {0x00, 0x00, 0x3F};
				else if (colorIndex == 1 || colorIndex == 2) color = (Color) {0x00, 0x00, 0xFF};
				else color = (Color) {0x00, 0x00, 0x00};

				Driver_LED_SetColor(sparkleIndex % LED_MATRIX_COL_NUM, sparkleIndex / LED_MATRIX_COL_NUM, color);
			}

			if (GameInfo.ticksInState > 2000)
			{
				GameInfo.playerScoreA = 0;
				GameInfo.playerScoreB = 0;
				App_StateMachine_SetState(GAMESTATE_IDLE);
				break;
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
	if (new_state == GAMESTATE_IDLE || new_state == GAMESTATE_SLEEP)
	{
		Driver_Display_Clear(DISPLAY_ALL);
		Driver_Relay_TurnOff();
	}
	else if (new_state == GAMESTATE_WAIT_A || new_state == GAMESTATE_WAIT_B)
	{
		Driver_Display_ShowScore(DISPLAY_ALL, GameInfo.playerScoreA, GameInfo.playerScoreB);
	}
	else
	{
		Driver_Display_ShowScore(DISPLAY_ALL, GameInfo.playerScoreA, GameInfo.playerScoreB);
	}
	Driver_Encoder_PollButton();
	// Clear globals
	GameInfo.ticksInState = 0;
	GameInfo.currFrame = 0;
	GameInfo.miscData = 0;
	for (int i = 0; i < TRAIL_SIZE; i++) {
		trail_on[i] = false;
	}

	Driver_LED_Clear();
	for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
		for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
			Driver_LED_SetColor(col, row, (Color) {0x00, 0x00, 0x00});
		}
	}

//	for (int col = 0; col < LED_MATRIX_COL_NUM; col++) {
//		for (int row = 0; row < LED_MATRIX_ROW_NUM; row++) {
//			Driver_LED_SetColor(col, row, (Color) {0xff, 0xff, 0xff});
//		}
//	}

	// Switch states
	GameInfo.currGameState = new_state;
}
