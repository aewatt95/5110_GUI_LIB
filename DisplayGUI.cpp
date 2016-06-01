/* 
* DisplayGUI.cpp
*
* Created: 11.05.2016 11:28:58
* Author: Andre Eberhard
*/


#include "DisplayGUI.h"
#include "PCD8544_SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// default constructor
DisplayGUI::DisplayGUI()
{
	currentMenu->title = "Main";
	currentSelection = 0;
	firstMenuPoint = 0;
	
	
	//TODO: Implement begin() with additional Parameters
	display->begin(false);
	
	//Initalise input varaibles
	lastAValue = digitalRead(pinA);
	lastBValue = digitalRead(pinB);
	lastClickValue = digitalRead(pinClick);
	
} //DisplayGUI

// default destructor
DisplayGUI::~DisplayGUI()
{
	
} //~DisplayGUI

void DisplayGUI::drawMenu()
{
	drawEntrys();
	drawCursor();
}

void DisplayGUI::addScreen(Menu *parentMenu, char* screenTitle)
{
	
}

void DisplayGUI::addMenu(Menu *parentMenu, char* menuTitle)
{
	if(parentMenu == NULL) parentMenu = root;
	parentMenu->addSubMenu(menuTitle);
	parentMenu->subMenuPtr[parentMenu->numberOfSubMenus].priority = parentMenu->getOverallItemNumber();	
}

//UNTESTED
void DisplayGUI::drawEntrys()
{
	//get the first entry that is displayed on the screen
	if(firstMenuPoint < currentSelection)
		firstMenuPoint == currentSelection;
	if(firstMenuPoint + 2 > currentSelection)
		firstMenuPoint = currentSelection - 2;
		
	//Print three entrys beginning with firstMenuPoint
	for(int i = firstMenuPoint; i < firstMenuPoint + 2; i++){
		for(int j = 0; j < currentMenu->getOverallItemNumber(); j++){
			if(currentMenu->subMenuPtr[j].priority == i){
				display->gotoXY(11, 13 + ((firstMenuPoint - i) * 8));
				display->print(currentMenu->screenPtr[j].title);
			}
			//TODO: Add Screens here
		}
	}
}


//UNTESTED
const byte slider[] = {0b10101010, 0b01010101};
void DisplayGUI::drawSlider(byte numberOfItems, byte selectedItem)
{
	byte maxHeight = 32;
	byte actualHeight = maxHeight / numberOfItems;
	for(int i = 0; i < maxHeight / 8; i++){
		display->writeBitmap(slider, 80, 12 + i, 2, 8);
	}	
	display->writeRect(80, 12, 2, actualHeight, true);
}

//UNTESTED
void DisplayGUI::drawCursor()
{
	display->writeBitmap(CURSOR, 3, (11 + (currentSelection - firstMenuPoint) * 8), 5, 1);
}


void DisplayGUI::drawStatusBar()
{
	drawOutline(DOUBLEDOTTED);
	drawMenuTitle();	
}


//UNTESTED
void DisplayGUI::drawOutline(LINETYPE linetype)
{
		for(int i = 0; i < 84; i = i + 2) 
		display->writeBitmap(LINE[linetype], i, 9, 2 , 2);
}

//Draw Icon in the Status Bar
byte lastIconXValue = 0; 

//TODO: Add priority support
//UNTESTED
void DisplayGUI::drawIcon(byte icon[])
{
		display->writeBitmap(icon,( 2 + sizeof(icon) / 8), 2, sizeof(icon) / 8, 5);
		lastIconXValue = sizeof(icon) / 8;
}

void DisplayGUI::drawMenuTitle()
{
	display->gotoXY(1, 1);
	display->print(currentMenu->title);
}


//TODO: Reimplement Inputs for rotary encoder with ACTUAL REAL INTERRUPTS cause it will not work with timerInterrupts
//UNUSED4
void DisplayGUI::initInterupt(){
}

//Clockwise:	A: 0 0 1 1
//				B: 1 0 0 1
//
//C.Clockwise:	A: 1 0 0 1
//				B: 0 0 1 1
//UNTESTED and probably wrong at some point
void DisplayGUI::handleInput(){
	if(lastAValue != digitalRead(pinA)){
		if(lastAValue){
				if(lastBValue) currentSelection++;
				else currentSelection--;
			}
		else{
			if(lastBValue) currentSelection--;
			else currentSelection ++;	
		}
		lastAValue = digitalRead(pinA);
	}
	
	else if(lastBValue != digitalRead(pinB)){
		if(lastBValue){
			if(lastAValue) currentSelection--;
			else currentSelection++;
		}
		else{
			if(lastAValue) currentSelection++;
			else currentSelection --;
		}
		lastAValue = digitalRead(pinA);
	}
	
	if(lastClickValue != digitalRead(pinClick)){
		if(!lastClickValue); //TODO: do something
		}
}