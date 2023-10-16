#ifndef INC_DRIVER_GOALDETECTION_H_
#define INC_DRIVER_GOALDETECTION_H_

#include <stdbool.h>
#include <inttypes.h>

typedef enum _ScoreState {
	ScoreState_Clear,	// Neither player has scored
	ScoreState_A,		// Player A has scored
	ScoreState_B		// Player B has scored
} ScoreState;

void Driver_GoalDetection_Init(bool isActive);
void Driver_GoalDetection_SetScoreState(ScoreState newScoreState);
ScoreState Driver_GoalDetection_GetScoreState();

#endif /* INC_DRIVER_GOALDETECTION_H_ */
