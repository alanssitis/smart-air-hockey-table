#ifndef INC_APP_CORE_H_
#define INC_APP_CORE_H_

#include <inttypes.h>
#include <limits.h>

#define BIT_MASK(__TYPE__, __ONE_COUNT__) \
    ((__TYPE__) (-((__ONE_COUNT__) != 0))) \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

void App_Init();
void TIM6_Handler();
void TIM7_Handler();

#endif /* INC_APP_CORE_H_ */
