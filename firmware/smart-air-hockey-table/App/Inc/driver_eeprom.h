#ifndef INC_DRIVER_EEPROM_H_
#define INC_DRIVER_EEPROM_H_

#include <inttypes.h>

// Signals
// Write protect (driver should turn off protection within write functions)
// Hold (driver should hold when needed within read functions)

void Driver_Eeprom_Init();
void Driver_Eeprom_Read_Data(uint32_t base_addr, void * dataPointer, uint_fast16_t len_data_doublewords);
void Driver_Eeprom_Write_Page(uint32_t base_addr, void* data, uint_fast16_t);

#endif /* INC_DRIVER_EEPROM_H_ */
