#ifndef INC_DRIVER_GOAL_H_
#define INC_DRIVER_GOAL_H_

#include <stdbool.h>

typedef enum {
	LDR1IN,
	LDR2IN,
	LDR3IN,
	LDR4IN
} GoalSensor;

bool Driver_Goal_Poll(GoalSensor goalSensor);

#endif /* INC_DRIVER_GOAL_H_ */
