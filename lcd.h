#ifndef LCD_H
#define LCD_H

#include "stm32f1xx_hal.h"

// LCD I2C Address (7-bit address for PCF8574)
#define LCD_ADDR 0x27  // Change this address if needed

// LCD Commands
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x04
#define LCD_DISPLAY_CTRL    0x08
#define LCD_FUNCTION_SET    0x20
#define LCD_SET_CURSOR      0x80

// Function Prototypes
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_PulseEnable(void);
void LCD_WriteByte(uint8_t value);
void LCD_Send(uint8_t value, uint8_t mode);

extern I2C_HandleTypeDef hi2c1;  // I2C Handle

#endif /* LCD_H */
