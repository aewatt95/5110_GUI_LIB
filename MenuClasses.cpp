#include "MenuClasses.h"

Menu::Menu(){
	title = "Main";
	numberOfSubMenus = 0;
	numberOfScreens = 0;
	priority = 0;
	parent = NULL;
	subMenuPtr = (Menu **) malloc(sizeof(Menu **));
	screenPtr = (Screen **) malloc(sizeof(Screen **));
};
Menu::~Menu(){};

Menu* Menu::addSubMenu(char* name){
	subMenuPtr[numberOfSubMenus] = new Menu();
	subMenuPtr[numberOfSubMenus]->title = name;
	subMenuPtr[numberOfSubMenus]->parent = this;
	subMenuPtr[numberOfSubMenus]->priority = this->getOverallItemNumber();
	
	numberOfSubMenus++;
	subMenuPtr = (Menu **) realloc(subMenuPtr, numberOfSubMenus * sizeof(**subMenuPtr));
	return subMenuPtr[numberOfSubMenus - 1];
};
	


Screen* Menu::addScreen(char * name){
	screenPtr[numberOfScreens] = new Screen();
	screenPtr[numberOfScreens]->title = name;
	numberOfScreens++;
	screenPtr = (Screen **) realloc(screenPtr, numberOfScreens * sizeof(**screenPtr));
	return screenPtr[numberOfScreens - 1];
}


byte Menu::getOverallItemNumber(){
	return (numberOfScreens + numberOfSubMenus);
};