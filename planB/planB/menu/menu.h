

#ifndef MENU_H_
#define MENU_H_


typedef struct {
	enum {UNSELECTED,SELECTED} isSelected;
	char* name;
	struct menuOption* subMenu;	
} menuOption;

void menu_display(menuOption menu[], int length, int* changed);
void menu_display_RAMV2(menuOption menu[], int length);

#endif /* MENU_H_ */