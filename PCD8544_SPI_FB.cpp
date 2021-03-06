#include "PCD8544_SPI.h"
#include "Arduino.h"
#include <SPI.h>

PCD8544_SPI_FB::PCD8544_SPI_FB()
{
}

void PCD8544_SPI_FB::begin(bool invert)
{
	this->begin(invert, 0xB0, 0x04, 0x12);
}

void PCD8544_SPI_FB::begin(bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias)
{
	PCD8544_PORT |= (PIN_DC | PIN_RESET | PIN_CE);
	PCD8544_DDR |= (PIN_DC | PIN_RESET | PIN_CE);
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	
	// LCD init section:
	
	uint8_t invertSetting = invert ? 0x0D : 0x0C;
	// Must reset LCD first!
	PCD8544_PORT &= ~PIN_RESET;
	PCD8544_PORT |= PIN_RESET;

	this->writeLcd(PCD8544_COMMAND, 0x21); //Tell LCD that extended commands follow
	this->writeLcd(PCD8544_COMMAND, vop); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
	this->writeLcd(PCD8544_COMMAND, tempCoef); //Set Temp coefficent
	this->writeLcd(PCD8544_COMMAND, bias); //LCD bias mode 1:48: Try 0x13 or 0x14. Mine works best with 1:65/1:65

	this->writeLcd(PCD8544_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
	this->writeLcd(PCD8544_COMMAND, invertSetting); //Set display control, normal mode. 0x0D for inverse

	this->clear();
	
	this->x_Position = 0;
	this->y_Position = 0;
}

//Andre: Changed to write at x, y Position in Pixels
//TESTED
size_t PCD8544_SPI_FB::write(uint8_t data)
{
	// Non-ASCII characters are not supported.
	if (data < 0x20 || data > 0x7F) return 0;
		for(byte i = 0; i < 5; i++){
			*(this->m_Buffer + this->m_Position + i) |= pgm_read_byte(&(ASCII[data - 0x20][i])) << (y_Position % 8);
			*(this->m_Buffer + this->m_Position + i + PCD8544_X_PIXELS) |= pgm_read_byte(&(ASCII[data - 0x20][i])) >> (8 - (y_Position % 8));
	}
	this->x_Position += 6;
	if(x_Position + 4 > PCD8544_X_PIXELS){
		 y_Position += 6;
		 x_Position = 0;
	}
	calcmPosition();
	return 0;
}

void PCD8544_SPI_FB::calcmPosition(){
	m_Position =  x_Position + ((y_Position / 8) * PCD8544_X_PIXELS);
}

void PCD8544_SPI_FB::clear(bool render)
{
	memset(this->m_Buffer, 0x00, sizeof(this->m_Buffer));
	if (render)
		this->renderAll();
	this->gotoXY(0, 0);
}

//Andre: Y is now in Pixels, not in Blocks.
//TESTED
uint8_t PCD8544_SPI_FB::gotoXY(uint8_t x, uint8_t y) 
{	
	if (x >= PCD8544_X_PIXELS || y >= PCD8544_Y_PIXELS) return PCD8544_ERROR;
	this->writeLcd(PCD8544_COMMAND, 0x80 | x);  // Column.
	this->writeLcd(PCD8544_COMMAND, 0x40 | (y/8));  // Row.
	this->y_Position = y;
	this->x_Position = x;
	this->calcmPosition();
	return PCD8544_SUCCESS;
}

//Andre: Change to fit 84*48 coordinates
//TODO: Don't use *pos two times if it's not necesary
//UNTESTED
uint8_t PCD8544_SPI_FB::writeBitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{		
	if (this->gotoXY(x, y) == PCD8544_ERROR) return PCD8544_ERROR;
		
	for(uint8_t j = 0; j <= height/8; j++){
		for (uint8_t i = 0; i < width; i++)
		{
			//*(m_Buffer + m_Position + (PCD8544_X_PIXELS * j)) |= 
			this->m_Buffer[m_Position] |= *(bitmap + (j* width) + i) << (y_Position % 8);
			this->m_Buffer[m_Position + PCD8544_X_PIXELS] |=  *(bitmap + (j* width) + i) >> (8- (y_Position % 8));
			x_Position++;
			calcmPosition();
		}
		y_Position++;
		calcmPosition();
	}
	
	return PCD8544_SUCCESS;
}

void PCD8544_SPI_FB::renderAll()
{	
	this->gotoXY(0, 0);
	this->writeLcd(PCD8544_DATA, this->m_Buffer, BUF_LEN);
}

uint8_t PCD8544_SPI_FB::renderString(uint8_t x, uint8_t y, uint16_t length)
{
	if (this->gotoXY(x, y) == PCD8544_ERROR) return PCD8544_ERROR;
	length *= 6;
	this->writeLcd(PCD8544_DATA, this->m_Buffer + this->m_Position, length);
	this->m_Position += length;
	return PCD8544_SUCCESS;
}

void PCD8544_SPI_FB::setPixel(uint8_t x, uint8_t y, uint8_t value)
{
	if (x >= PCD8544_X_PIXELS || y >= PCD8544_Y_PIXELS) return;
	uint8_t bank = y / 8;
	uint8_t bitMask = 1 << (y % 8);
	uint8_t &byte = this->m_Buffer[(PCD8544_X_PIXELS * bank) + x];
	if (value)
		byte |= bitMask;
	else
		byte &= ~bitMask;
}

uint8_t PCD8544_SPI_FB::writeLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	if (x1 == x2 || y1 == y2)
	{
		if (y1 > y2)
			swap(y1, y2);
		if (x1 > x2)
			swap(x1, x2);
		return this->writeRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
	}
	else
		return PCD8544_ERROR;
}

