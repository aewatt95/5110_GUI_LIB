/*
 * _5110_GUI_LIB.ino
 *
 * Created: 5/11/2016 11:26:35 AM
 * Author: Andre Eberhard
 */ 
#include "DisplayGUI.h"
#include <SPI/SPI.h>
#include "PCD8544_SPI.h"
#include "MenuClasses.h"
#include "MemoryFree.h"

// Use Pins 
// PIN_DC    D8
// PIN_RESET D9
// PIN_SCE   D10
// PIN_SDIN  D11
// PIN_SCLK  D13
// for now

DisplayGUI *gui;
Menu *root, *settings;

void setup()
{
	generateMenus();
	gui = new DisplayGUI();
	gui->setRootMenu(root);
	
	Serial.begin(9600);
	
	pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	attachInterrupt(0,pinAHandler,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
	attachInterrupt(1,pinBHandler,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
	
	gui->drawMenu();

}

void generateMenus(){
	
	root = new Menu();
	root->title = "Hauptmenue";
	
	settings = root->addSubMenu("Einstell.");
	settings->addSubMenu("Rofl");
	settings->addSubMenu("Di");
	settings->addSubMenu("Copter");
	root->addSubMenu("Auschaltm.");
	root->addSubMenu("Einschaltm.");
	root->addSubMenu("Hey Dome");
	root->addSubMenu("was");
	root->addSubMenu("geht");
}

void loop()
{
	Serial.println(freeMemory());
	delay(100);
	  /* add main program code here, this code starts again each time it ends */

}

//TODO: Please! Find another solution for this! It's just bad
void pinAHandler(){
	if(gui) gui->PinA();
	gui->handleInput();
}
void pinBHandler(){
	if(gui) gui->PinB();
	gui->handleInput();
}
