#ifndef INC_DRIVER_EEPROM_H_
#define INC_DRIVER_EEPROM_H_

#include <inttypes.h>

// Signals
// Write protect (driver should turn off protection within write functions)
// Hold (driver should hold when needed within read functions)

// Needed Instructions
// "WREN"
// "WRDI"
// "RDSR"
// "WRSR"
// "FREAD" (NOT "READ" because it is slower :) )
// "PGWR"
// "CHER"
// "RSTEN"
// "RESET"

// Stretch Instructions
// "DPD"
// "RDPD"

// TODO: Look into volatile register to see if buffer loading could be useful

void Driver_Eeprom_Init();
uint8_t Driver_Eeprom_Read_StatusReg();
void Driver_Eeprom_Write_StatusReg(uint8_t data);
void Driver_Eeprom_Write_StatusConfigurationRegs(uint8_t statusReg_data, uint8_t configurationReg_data);
void Driver_Eeprom_Read_Data(uint32_t base_addr);
void Driver_Eeprom_Write_Page(uint32_t base_addr, uint8_t* data, uint_fast16_t len_data);
void Driver_Eeprom_Clear_Chip();
void Driver_Eeprom_Software_Reset();
void Driver_Eeprom_Low_Power_Enter();
void Driver_Eeprom_Low_Power_Exit();

#endif /* INC_DRIVER_EEPROM_H_ */
