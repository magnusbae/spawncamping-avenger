

#ifndef MENU_H_
#define MENU_H_

/**
* The struct for building a menu.
* Use an array to have multiple menuOptions
*/
typedef struct {
	/** Set to SELECTED if this should be the selected menu item. Set all others to UNSELECTED */
	enum {UNSELECTED,SELECTED} isSelected;
	/** The text to display for this menu item */
	char* name;
	/** Allows for subMenu structures */ 
	struct menuOption* subMenu;	
} menuOption;

/**
* Displays the menu on the OLED. Direct write to OLED-controller
@param [in] menu The array of menuOption items.
@param [in] length The length of the array
@param [in] changed Indicator if the menu has been changed since the last call. Don't draw if it has
*/
void menu_display(menuOption menu[], int length, int* changed);

/**
* Write the menu to SRAM instead. This allows for double buffering the OLED.
@param [in] menu The array of menuOption items.
@param [in] length The length of the array
*/
void menu_display_RAMV2(menuOption menu[], int length);

#endif /* MENU_H_ */