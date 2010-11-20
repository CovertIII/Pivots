#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include "pivots_game.h"

#define SCALE 1.0f/15.0f  //Size in pixels of the snake body squares
#define sc_width 600
#define sc_height 600
#define TIMERMSECS 100


int lastFrameTime = 0;
float timecheck = 0;

pivots_game pivots;


void square(int x, int y, float scale){
	glPushMatrix();
	glTranslatef(x,y,0);
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
	glTranslatef(pos_x,pos_y,0);
	float counter;
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

void init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}
void calc(int value)
{
	glutTimerFunc(TIMERMSECS, calc, 0);
	glutPostRedisplay();
}

void render(void)
{
	int i,k;
	int w_width = glutGet(GLUT_WINDOW_WIDTH);
	int w_height = glutGet(GLUT_WINDOW_HEIGHT);
	v2i boundary = pivots.get_bound();
	float margin = SCALE*w_width;
	float cell = (1.0f-2.0f*SCALE)*w_width / ((float) boundary.x);
	float p_width = (1.0f-2.0f*SCALE)*w_width;
	float p_height = ((float)boundary.y)/((float) boundary.x) * p_width;
	
	//-----This is the stuff involved with drawing the screen----//	
	glClearColor (0.8, 0.9,0.9,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Displays Boundaries
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
	for(i=0; i <= boundary.x; i++)
	{
		glBegin(GL_LINES);
		glVertex2f (0,i*cell);
		glVertex2f (p_width,i*cell);
	  glEnd();
	}
	
	glColor3f (0.4, 1.0 ,0.5);
	float scale = cell*0.3;
	v2i start = pivots.pos_start();
	square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
	
	glColor3f (1.0, 0.4 ,0.5);
	v2i end = pivots.pos_end();
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	
	
	glLineWidth(3.0);
	glColor3f (0.4, 0.4 ,0.4);
	scale = cell*0.2;
	for(i=0; i<pivots.num_line(); i++)
	{
		start = pivots.pos_line_pt1(i);
		end = pivots.pos_line_pt2(i);
		square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
		square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
		glBegin(GL_LINES);
		glVertex2f (start.x*cell,start.y*cell);
		glVertex2f (end.x*cell,end.y*cell);
	  glEnd();
	}
	
	
	
	for(i=0; i<pivots.num_pivot(); i++)
	{
		start = pivots.pos_pivot(i);
		glColor3f (0.2, 0.2 ,0.2);
		circle(start.x*cell, start.y*cell, cell*0.5*0.3);
		glLineWidth(2.0);
		glColor3f (1,1,1);
		if(pivots.dir_pivot(i))
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
	
	if(pivots.grasp_cursor())
		{glColor3f (0.8, 0.7, 0);}
	else
		{glColor3f (0.3, 0.5,0.7);}
	scale = cell*0.22;
	end = pivots.pos_cursor();
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	glLineWidth(1.0);
	glPopMatrix();
	glutSwapBuffers();
	
	glFlush();
}


void display(void) 
{
	render();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch(key) {
		case 27:
			exit(0);
			break;
		case 'D':
		case 'd':
			pivots.request_pick_drop_pivot();
			break;
		case 'A':
		case 'a':
			pivots.request_pivot(1);
			break;
		case 'S':
		case 's':
    	pivots.request_pivot(-1);
			break;	
		default:
			break;
	}
}


void pressKey(int key, int xx, int yy) {
	switch(key) {
		case GLUT_KEY_LEFT : 
			pivots.request_move_cursor(-1,0);
			break;
		case GLUT_KEY_RIGHT : 
			pivots.request_move_cursor(1,0);
			break;
		case GLUT_KEY_UP : 
			pivots.request_move_cursor(0,1);
			break;
		case GLUT_KEY_DOWN : 
			pivots.request_move_cursor(0,-1);
			break;
	}
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
	pivots.load_file(argv[1]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(sc_width, sc_height);
  glutCreateWindow("Pivots");
	
	init();
	
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutDisplayFunc(display);
  glutReshapeFunc(reshape);
	glutPostRedisplay();
	
	glutTimerFunc(TIMERMSECS, calc, 0);
	
  glutMainLoop();
  return EXIT_SUCCESS;
}