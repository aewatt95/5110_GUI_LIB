/* 
* DisplayGUI.cpp
*
* Created: 11.05.2016 11:28:58
* Author: Andre Eberhard
*/


#include "DisplayGUI.h"
#include "PCD8544_SPI.h"

// default constructor
DisplayGUI::DisplayGUI()
{
	currentMenu->title = "Main";
	currentSelection = 0;
} //DisplayGUI

// default destructor
DisplayGUI::~DisplayGUI()
{
} //~DisplayGUI

void DisplayGUI::drawMenu()
{
	
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

void DisplayGUI::drawEntrys()
{
	for(int i = (currentSelection % 3); i < (currentSelection % 3) + 3; i++){
		
		if(i == currentMenu->subMenuPtr[i].priority);
			//print currentMenu->subMenuPtr[i].title; 
		if(i == currentMenu->screenPtr[i].priority);
			//print currentMenu->screenPtr[i].title;
	}
}

void DisplayGUI::drawSlider(byte numberOfItems, byte selectedItem)
{
	
}

void DisplayGUI::drawSlider()
{
	
}

void DisplayGUI::drawCursor(byte selectedItem)
{
	byte printPosition = currentSelection % 3;
}

void DisplayGUI::drawStatusBar()
{
	//drawIcon(battery)
}

void DisplayGUI::drawOutline(LINETYPE linetype)
{
	
}

void DisplayGUI::drawIcon(byte icon[])
{
	
}

void DisplayGUI::drawMenuTitle()
{
	
}

void DisplayGUI::drawSwitchButton()
{
	
}

