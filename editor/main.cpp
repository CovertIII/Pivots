#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include "pivotseditor.h"

#define sc_width 600
#define sc_height 600
#define TIMERMSECS 100


int lastFrameTime = 0;

pv_editor editor;
v2i mouse;

void init(int argc, char **argv)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}
void calc(int value)
{
	glutTimerFunc(TIMERMSECS, calc, 0);
	glutPostRedisplay();
}


void display(void) 
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	
	editor.render(now);
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	editor.normal_keys(key, xx, yy);
}


void pressKey(int key, int xx, int yy) {
	//editor.special_keys(key, xx, yy);	
}


void mouseButton(int bbutton, int sstate, int xx, int yy){
	yy=glutGet(GLUT_WINDOW_HEIGHT)-yy;	
	mouse.x = xx;
	mouse.y = yy;
	//editor.mouse_button(bbutton, sstate, xx, yy);	
}

void mouseMotion(int xx, int yy) {
	yy=glutGet(GLUT_WINDOW_HEIGHT)-yy;	
	mouse.x = xx;
	mouse.y = yy;
	//editor.mouse_motion(xx, yy);
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
