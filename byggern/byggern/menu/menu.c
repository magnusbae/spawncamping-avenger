
#include "menu.h"
#include "../drivers/oled.h"
#include "../drivers/multifunction.h"

void menu_display(menuOption menu[], int length, int* changed){
	if (*changed){
		oled_clear_display();
		*changed=0;
		
	}
	
	oled_home();
	for(int i = 0; i < length; i++){
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

