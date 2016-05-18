/* 
* DisplayGUI.h
*
* Created: 11.05.2016 11:28:59
* Author: Andre Eberhard
*/


#ifndef __DISPLAYGUI_H__
#define __DISPLAYGUI_H__

#include "Arduino.h"
#include "MenuClasses.cpp"
#include "PCD8544_SPI.h"

//TODO: Move all Bitmaps to external file and ban into PROGMEM >:)
const uint8_t LINE[][2] = {{1, 1}, {0b1, 0b0}, {0b10, 0b01}};
const uint8_t CURSOR[] = {0x0E, 0x0E, 0x0E, 0x1F, 0x0E, 0x04}; // Code for char cursor
	
const uint8_t pinA = 3;
const uint8_t pinB = 4;
const uint8_t pinClick = 5;

class DisplayGUI
{
//variables
public:
	enum LINETYPE {SOLID, DOTTED, DOUBLEDOTTED};
		
protected:

private:
	Menu* root;	
	Menu* currentMenu;
	byte currentSelection;
	
	//Needed for scrolling through Menus
	byte firstMenuPoint;
	
	PCD8544_SPI_FB *display;
	
	//Variables for Input
	bool lastAValue;
	bool lastBValue;
	bool lastClickValue;
		
//functions
public:
	DisplayGUI();
	~DisplayGUI();
	void addScreen(Menu *parentMenu, char* screenTitle);
	void addMenu(Menu *parentMenu, char* menuTitle);
	void initInterupt();
	
	void drawMenu();
		void drawEntrys();
		void drawSlider(byte numberOfItems, byte selectedItem);
		void drawCursor();
	
	void drawStatusBar();
		void drawOutline(LINETYPE linetype);
		void drawIcon(byte icon[]);
		void drawMenuTitle();
	//GUI Elements
	void drawSwitchButton();
	void drawSlider();
	
	void handleInput();
		
protected:
private:
	DisplayGUI( const DisplayGUI &c );
	DisplayGUI& operator=( const DisplayGUI &c );

}; //DisplayGUI

#endif //__DISPLAYGUI_H__
