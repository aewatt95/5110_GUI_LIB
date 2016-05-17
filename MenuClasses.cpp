/*
 * MenuClasses.c
 *
 * Created: 12.05.2016 16:05:51
 *  Author: Andre Eberhard
 */ 

#include "Arduino.h"
#include <string.h>

class Menu;
class Screen;

class Screen{
	public:
	char* title;
	byte priority;	
	
	Screen(){}
	~Screen(){}
	void addButton(){
		
	}
	
	void addText(){
		
	}
};

class Menu{
	public:
	String title;
	Menu *subMenuPtr;
	Screen *screenPtr;
	byte numberOfSubMenus;
	byte numberOfScreens;
	byte priority;
	Menu* parent;
	
	Menu(){
		title = "Main";
		numberOfSubMenus = 0;
		numberOfScreens = 0;
		priority = 0;
		parent = NULL;
		subMenuPtr = (Menu *) malloc(sizeof(Menu *));
		screenPtr = (Screen *) malloc(sizeof(Screen *));
	}
	~Menu(){}
	
	void addSubMenu(char* name){
		subMenuPtr[numberOfSubMenus].title = name;
		subMenuPtr[numberOfSubMenus].parent = this;
		
		numberOfSubMenus++;
		subMenuPtr = (Menu *) realloc(subMenuPtr, numberOfSubMenus + 1);
		
	}
	void addScreen(char * name){
		screenPtr[numberOfScreens].title = name;
		numberOfScreens++;
		screenPtr = (Screen *) realloc(screenPtr, numberOfScreens + 1);
	}
	byte getOverallItemNumber(){
		return (numberOfScreens + numberOfSubMenus);
	}
};
