#include "driver_eeprom.h"

#include "stm32u5xx_ll_gpio.h"
#include "stm32u5xx_ll_spi.h"
#include "stm32u5xx_ll_utils.h"

// EEPROM Chip Select (active low)
#define MEM1CS_GPIO		GPIOB
#define MEM1CS_PIN		LL_GPIO_PIN_12
// Hold signal (active low)
#define MEMHOLD_GPIO	GPIOD
#define MEMHOLD_PIN		LL_GPIO_PIN_8
// Write protect signal (active low)
#define MEMW_GPIO		GPIOD
#define MEMW_PIN		LL_GPIO_PIN_9

// Note: Must set datawidth before this
static inline void transmit_data8_blocking(uint8_t data)
{
	LL_SPI_Disable(SPI2);
	LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_Enable(SPI2);

	LL_SPI_TransmitData8(SPI2, data);
	LL_SPI_StartMasterTransfer(SPI2);
	while (!LL_SPI_IsActiveFlag_TXC(SPI2));

	LL_SPI_Disable(SPI2);
	LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_32BIT);
	LL_SPI_Enable(SPI2);
}

static inline void transmit_data32_blocking(uint32_t data)
{
	LL_SPI_TransmitData32(SPI2, data);
	LL_SPI_StartMasterTransfer(SPI2);
	while (!LL_SPI_IsActiveFlag_TXC(SPI2));
}

static inline uint32_t receive_data32_blocking()
{
	uint32_t data = LL_SPI_ReceiveData32(SPI2);
	return data;
}

static inline void transmit_start()
{
	LL_SPI_Disable(SPI2);
	LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_32BIT);
	LL_SPI_SetBaudRatePrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV16);
	LL_SPI_SetTransferBitOrder(SPI2, LL_SPI_MSB_FIRST);
	LL_SPI_Enable(SPI2);
	LL_GPIO_ResetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);
}

static inline void transmit_stop()
{
	LL_GPIO_SetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);
	LL_SPI_SuspendMasterTransfer(SPI2);
	LL_SPI_ClearFlag_SUSP(SPI2);
	LL_SPI_Disable(SPI2);
	LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_Enable(SPI2);
	LL_SPI_StartMasterTransfer(SPI2);
}

void Driver_Eeprom_Init()
{
	LL_GPIO_SetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);

	LL_GPIO_SetOutputPin(MEMHOLD_GPIO, MEMHOLD_PIN);

	// Disable memory protection
	LL_GPIO_SetOutputPin(MEMW_GPIO, MEMW_PIN);
}

void Driver_Eeprom_Read_Data(uint32_t base_addr, void * dataPointer, uint_fast16_t len_data_doublewords) {
	transmit_start();

	// FREAD & Address & Dummy Byte
	transmit_data32_blocking(0x0B000000 | base_addr);
	transmit_data8_blocking(0x00); // dummy byte
	// Data
	while (len_data_doublewords > 0)
	{
		transmit_data32_blocking(0);
		uint32_t x = receive_data32_blocking();
		*(uint32_t *)dataPointer = x;
		dataPointer += sizeof(uint32_t);
		len_data_doublewords -= 1;
	}

	transmit_stop();
}

void Driver_Eeprom_Write_Page(uint32_t base_addr, void * dataPointer, uint_fast16_t len_data_doublewords) {

	transmit_start();

	LL_GPIO_ResetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);

	// set WEN
	transmit_data8_blocking(0x06);

	// need both of these to allow setting WEN
	LL_GPIO_SetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);
	LL_GPIO_ResetOutputPin(MEM1CS_GPIO, MEM1CS_PIN);

	// PGWR & Address
	transmit_data32_blocking(0x02000000 | base_addr);
	// Data
	while (len_data_doublewords > 0)
	{
		uint32_t y = *(uint32_t *) dataPointer;
		transmit_data32_blocking(y);
		dataPointer += 4;
		len_data_doublewords -= sizeof(char);
	}
	transmit_stop();

	LL_mDelay(5); // to allow EEPROM to finish write process
}
