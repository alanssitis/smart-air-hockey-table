#ifndef INC_DRIVER_ENCODER_H_
#define INC_DRIVER_ENCODER_H_

#include <stdbool.h>
#include <inttypes.h>

void Driver_Encoder_Init();
void Driver_Encoder_Tick();
bool Driver_Encoder_PollButton();
int8_t Driver_Encoder_PollRotation();

#endif /* INC_DRIVER_ENCODER_H_ */
