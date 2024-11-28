#include "lcd.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;  // I2C handle

// Function to send a command to the LCD
void LCD_SendCommand(uint8_t cmd) {
    LCD_Send(cmd, 0);  // 0 indicates command mode
}

// Function to send data to the LCD
void LCD_SendData(uint8_t data) {
    LCD_Send(data, 1);  // 1 indicates data mode
}

// Function to send a value (command or data) to the LCD
void LCD_Send(uint8_t value, uint8_t mode) {
    uint8_t highNibble = (value & 0xF0);  // Extract high nibble
    uint8_t lowNibble = ((value << 4) & 0xF0);  // Extract low nibble

    // Send high nibble
    if (mode) {
        LCD_WriteByte(highNibble | 0x01);  // RS = 1 (data mode)
    } else {
        LCD_WriteByte(highNibble);  // RS = 0 (command mode)
    }
    LCD_PulseEnable();

    // Send low nibble
    if (mode) {
        LCD_WriteByte(lowNibble | 0x01);  // RS = 1 (data mode)
    } else {
        LCD_WriteByte(lowNibble);  // RS = 0 (command mode)
    }
    LCD_PulseEnable();
}

// Function to pulse the enable pin (EN)
void LCD_PulseEnable(void) {
    HAL_Delay(1);  // Wait for stable operation

    // Toggle enable pin (EN) to latch data into the LCD
    uint8_t value = 0x04;  // EN = 1
    LCD_WriteByte(value);  // Send byte to LCD with EN high
    HAL_Delay(1);          // Short delay

    value = 0x00;  // EN = 0
    LCD_WriteByte(value);  // Send byte to LCD with EN low
    HAL_Delay(1);          // Short delay
}

// Function to write a byte to the LCD via I2C
void LCD_WriteByte(uint8_t value) {
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, &value, 1, HAL_MAX_DELAY);
}

// Function to initialize the LCD
void LCD_Init(void) {
    // Initialize the LCD to 4-bit mode, 2-line, 5x8 font
    HAL_Delay(15);  // Wait for LCD to power up
    LCD_SendCommand(0x30);  // 8-bit mode
    HAL_Delay(5);
    LCD_SendCommand(0x30);  // 8-bit mode
    HAL_Delay(1);
    LCD_SendCommand(0x30);  // 8-bit mode
    LCD_SendCommand(0x20);  // Switch to 4-bit mode

    // Set function: 4-bit mode, 2 lines, 5x8 font
    LCD_SendCommand(LCD_FUNCTION_SET | 0x00 | 0x08); // 4-bit mode, 2 lines, 5x8 font
    
    // Display control: turn on display, cursor off, blink off
    LCD_SendCommand(LCD_DISPLAY_CTRL | 0x04 | 0x00);

    // Clear display
    LCD_Clear();

    // Set entry mode: auto increment address, no shift
    LCD_SendCommand(LCD_ENTRY_MODE | 0x02);
}

// Function to clear the LCD
void LCD_Clear(void) {
    LCD_SendCommand(LCD_CLEAR);
    HAL_Delay(2);  // Wait for clear to complete
}

// Function to set the cursor position (row, column)
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;

    // Calculate the address based on row and column
    if (row == 0) {
        address = 0x80 + col;  // First row, 0x80 is base address
    } else if (row == 1) {
        address = 0xC0 + col;  // Second row, 0xC0 is base address
    } else {
        return;  // Invalid row
    }

    LCD_SendCommand(address);  // Set the address of the cursor
}

// Function to print a string on the LCD
void LCD_Print(char* str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}
