#include "driver_goaldetection.h"

#include "stm32u5xx_ll_exti.h"

static volatile ScoreState currScoreState = ScoreState_Clear;

void Driver_GoalDetection_Init(bool isActive)
{
	LL_EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_6 | LL_EXTI_LINE_13 | LL_EXTI_LINE_14 | LL_EXTI_LINE_15;
	EXTI_InitStruct.LineCommand = isActive ? ENABLE : DISABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
	LL_EXTI_Init(&EXTI_InitStruct);
}

void Driver_GoalDetection_SetScoreState(ScoreState newScoreState)
{
	currScoreState = newScoreState;
}

ScoreState Driver_GoalDetection_GetScoreState()
{
	return currScoreState;
}
