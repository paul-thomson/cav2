#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


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
Vector3 startingPosition = Vector3(0, 0, 0);
Vector3 directionVector = Vector3(0, 0, 1);
Matrix_3x3 rotationMatrix = Matrix_3x3::Id();


void Update() {
	startingPosition = Matrix_4x4::Translation(Vector3(128, 128, 128)) * rotationMatrix * Matrix_4x4::Translation(Vector3(-128, -128, -128)) * Vector3(0, 0, 0);
	directionVector = rotationMatrix * Vector3(0, 0, 1);
	//cout << startingPosition.r()  << "," << startingPosition.g() << "," << startingPosition.b() << "\n";
	cout << directionVector.r()  << "," << directionVector.g() << "," << directionVector.b() << "\n";
	cout << "UPDATED\n";
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

Vector4 manualColoursAndOpacities(double val) {
	if (val < 0.3) {
		return Vector4(0, 0, 1, 0.1);
	} else if (val < 0.35) {
		return Vector4(0, 1, 0, 0.05);
	} else {
		return Vector4(1, 0, 0, 0.2);
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

void redWhiteDraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(int y = 0; y < head->GetHeight(); y++)
		for(int z = 0; z < head->GetDepth(); z++) {

			for (int x = head->GetWidth(); x > 0 ; x--) {

				unsigned char val = head->Get(x, y, z);

				double normalisedVal = val/255.0;
				if (normalisedVal > 0.2) {
					Vector3 color = redToWhite(normalisedVal);
					glColor4f(color.r(), color.g(), color.b(), normalisedVal);

					glVertex3f(y, z, 0);
				}
			}
		}
	glEnd();

	glFlush();
	glutSwapBuffers();

}

void manualDraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for (int x = 0; x < head->GetWidth() ; x++) {
		for(int y = 0; y < head->GetHeight(); y++) {
			//for(int z = 0; z < head->GetDepth(); z++) {
			for(int z = head->GetDepth(); z > 0; z--) {
				//for(int y = head->GetHeight(); y > 0; y--) {
				//for(int x = head->GetWidth(); x > 0; x--) {


				unsigned char val = head->Get(x, y, z);

				double normalisedVal = val/255.0;
				if (normalisedVal > 0.3) {
					Vector4 color = manualColoursAndOpacities(normalisedVal);
					glColor4f(color.r(), color.g(), color.b(), color.a());

					glVertex3f(x, y,0);
				}
			}
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

int withinBounds(int width, int height, int depth, Vector3 loc) {
	if (loc.r() > 0 && loc.g() > 0 && loc.b() > 0 && loc.r() < width && loc.g() < height && loc.b() < depth) {
		return 1;
	} else {
		return 0;
	}
}

void draw3D() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);

	for(int c = 0; c < WIDTH; c++) {
		for(int r = 0; r < HEIGHT; r++) {
			//
			Vector3 voxelLocation = rotationMatrix * Vector3(c, r, 0) + startingPosition + (directionVector * 100);
			for (int i = 0; i < 100; i++) {
				if (withinBounds(head->GetWidth(), head->GetHeight(), 256, voxelLocation) ) { //TODO 256 or depth?
					unsigned char val = head->Get((int)voxelLocation.r(), (int)voxelLocation.g(), (int)(voxelLocation.b()*100.0/256.0));

					double normalisedVal = val/255.0;
					if (normalisedVal > 0.3) {
						Vector4 color = manualColoursAndOpacities(normalisedVal);
						glColor4f(color.r(), color.g(), color.b(), color.a());

						glVertex3f(c, r,0);
					}
				}
				voxelLocation -= directionVector;
			}


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
	case 'x':
		rotationMatrix = rotationMatrix * Matrix_3x3::RotationX(0.52);
		Update();
		break;
	case 'y':
		rotationMatrix = rotationMatrix * Matrix_3x3::RotationY(0.52);
		Update();
		break;
	case 'z':
		rotationMatrix = rotationMatrix * Matrix_3x3::RotationZ(0.52);
		Update();
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
	glutDisplayFunc(draw3D);
	glutIdleFunc(Update);

	glutMainLoop();

	delete head;
};
