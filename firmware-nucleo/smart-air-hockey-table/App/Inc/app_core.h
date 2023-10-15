#ifndef INC_APP_CORE_H_
#define INC_APP_CORE_H_

#include <inttypes.h>

void App_Init();
void TIM6_Handler();
void TIM7_Handler();
uint32_t App_GetTicksElapsed();

#endif /* INC_APP_CORE_H_ */
