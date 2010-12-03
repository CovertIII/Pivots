#include "pivotseditor.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>

void pv_editor::command_key(unsigned char key) 
{
	int i;
	if(key==10 || key==13)
	{
		mode = EDIT_MODE;
		command_buf[cbuf_num] = NULL;
		
		int i;
		for(i = cmd_his_num; i>0; i--)
		{
			strcpy(command_his[i], command_his[i-1]);
		}
		
		if(cmd_his_num<500)
			{cmd_his_num++;}
		
		
		sort_commands(); 
		
		cbuf_num=0;
		command_buf[cbuf_num] = NULL;
	}
	else if (key == 127) 
	{
		cbuf_num--;
	}
	else if (cbuf_num < 97) 
	{
		command_buf[cbuf_num]=key;
		cbuf_num++;
	}

}

void pv_editor::command_specialkeys(int key)
{
	switch(key) {
		case GLUT_KEY_LEFT : 
			
			break;
		case GLUT_KEY_RIGHT : 
			
			break;
		case GLUT_KEY_UP:
			cmd_his_cur++;
			strcpy(command_buf, command_his[cmd_his_cur-1]);
			break;
		case GLUT_KEY_DOWN : 
			cmd_his_cur--;
			strcpy(command_buf, command_his[cmd_his_cur-1]);
			break;
	}
}

void pv_editor::sort_commands(void)
{
	char command[3];
//	sscanf(command_buf, "%s", &command);
	
	if(!strncmp(command_buf, "q", 1))
		{exit(0);}
	else if(!strncmp(command_buf, "m", 1))
		{strcpy(message_buf, command_buf+2);}
	else if(!strncmp(command_buf, "add pivot", 9))
	{
		int i = workspace.pivot_num;
		sscanf(command_buf + 9, "%d %d %d", &workspace.pivot[i].x, &workspace.pivot[i].y, &workspace.pivot[i].dir);
		workspace.pivot_num++;
		//will add a function that will error check.
	}
} 

