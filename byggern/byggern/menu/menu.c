
#include "menu.h"
#include "../drivers/oled.h"
#include "../drivers/multifunction.h"

void menu_display(menuOption menu[]){
	oled_clear_display();
	oled_goto_position(7, 10);
	oled_print(sizeof(menu)+16);
	oled_home();
	
	for(int i = 0; i < sizeof(menu); i++){
		if(menu[i].isSelected == SELECTED){
			oled_printf(">"); 
			oled_printf(menu[i].name);
			oled_printf("\n");
		}else{
			oled_printf(" ");
			oled_printf(menu[i].name);
			oled_printf("\n");
		}
	}
}

