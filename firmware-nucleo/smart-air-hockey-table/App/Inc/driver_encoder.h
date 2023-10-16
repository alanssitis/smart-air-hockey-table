#ifndef INC_DRIVER_ENCODER_H_
#define INC_DRIVER_ENCODER_H_

#include <inttypes.h>

void Driver_Encoder_Init();
void Driver_Encoder_Tick();
uint8_t Driver_Encoder_PollButton();
int8_t Driver_Encoder_PollRotation();
void EXTI5_Handler();

#endif /* INC_DRIVER_ENCODER_H_ */
