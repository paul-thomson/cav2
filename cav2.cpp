#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>
#define GLUT_KEY_ESCAPE 27
#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

#include "Vector.h"
#include "Matrix.h"
#include "Volume.h"

#define WIDTH 256
#define HEIGHT 256

static Volume* head = NULL;

void Update() {
	glutPostRedisplay();
}

Vector3 whiteTransfer(double val) {
	return Vector3(val, val, val);
}

Vector3 redToWhite(double val) {
	if (val < 0.4) {
		return Vector3(1, 0, 0);
	} else {
		return Vector3(1, 1, 1);
	}
}

void maximumIntensityProjection() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(int y = 0; y < head->GetHeight(); y++)
		for(int z = 0; z < head->GetDepth(); z++) {
			double maxValue = 0;
			for (int x = 0; x < head->GetWidth(); x++) {

				unsigned char val = head->Get(x, y, z);

				double normalisedVal = val/255.0;
				if (normalisedVal > maxValue) {
					maxValue = normalisedVal;
				}
			}

			Vector3 color = whiteTransfer(maxValue);
			glColor4f(color.r(), color.g(), color.b(), 1);

			glVertex3f(y, z, 0);
		}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void averageValue() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(int y = 0; y < head->GetHeight(); y++)
		for(int z = 0; z < head->GetDepth(); z++) {
			double totalValue = 0;
			for (int x = 0; x < head->GetWidth(); x++) {

				unsigned char val = head->Get(x, y, z);

				double normalisedVal = val/255.0;
				totalValue += normalisedVal;
			}

			Vector3 color = whiteTransfer(totalValue/(double)head->GetWidth());
			glColor4f(color.r(), color.g(), color.b(), 1);

			glVertex3f(y, z, 0);
		}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void Draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(int y = 0; y < head->GetHeight(); y++)
		for(int z = 0; z < head->GetDepth(); z++) {
			for (int x = head->GetWidth(); x > 0 ; x--) {

				unsigned char val = head->Get(x, y, z);

				double normalisedVal = val/255.0;
				Vector3 color = redToWhite(normalisedVal);
				glColor4f(color.r(), color.g(), color.b(), normalisedVal);

				glVertex3f(y, z, 0);
			}


		}
	glEnd();

	glFlush();
	glutSwapBuffers();
}


void KeyEvent(unsigned char key, int x, int y) {

	switch(key) {
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	}

}

int main(int argc, char **argv) {

	head = new Volume("head");


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("cav2");

	glClearColor(0.5, 0.5, 0.5, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -512, 512);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutKeyboardFunc(KeyEvent);
	glutDisplayFunc(Draw);
	glutIdleFunc(Update);

	glutMainLoop();

	delete head;
};