void PCD8544_SPI_FB::swap(uint8_t &a, uint8_t &b)
{
	uint8_t temp = a;
	a = b;
	b = temp;
}

uint8_t PCD8544_SPI_FB::writeRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool fill)
{
	if (x >= PCD8544_X_PIXELS || y >= PCD8544_Y_PIXELS || width == 0 || height == 0) return PCD8544_ERROR;

	// Calculate the bitmasks for the pixels.
	uint8_t bottom = y + height - 1;
	uint8_t bankTop = y / 8;
	uint8_t bankBottom = bottom / 8;
	uint8_t bitMaskTop = 0x01;
	uint8_t bitMaskBottom = 0x80;
	uint8_t bitMaskTopFill = 0xFF;
	uint8_t bitMaskBottomFill = 0xFF;
	bitMaskTop <<= (y % 8);
	bitMaskBottom >>= 7 - (bottom % 8);
	bitMaskTopFill <<= (y % 8);
	bitMaskBottomFill >>= 7 - (bottom % 8);

	// When fill is selected, we'll use the FillMask.
	if (fill)
	{
		bitMaskTop = bitMaskTopFill;
		bitMaskBottom = bitMaskBottomFill;
	}

	// When the rectangle fits in a single bank, we AND the top and bottom masks
	// So that we only fill the required area on the LCD.
	if (bankTop == bankBottom)
	{
		bitMaskTop = fill ? bitMaskTop & bitMaskBottom : bitMaskTop | bitMaskBottom;
		bitMaskTopFill &= bitMaskBottomFill;
	}
	this->gotoXY(x, bankTop * 8);

	// Write the left 'side' of the rectangle on the top bank.
	this->m_Buffer[this->m_Position++] |= bitMaskTopFill;
	// Write a line or a fill.
	for (uint8_t i = 1; i < width-1; i++)
		this->m_Buffer[this->m_Position++] |= bitMaskTop;
	// Write the right 'side' of the rectangle on the top bank.
	if (width > 1)
		this->m_Buffer[this->m_Position++] |= bitMaskTopFill;

	this->m_Position += (PCD8544_X_PIXELS - width);

	// Write a fill across the middle banks or two sides of the rectangle.
	if (bankBottom - bankTop > 1)
	{
			for (uint8_t i = bankTop + 1; i < bankBottom; i++)
			{
				if (fill)
					memset(this->m_Buffer + this->m_Position, 0xFF, width);
				else
				{
					this->m_Buffer[this->m_Position] = 0xFF;
					this->m_Buffer[this->m_Position+width-1] = 0xFF;
				}
				this->m_Position += PCD8544_X_PIXELS;
			}
	}
	// If the rectangle spans across more than one bank,
	// apply the same logic for the bottom as the top.
	if (bankBottom > bankTop)
	{
		this->m_Buffer[this->m_Position++] |= bitMaskBottomFill;
		for (uint8_t i = 1; i < width-1; i++)
			this->m_Buffer[this->m_Position++] |= bitMaskBottom;
		if (width > 1)
			this->m_Buffer[this->m_Position++] |= bitMaskBottomFill;
	}
	return PCD8544_SUCCESS;
}

void PCD8544_SPI_FB::writeLcd(uint8_t dataOrCommand, const uint8_t *data, uint16_t count)
{
	PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | dataOrCommand;
	//for (uint16_t i = 0; i < count; i++)
	//	SPI.transfer(data[i]);
    for (uint16_t i = count; i > 0; i--)
		SPI.transfer(data[count-i]);
	PCD8544_PORT |= PIN_CE;
}

void PCD8544_SPI_FB::writeLcd(uint8_t dataOrCommand, uint8_t data)
{
	PCD8544_PORT = (PCD8544_PORT & ~PINS_CE_DC) | dataOrCommand;
	SPI.transfer(data);
	PCD8544_PORT |= PIN_CE;
}
