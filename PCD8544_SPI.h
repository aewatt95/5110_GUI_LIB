/********************************************************************
 This is a "Fast PCD8544 Library". It is designed to be used with
 Nokia 5110 type of display, driven by the PCD8544 controller.
 This library uses hardware SPI of your Arduino microcontroller,
 and does not supprt 'software SPI' mode.
 
 Written by Arthur Liberman (aka 'The Coolest'). http://www.alcpu.com
 Special thanks goes out to 'robtillaart' for his help with debugging
 and optimization.

 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
********************************************************************/

// Version 1.2 -- Sept. 15, 2013

#pragma once
#include "Arduino.h"

// It is very important to wire the LCD correctly. 
// The following is the default wiring configuration for an Atmega168/328 based Arduino:
// PIN_DC    D8 
// PIN_RESET D9 
// PIN_SCE   D10 
// PIN_SDIN  D11 
// PIN_SCLK  D13 

// You may change pins DC, Reset and SCE to map them to different pins on your Arduino board. 
// Please keep SDIN and SCLK connected to your Arduino's MOSI and SCK (hardware SPI) pins.
// You can remap the LCD control pins by changing the following '#define' values:
// Please refer to the pin/port mapping for your Arduino device:
// http://arduino.cc/en/Hacking/PinMapping168
// http://arduino.cc/en/Hacking/PinMapping2560
// Pins labeled Px (PB, PC, etc.) stand for PORTx.
// The current version of this library only allows the use of one port for the control pins.
// Pins are mapped in binary order. Valid values are 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80.
// Px0 is the LSB pin of the port, which in turn is represented by the hexadecimal number 0x01. 
// Px7 is the MSB pin of the port, which in turn is represented by the hexadecimal number 0x80.

// This is the port where the LCD is connected.
#define PCD8544_PORT		PORTB
#define PCD8544_DDR			DDRB	// Should be DDRx, x = port name (B, C, D, etc.)
						
#define PIN_DC				0x01	// D8
#define PIN_RESET			0x02	// D9
#define PIN_CE				0x04	// D10
#define PINS_CE_DC			(PIN_DC | PIN_CE)

// When DC is '1' the LCD expects data, when it is '0' it expects a command.
#define PCD8544_COMMAND		0 
#define PCD8544_DATA		PIN_DC

// You may find a different size screen, but this one is 84 by 48 pixels
#define PCD8544_X_PIXELS	84
#define PCD8544_Y_PIXELS	48
#define PCD8544_ROWS		6

#define BUF_LEN				504 // 84 * 6 (6 rows of 8 bits)

// Functions gotoXY, writeBitmap, renderString, writeLine and writeRect
// will return PCD8544_SUCCESS if they succeed and PCD8544_ERROR if they fail.
#define PCD8544_SUCCESS		1
#define PCD8544_ERROR		0


class PCD8544_SPI : public Print
{
public:
	PCD8544_SPI();	
	
	void begin(bool invert = false);
	void begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias);
	void clear();
	uint8_t gotoXY(uint8_t x, uint8_t y);
	virtual size_t write(uint8_t uint8_t);
	uint8_t writeBitmap(const uint8_t *bitmap[], uint8_t x, uint8_t y, uint8_t width, uint8_t height);

private:

	//void init(void);
	void writeLcd(uint8_t dataOrCommand, uint8_t data);
	void writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count);

	uint8_t m_Column;
	uint8_t y_Position;
	uint8_t m_Line;
	void advanceXY(uint8_t columns);
};

class PCD8544_SPI_FB : public Print
{
public:
	PCD8544_SPI_FB();	

	// Call a render method after any print/write methods are called.
	// For best perofrmance aggragate all writes before calling a render method.
	void renderAll();
	uint8_t renderString(uint8_t x, uint8_t y, uint16_t length);
	
	void setPixel(uint8_t x, uint8_t y, uint8_t value);
	
	// WriteLine currently only supports horizontal and vertical lines.
	uint8_t writeLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
	uint8_t writeRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill = false);
	
	void begin(bool invert = false);
	void begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias);
	void clear(bool render = true);
	uint8_t gotoXY(uint8_t x, uint8_t y);
	//ADDED BY ANDRE
	void calcmPosition();
	virtual size_t write(uint8_t uint8_t);
	uint8_t writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

private:

	//void init(void);
	void writeLcd(uint8_t dataOrCommand, uint8_t data);
	void writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count);

	inline void swap(uint8_t &a, uint8_t &b);
	uint16_t m_Position;
	uint8_t x_Position;
	uint8_t y_Position;
	uint8_t m_Buffer[BUF_LEN];
};

