#include <stdlib.h> #include <stdio.h> #include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include "pivots_editor.h"

#define SCALE 1.0f/15.0f  //Size in pixels of the snake body squares
#define sc_width 600
#define sc_height 600
#define TIMERMSECS 100


char level[255];
int lvl = 1;
int lastFrameTime = 0;
float timecheck = 0;

pivots_game pivots;


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

void init(int argc, char **argv)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	if (argc > 1)
	{
		sprintf(level, "./levels/%s", argv[1]);
		pivots.load_file(level);
	}
	else
	{
		sprintf(level, "./levels/lvl%d.piv", lvl);
		pivots.load_file(level);
	}
}
void calc(int value)
{
	glutTimerFunc(TIMERMSECS, calc, 0);
	glutPostRedisplay();
}

void render(void)
{
	int i,k;
	char buf[255];
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
	v2i start = pivots.pos_start();
	square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
	
	
	//Displays End
	glColor3f (1.0, 0.4 ,0.5);
	v2i end = pivots.pos_end();
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	
	//Displays a the lines
	glColor3f (0.4, 0.4 ,0.4);
	scale = cell*0.2;
	for(i=0; i<pivots.num_line(); i++)
	{
		start = pivots.pos_line_pt1(i);
		end = pivots.pos_line_pt2(i);
		square(start.x*cell - scale*0.5, start.y*cell - scale*0.5, scale);
		square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex2f (start.x*cell,start.y*cell);
		glVertex2f (end.x*cell,end.y*cell);
	  glEnd();
	}
	
	
	
	//Displays a the pivots
	for(i=0; i<pivots.num_pivot(); i++)
	{
		start = pivots.pos_pivot(i);
		glColor3f (0.2, 0.2 ,0.2);
		circle(start.x*cell, start.y*cell, cell*0.5*0.3);
		//Displays a the direction of the pivot 0 for horizontal and 1 for vertical
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
	
	//Displays the cursor, checks to see if the cursor has a pivot, if it does its color is yellow, if not, blue.
	if(pivots.grasp_cursor())
		{glColor3f (0.8, 0.7, 0);}
	else
		{glColor3f (0.3, 0.5,0.7);}
	scale = cell*0.22;
	end = pivots.pos_cursor();
	square(end.x*cell - scale*0.5, end.y*cell - scale*0.5, scale);
	glLineWidth(1.0);
	
	//Tells you if you've won and to advance to then next level
	if(pivots.declare_win())
	{
		glColor3f (0, 0 ,0);
		sprintf(buf, "You Win! Press `L' to advance to level %d.", lvl+1);
		glPushMatrix();
		glLoadIdentity();
		renderBitmapString(90, glutGet(GLUT_WINDOW_HEIGHT)/2, GLUT_BITMAP_HELVETICA_18, buf);
		glPopMatrix();
	}
	
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
		case 'R':
		case 'r':
			pivots.load_file(level);
			break;
		case 'L':
		case 'l':
			if(pivots.declare_win())
			{
				lvl++;
				sprintf(level, "levels/lvl%d.piv", lvl);
				pivots.load_file(level);
			}
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


void mouseButton(int bbutton, int sstate, int xx, int yy){
	
	int i,j,h;
	if(sstate == 1 && create == TRUE){
		floater[particle_num].p.x = mouse.x;
		floater[particle_num].p.y = mouse.y;
		floater[particle_num].v.x = 0;
		floater[particle_num].v.y = 0;
		floater[particle_num].f.x = 0;
		floater[particle_num].f.y = 0;
		floater[particle_num].m = 1;
		
		for(i=0; i<particle_num; i++){
			connect[particle_num][i] = 0;
			connect[i][particle_num] = 0;
		}
		
		particle_num++;
		
	}
	if(sstate == 1 && spring == TRUE){
		
		if(pp1 < 0){
			for(i=0; i<particle_num; i++){
				if(v2Len(v2Sub(mouse, floater[i].p))<crad+20){
					p1 = floater[i].p;
					pp1 = i;
					i = particle_num;
				}
			}
		}
		
		if(pp2 < 0 && pp1 >= 0){
			//attach the particles
			for(i=0; i<particle_num; i++){
				if(v2Len(v2Sub(mouse, floater[i].p))<crad+20 && pp1 != i){
					p2 = floater[i].p;
					pp2 = i;
					
					if(connect[pp1][pp2] && connect[pp2][pp1]){
						connect[pp1][pp2]=0;
						connect[pp2][pp1]=0;
					}
					else{
						connect[pp1][pp2]=1;
						connect[pp2][pp1]=1;
					}
					
					i = particle_num;
					p1.x = -10;
					p1.y = -10;
					p2.x = -10;
					p2.y = -10;
					pp1 = -10;
					pp2 = -10;
				}
				
			}
		}
		
	}
	if(sstate == 1 && nail == TRUE){
		int space = TRUE;
		for(i=0; i<particle_num; i++){
			if(v2Len(v2Sub(mouse, floater[i].p))<crad+20){
				space = FALSE;
				if(floater[i].m< 1000000000){
					floater[i].m=1000000000;
					floater[i].v.x=0;
					floater[i].v.y=0;
				}
				else{floater[i].m=1;}	
				i = particle_num;
			}
		}
		if(space){
			floater[particle_num].p.x = mouse.x;
			floater[particle_num].p.y = mouse.y;
			floater[particle_num].v.x = 0;
			floater[particle_num].v.y = 0;
			floater[particle_num].f.x = 0;
			floater[particle_num].f.y = 0;
			floater[particle_num].m = 1000000000;
			
			for(i=0; i<particle_num; i++){
				connect[particle_num][i] = 0;
				connect[i][particle_num] = 0;
			}
			
			particle_num++;
		}
		
	}
	
	if((sstate == GLUT_DOWN) && (destroy)){
		for(i=0; i<particle_num; i++){
			if(v2Len(v2Sub(mouse, floater[i].p))<crad+20){
				for(j=i;j<particle_num;j++){
					floater[j]=floater[j+1];
				}
				for(j=i;j<particle_num;j++){
					for(h=0; h<particle_num; h++){
						connect[j][h]=connect[j+1][h];
					}
				}
				for(j=i;j<particle_num;j++){
					for(h=0; h<particle_num; h++){
						connect[h][j]=connect[h][j+1];
					}
				}
				i = particle_num;
				particle_num--;
			}
		}
		
	}
	
	if((sstate == GLUT_DOWN) && (nail+spring+create+destroy == FALSE)){
		int i;
		for(i=0; i<particle_num; i++){
			if(v2Len(v2Sub(mouse, floater[i].p))<crad+20){
				button = i;
				i = particle_num;
			}
		}
		
	}
	else{button = -1;}
	
	yy=glutGet(GLUT_WINDOW_HEIGHT)-yy;	
	mouse.x = xx;
	mouse.y = yy;
	
}

void mouseMotion(int xx, int yy) {
	yy=glutGet(GLUT_WINDOW_HEIGHT)-yy;	
	mouse.x = xx;
	mouse.y = yy;
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(sc_width, sc_height);
	glutCreateWindow("Pivots");
		
	init(argc, argv);
	
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPostRedisplay();
	
	glutTimerFunc(TIMERMSECS, calc, 0);
	
	glutMainLoop();
	return EXIT_SUCCESS;
}
