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
	//root = new Menu();
	//currentMenu = root;
	currentSelection = 0;
	firstMenuPoint = 0;

	display = new PCD8544_SPI_FB();
	display->begin(false, 0xBB, 0x04, 0x14);
	
	
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
	display->clear();
	if(root == NULL){
		display->gotoXY(0, 0);
		display->print("Error! ROOT UNDEFINED");
	}
	else{
		drawStatusBar();
		drawSlider(currentMenu->getOverallItemNumber(), currentSelection);
		drawEntrys();
		drawCursor();
	}
	display->renderAll();
}

void DisplayGUI::addScreen(Menu *parentMenu, char* screenTitle)
{
	
}

void DisplayGUI::addMenu(Menu *parentMenu, char* menuTitle)
{
	if(parentMenu == NULL) parentMenu = root;
	parentMenu->addSubMenu(menuTitle);
	parentMenu->subMenuPtr[parentMenu->getOverallItemNumber()]->priority = 4;	
}

void DisplayGUI::setRootMenu(Menu* rootMenu){
	this->root = rootMenu;
	this->currentMenu = this->root;
}

//UNTESTED
void DisplayGUI::drawEntrys()
{
	if(currentSelection >= currentMenu->numberOfSubMenus - 1)
		firstMenuPoint = currentSelection - 2;
	else if(currentSelection > 1) 
		firstMenuPoint = currentSelection - 1;
	else 
		firstMenuPoint = 0;
	
	for(byte i = 0; i < 3; i++){
				display->gotoXY(11, (17 + (8*i)));
				if(currentMenu->subMenuPtr[i + firstMenuPoint] != NULL)
					display->print(currentMenu->subMenuPtr[i + firstMenuPoint]->title);
	}
	
}


const byte slider[] = {0b10101010, 0b01010101};
void DisplayGUI::drawSlider(byte numberOfItems, byte selectedItem)
{
	byte maxHeight = 32;
	float actualHeight = (float)maxHeight / (float)numberOfItems;
	for(int i = 0; i < maxHeight; i = i + 8){
		display->writeBitmap(slider, 80, 12 + i, 2, 7);
	}	
	display->writeRect(80, 12 + (actualHeight * currentSelection), 2, (byte)actualHeight, true);
}

//UNTESTED
void DisplayGUI::drawCursor()
{
		display->writeBitmap(CURSOR, 3, (17 + (currentSelection - firstMenuPoint) * 8), 6, 5);
}


void DisplayGUI::drawStatusBar()
{
	drawOutline(DOTTED);
	drawMenuTitle();		
}



void DisplayGUI::drawOutline(LINETYPE linetype)
{
	for(int i = 0; i < PCD8544_X_PIXELS - 10; i = i + 2)
		display->writeBitmap((LINE[linetype]), i, 8, 2 , 2);
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
	display->gotoXY(1,2);
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
	if(currentSelection >= 250) currentSelection = 0;
	if(currentSelection >= currentMenu->numberOfSubMenus - 1)
		currentSelection = currentMenu->numberOfSubMenus - 1;
	
	this->drawMenu();
}


volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent



void DisplayGUI::PinA(){
	cli(); //stop interrupts happening before we read pin values
	reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
	if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
		currentSelection --; //decrement the encoder's position count
		bFlag = 0; //reset flags for the next turn
		aFlag = 0; //reset flags for the next turn
	}
	else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
	sei(); //restart interrupts
}

void DisplayGUI::PinB(){
	cli(); //stop interrupts happening before we read pin values
	reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
	if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
		currentSelection ++; //increment the encoder's position count
		bFlag = 0; //reset flags for the next turn
		aFlag = 0; //reset flags for the next turn
	}
	else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
	sei(); //restart interrupts
}