//This table contains the hex values that represent pixels
//for a font that is 5 pixels wide and 8 pixels high
//CHANGED!!!
static const PROGMEM uint8_t ASCII[][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char
	,{0x00, 0x00, 0x17, 0x00, 0x00,}            // Code for char !
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char "
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char #
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char $
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char %
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char &
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char '
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char (
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char )
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char *
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char +
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ,
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char -
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char .
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char /
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 0
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 1
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 2
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 3
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 4
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 5
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 6
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 7
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 8
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char 9
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char :
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ;
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char <
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char =
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char >
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ?
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char @
	,{0x1F, 0x05, 0x05, 0x05, 0x1F,}            // Code for char A
	,{0x1F, 0x15, 0x15, 0x15, 0x0E,}            // Code for char B
	,{0x0E, 0x11, 0x11, 0x11, 0x11,}            // Code for char C
	,{0x1F, 0x11, 0x11, 0x11, 0x0E,}            // Code for char D
	,{0x1F, 0x15, 0x15, 0x15, 0x15,}            // Code for char E
	,{0x1F, 0x05, 0x05, 0x05, 0x05,}            // Code for char F
	,{0x0E, 0x11, 0x11, 0x15, 0x0D,}            // Code for char G
	,{0x1F, 0x04, 0x04, 0x04, 0x1F,}            // Code for char H
	,{0x00, 0x00, 0x1F, 0x00, 0x00,}            // Code for char I
	,{0x08, 0x10, 0x10, 0x10, 0x0F,}            // Code for char J
	,{0x1F, 0x04, 0x0A, 0x0A, 0x11,}            // Code for char K
	,{0x1F, 0x10, 0x10, 0x10, 0x10,}            // Code for char L
	,{0x1F, 0x02, 0x04, 0x02, 0x1F,}            // Code for char M
	,{0x1F, 0x02, 0x04, 0x08, 0x1F,}            // Code for char N
	,{0x0E, 0x11, 0x11, 0x11, 0x0E,}            // Code for char O
	,{0x1F, 0x05, 0x05, 0x05, 0x02,}            // Code for char P
	,{0x0E, 0x11, 0x11, 0x19, 0x0E,}            // Code for char Q
	,{0x1F, 0x05, 0x05, 0x05, 0x1A,}            // Code for char R
	,{0x12, 0x15, 0x15, 0x15, 0x09,}            // Code for char S
	,{0x01, 0x01, 0x1F, 0x01, 0x01,}            // Code for char T
	,{0x0F, 0x10, 0x10, 0x10, 0x0F,}            // Code for char U
	,{0x03, 0x0C, 0x10, 0x0C, 0x03,}            // Code for char V
	,{0x1F, 0x08, 0x04, 0x08, 0x1F,}            // Code for char W
	,{0x11, 0x0A, 0x04, 0x0A, 0x11,}            // Code for char X
	,{0x11, 0x0A, 0x04, 0x02, 0x01,}            // Code for char Y
	,{0x11, 0x19, 0x15, 0x13, 0x11,}            // Code for char Z
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char [
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char BackSlash
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ]
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ^
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char _
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char `
	,{0x1F, 0x05, 0x05, 0x05, 0x1F,}            // Code for char A
	,{0x1F, 0x15, 0x15, 0x15, 0x0E,}            // Code for char B
	,{0x0E, 0x11, 0x11, 0x11, 0x11,}            // Code for char C
	,{0x1F, 0x11, 0x11, 0x11, 0x0E,}            // Code for char D
	,{0x1F, 0x15, 0x15, 0x15, 0x15,}            // Code for char E
	,{0x1F, 0x05, 0x05, 0x05, 0x05,}            // Code for char F
	,{0x0E, 0x11, 0x11, 0x15, 0x0D,}            // Code for char G
	,{0x1F, 0x04, 0x04, 0x04, 0x1F,}            // Code for char H
	,{0x00, 0x00, 0x1F, 0x00, 0x00,}            // Code for char I
	,{0x08, 0x10, 0x10, 0x10, 0x0F,}            // Code for char J
	,{0x1F, 0x04, 0x0A, 0x0A, 0x11,}            // Code for char K
	,{0x1F, 0x10, 0x10, 0x10, 0x10,}            // Code for char L
	,{0x1F, 0x02, 0x04, 0x02, 0x1F,}            // Code for char M
	,{0x1F, 0x02, 0x04, 0x08, 0x1F,}            // Code for char N
	,{0x0E, 0x11, 0x11, 0x11, 0x0E,}            // Code for char O
	,{0x1F, 0x05, 0x05, 0x05, 0x02,}            // Code for char P
	,{0x0E, 0x11, 0x11, 0x19, 0x0E,}            // Code for char Q
	,{0x1F, 0x05, 0x05, 0x05, 0x1A,}            // Code for char R
	,{0x12, 0x15, 0x15, 0x15, 0x09,}            // Code for char S
	,{0x01, 0x01, 0x1F, 0x01, 0x01,}            // Code for char T
	,{0x0F, 0x10, 0x10, 0x10, 0x0F,}            // Code for char U
	,{0x03, 0x0C, 0x10, 0x0C, 0x03,}            // Code for char V
	,{0x1F, 0x08, 0x04, 0x08, 0x1F,}            // Code for char W
	,{0x11, 0x0A, 0x04, 0x0A, 0x11,}            // Code for char X
	,{0x11, 0x0A, 0x04, 0x02, 0x01,}            // Code for char Y
	,{0x11, 0x19, 0x15, 0x13, 0x11,}            // Code for char Z
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char {
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char |
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char }
	,{0x00, 0x00, 0x00, 0x00, 0x00,}            // Code for char ~
	,{0x00, 0x00, 0x00, 0x00, 0x00 }            // Code for char 
};