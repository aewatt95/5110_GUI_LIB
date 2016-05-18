/*
 * _5110_GUI_LIB.ino
 *
 * Created: 5/11/2016 11:26:35 AM
 * Author: Andre Eberhard
 */ 
#include "DisplayGUI.h"
#include <SPI/SPI.h>
#include "PCD8544_SPI.h"


// Use Pins 
// PIN_DC    D8
// PIN_RESET D9
// PIN_SCE   D10
// PIN_SDIN  D11
// PIN_SCLK  D13
// for now

DisplayGUI gui;

void setup()
{
	
	gui.addMenu(NULL, "Hello");
	gui.initInterupt();
	
}

void loop()
{

	  /* add main program code here, this code starts again each time it ends */

}

//TODO: Please! Find another solution for this! It's just bad :(
ISR (TIMER1_OVF_vect){
	gui.handleInput();
}
