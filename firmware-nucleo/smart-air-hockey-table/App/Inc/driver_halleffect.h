#ifndef INC_DRIVER_HALLEFFECT_H_
#define INC_DRIVER_HALLEFFECT_H_

#include <inttypes.h>

extern volatile uint32_t halleffect_cols;
extern volatile uint16_t halleffect_rows;

void Driver_HallEffect_PollInputs();

#endif /* INC_DRIVER_HALLEFFECT_H_ */
