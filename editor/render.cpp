#include "pivotseditor.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>

#define SCALE 1.0f/15.0f

void square(int x, int y, float scale){
	glPushMatrix();
	glLineWidth(2.0);
	glTranslatef(x,y,0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0,0);
	glVertex2f(scale,0);
	glVertex2f(scale,scale);
	glVertex2f(0,scale);
  glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(scale,0);
	glVertex2f(scale,scale);
	glVertex2f(0,scale);
    glEnd();
	glPopMatrix();
}

void circle(float pos_x, float pos_y, float size) {
	glPushMatrix();
	glLineWidth(2.0);
	glTranslatef(pos_x,pos_y,0);
	float counter;
	glBegin(GL_LINE_LOOP);
	for (counter = 0; counter <= 2*3.14159; counter = counter + 3.14159/8) {
		glVertex3f ((size)*cos(counter), (size)*sin(counter), 0.0);
	}
  glEnd();
	glBegin(GL_POLYGON);
	for (counter = 0; counter <= 2*3.14159; counter = counter + 3.14159/8) {
		glVertex3f ((size)*cos(counter), (size)*sin(counter), 0.0);
	}
  glEnd();
	glPopMatrix();
}

void renderBitmapString(float x, float y, void *font, char *string) 
{  
	glPushMatrix();
	glLoadIdentity();
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
	glPopMatrix();
}

void pv_editor::render(int now)
{
	int i,k;
	char buf[255];
	int w_width = glutGet(GLUT_WINDOW_WIDTH);
	int w_height = glutGet(GLUT_WINDOW_HEIGHT);
	v2i boundary = workspace.bound.Get();
	float margin = SCALE*w_width;
	float cell = (1.0f-2.0f*SCALE)*w_width / ((float) boundary.x);
	float p_width = (1.0f-2.0f*SCALE)*w_width;
	float p_height = ((float)boundary.y)/((float) boundary.x) * p_width;
	
	//-----This is the stuff involved with drawing the screen----//	
	glClearColor (0.8, 0.9,0.9,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Displays Boundaries and a grid
	glPushMatrix();
	glTranslatef(margin, margin, 0);
	glColor3f (0.6, 0.7,0.7);
	for(i=0; i <= boundary.x; i++)
	{
		glBegin(GL_LINES);
		glVertex2f (i*cell,0);
		glVertex2f (i*cell,p_height);
	  glEnd();
	}
	for(i=0; i <= boundary.y; i++)
	{
		glBegin(GL_LINES);
		glVertex2f (0,i*cell);
		glVertex2f (p_width,i*cell);
	  glEnd();
	}
	
	//Displays Start 
	glColor3f (0.4, 1.0 ,0.5);
	float scale = cell*0.3;
	v2i start = workspace.start.Get();
	square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
	
	
	//Displays End
	glColor3f (1.0, 0.4 ,0.5);
	v2i end = workspace.end.Get();
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	
	//Displays a the lines
	glColor3f (0.4, 0.4 ,0.4);
	scale = cell*0.2;
	for(i=0; i<workspace.line_num; i++)
	{
		start = workspace.line[i].pt1;
		end = workspace.line[i].pt2;
		square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
		square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex2f (start.x*cell,start.y*cell);
		glVertex2f (end.x*cell,end.y*cell);
	  glEnd();
	}
	
	
	
	//Displays a the pivots
	for(i=0; i<workspace.pivot_num; i++)
	{
		start = workspace.pivot[i];
		glColor3f (0.2, 0.2 ,0.2);
		circle(start.x*cell, start.y*cell, cell*0.5*0.3);
		//Displays a the direction of the pivot 0 for horizontal and 1 for vertical
		glLineWidth(2.0);
		glColor3f (1,1,1);
		if(workspace.pivot[i].dir)
		{
			glBegin(GL_LINES);
			glVertex2f (start.x*cell, start.y*cell + cell*0.5*0.3);
			glVertex2f (start.x*cell, start.y*cell - cell*0.5*0.3);
		  glEnd();
		}
		else
		{
			glBegin(GL_LINES);
			glVertex2f (start.x*cell + cell*0.5*0.3, start.y*cell);
			glVertex2f (start.x*cell - cell*0.5*0.3, start.y*cell);
		  glEnd();
		}	
	}
	
	//Displays the cursor, checks to see if the cursor has a pivot, if it does its color is yellow, if not, blue.
	if(workspace.cursor.grasp)
		{glColor3f (0.8, 0.7, 0);}
	else
		{glColor3f (0.3, 0.5,0.7);}
	scale = cell*0.22;
	end = workspace.cursor;
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	glLineWidth(1.0);


	if (now%1000 < 500) {
		command_buf[cbuf_num] = 32;
	}
	else {
		command_buf[cbuf_num] = 95;
	}
	
	if(mode == COMMAND_MODE)
	{
		glColor3f (0, 0 ,0);
		command_buf[cbuf_num+1]=0;
		glPushMatrix();
		glLoadIdentity();
		renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT)-35, GLUT_BITMAP_HELVETICA_12, command_buf);
		glPopMatrix();
	}
	

	glColor3f (0, 0 ,0);
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT)-20, GLUT_BITMAP_HELVETICA_12, message_buf);
	glPopMatrix();

	glPopMatrix();
	
	glutSwapBuffers();
	glFlush();
}