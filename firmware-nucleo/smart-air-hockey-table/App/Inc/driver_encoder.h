#ifndef INC_DRIVER_ENCODER_H_
#define INC_DRIVER_ENCODER_H_

#include <stdbool.h>
#include <inttypes.h>

void Driver_Encoder_Init();
bool Driver_Encoder_PollButton();
int_fast8_t Driver_Encoder_PollRotation();
void Driver_Encoder_SetActive(bool active);
void EXTI5_Handler();

#endif /* INC_DRIVER_ENCODER_H_ */
