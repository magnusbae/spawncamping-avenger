

#ifndef MENU_H_
#define MENU_H_


typedef struct {
	enum {UNSELECTED,SELECTED} isSelected;
	char* name;
	struct menuOption* subMenu;	
} menuOption;

void menu_display(menuOption menu[], int length, int* changed);

#endif /* MENU_H_ */