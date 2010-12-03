#include "pivotseditor.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>

pv_editor::pv_editor(void)
{
	start.bound.Set(10, 10);
	start.start.Set(0,0);
	start.end.Set(10,10);
	start.cursor.x = start.start.x;
	start.cursor.y = start.start.y;
	start.pivot_num = 0;
	start.line_num = 0;
	cmd_his_num = 0;
	cmd_his_cur = 0;
	mode = EDIT_MODE;
	
	workspace = start;
}



void pv_editor::normal_keys(unsigned char key, int xx, int yy)
{
	switch(mode)
	{
		case EDIT_MODE:
			switch(key)
				{
					case ':':
						mode = COMMAND_MODE;
						break;
					
				}

			break;
		case GAME_MODE:
				break;
		case COMMAND_MODE:
			command_key(key);
			break;
		default:
				break;
	}	
}

void pv_editor::special_keys(int key, int xx, int yy)
{
	switch(mode)
	{
		case EDIT_MODE:
			break;
		case GAME_MODE:
			break;
		case COMMAND_MODE:
			command_specialkeys(key);
			break;
		default:
				break;
	}	
}