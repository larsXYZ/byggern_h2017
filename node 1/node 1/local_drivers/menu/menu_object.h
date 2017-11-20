#ifndef MENU_OBJECT
#define MENU_OBJECT

//The option object
struct option
{

	//Name
	char *name;

	//Action
	void (*action)();

	//Next
	struct option *next;

	//Parent menu
	struct menu *parent;
	
	//Submenu
	struct menu *submenu;
	
};

//The menu object 
struct menu
{
	//Headline
	char *headline;

	//Root option
	struct option *root_option;
	
	//Cursor pos
	int cursor_pos;
	
	//Submenu
	struct menu *curr_menu;
	
	//Parent menu
	struct menu * parent_menu;

};

//Option-functions
void opt_constr(struct option *o, char *name, void (*a)());

//Menu-functions
void menu_constr(struct menu *m, char *name); //Constructs initial menu object
void menu_print(struct menu *m); //Prints menu with all options
void menu_control(struct menu *m); //Controls menu with input
int menu_count_options(struct menu *m); //Returns number of options

#endif