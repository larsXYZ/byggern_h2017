#include "menu_object.h"
#include "oled_driver.h"
#include "ADC_driver.h"

//Constants, mostly positions
#define HEADLINE_START_COLUMN 10
#define HEADLINE_START_PAGE 0
#define HEADLINE_SIZE 2
#define OPTION_START_COLUMN 20
#define OPTION_START_PAGE 2
#define OPTION_SIZE 1
#define CURSOR_TYPE "->\0"
#define CURSOR_START_COLUMN 10

//Previous input, hinders several inputs in short timespan
struct input prev_input;

void opt_constr(struct option *o, char *name, void (*a)())
{
	//Sets initial name
	o->name = name;
	
	//Sets initial next pointer
	o->next = NULL;

	//Sets action
	o->action = a;
	
	//Sets submenu
	o->submenu = NULL;
}

void menu_constr(struct menu *m, char *name)
{
	//Resets headline
	m->headline = name;

	//Sets root node as NULL
	m->root_option = NULL;
	
	//Sets cursor position at start
	m->cursor_pos = 0;
	
	//Sets current menu pointer
	m->curr_menu = m;
	
	//Sets parent menu
	m->parent_menu = NULL;
	
}

void menu_print(struct menu *m)
{
	//Print headline
	oled_go_to(HEADLINE_START_COLUMN, HEADLINE_START_PAGE);
	oled_cstring_write(m->curr_menu->headline,HEADLINE_SIZE);
	
	//Print option-nodes
	struct option *opt_element = m->curr_menu->root_option;
	uint8_t col_number = 0;
	
	while (opt_element != NULL)
	{
		oled_go_to(OPTION_START_COLUMN, OPTION_START_PAGE+col_number);
		oled_cstring_write(opt_element->name,OPTION_SIZE);
		opt_element = opt_element->next;
		col_number += 1;
	}
	//Print Cursor
	oled_go_to(CURSOR_START_COLUMN,OPTION_START_PAGE+m->curr_menu->cursor_pos);
	oled_cstring_write(CURSOR_TYPE,OPTION_SIZE);
	
}

void menu_control(struct menu *m)
{
	//We are working on what the current menu pointer is pointing at
	struct menu *current_menu = m->curr_menu;
	struct option *opt_element = m->curr_menu->root_option;
	
	//Checks if something changed on screen (then we must refresh)
	int change = 0;
	
	//Moves cursor
	if (joystick_up()) //UP
	{
		if (current_menu->cursor_pos > 0) current_menu->cursor_pos -= 1;
		change += 1;
	}
	else if (joystick_down()) //DOWN
	{
		if (current_menu->cursor_pos < menu_count_options(current_menu)-1)current_menu->cursor_pos += 1;
		change += 1;
	}
	
	if (joystick_right()) //ENTERS SUBMENU
	{
		//Finds target option
		struct option *target = current_menu->root_option;
		
		if (target != NULL)
		{
			for (int i = 0; i < current_menu->cursor_pos; i++) target = target->next;

			//Executes action
			if (target->action != NULL) target->action();
		
			//Enters submenu
			if (target->submenu != NULL) current_menu = target->submenu;
			change += 1;
		}
		
	}
	
	if (joystick_left()) //Leaves submenu
	{
		//Enters submenu
		if (current_menu->parent_menu != NULL ) current_menu = current_menu->parent_menu;
		change += 1;
	}
	
	//Updates curr_menu pointer in main menu
	m->curr_menu = current_menu;
	
	//Current input is now previous input
	prev_input = CURRENT_INPUT;
	
	if (change)
	{
		oled_clear_SRAM();
		menu_print(m->curr_menu);
	}
	
	
	
}

int menu_count_options(struct menu *m)
{
	int number = 0;
	struct option *curr_option = m->root_option;
	
	//Iterates through linked list
	while (curr_option != NULL)
	{
		number += 1;
		curr_option = curr_option->next;
	}
	
	return number;
}

