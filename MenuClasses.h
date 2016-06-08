/*
 * MenuClasses.c
 *
 * Created: 12.05.2016 16:05:51
 *  Author: Andre Eberhard
 */ 


#ifndef __MENUCLASSES_H__
#define __MENUCLASSES_H__

#include "Arduino.h"
#include <string.h>


class Screen{
	public:
	char* title;
	byte priority;	
	
	
	Screen();
	~Screen();
	
	void addButton();
	void addText();
};


class Menu{
	public:
	String title;
	Menu **subMenuPtr;
	Screen **screenPtr;
	byte numberOfSubMenus;
	byte numberOfScreens;
	byte priority;
	Menu* parent;
	
	Menu();
	~Menu();
	
	Menu* addSubMenu(char* name);
	Screen* addScreen(char * name);
	byte getOverallItemNumber();
};

#endif //__MENUCLASSES_H__