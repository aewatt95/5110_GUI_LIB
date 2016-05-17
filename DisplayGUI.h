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

const uint8_t LINE[][2] = {{1, 1}, {0b1, 0b0}, {0b10, 0b01}};

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
	PCD8544_SPI_FB *display;
		
//functions
public:
	DisplayGUI();
	~DisplayGUI();
	void addScreen(Menu *parentMenu, char* screenTitle);
	void addMenu(Menu *parentMenu, char* menuTitle);
	
	void drawMenu();
		void drawEntrys();
		void drawSlider(byte numberOfItems, byte selectedItem);
		void drawCursor(byte selectedItem);
	
	void drawStatusBar();
		void drawOutline(LINETYPE linetype);
		void drawIcon(byte icon[]);
		void drawMenuTitle();
	//GUI Elements
	void drawSwitchButton();
	void drawSlider();
		
protected:
private:
	DisplayGUI( const DisplayGUI &c );
	DisplayGUI& operator=( const DisplayGUI &c );

}; //DisplayGUI

#endif //__DISPLAYGUI_H__
