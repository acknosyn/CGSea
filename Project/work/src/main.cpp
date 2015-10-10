//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "terrain.hpp"
#include "school.hpp"

using namespace std;
using namespace comp308;

#define PI 3.14159265

// Global variables
// 
GLuint g_winWidth  = 640;
GLuint g_winHeight = 480;
GLuint g_mainWindow = 0;


// Terrain loader and drawer
//
Terrain *g_terrain = nullptr;


// Fishy stuff
//
// School of fish
School *g_school = nullptr;
bool play = true;


// toggle values
bool g_terrainActive = true;
bool g_fishActive = true;
bool g_causticsActive = true;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_lMouseDown = false;
vec2 g_lMousePos;
bool g_rMouseDown = false;
vec2 g_rMousePos;
float g_yWorldRotation = 0;
float g_xWorldRotation = 0;
float g_yRotation = 0;
float g_xRotation = 0;
float g_xPos = 0;
float g_yPos = 0;
float g_zPos = -20;


// Sets up where and what the light is
// Called once on start up
// 
void initLight() {
	float direction[]	  = {0.0f, 0.0f, 1.0f, 0.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);	
	
	
	glEnable(GL_LIGHT0);
}


// Sets up where the camera is in the scene
// Called every frame
// 
void setUpCamera() {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, float(g_winWidth) / float(g_winHeight), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Load camera transforms
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
	glTranslatef(g_xPos, g_yPos, g_zPos);
	glRotatef(g_xWorldRotation, 1, 0, 0);
	glRotatef(g_yWorldRotation, 0, 1, 0);
}


// Draw function
//
void draw() {

	// Set up camera every frame
	setUpCamera();

	// Black background
	glClearColor(0.04705f,0.25098f,0.34902f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	// Set the current material (for all objects) to red
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
	glColor3f(1.0f,0.0f,0.0f);


	// Render 
	if (g_terrainActive) g_terrain->renderTerrain();

	if (g_fishActive) {
		g_school->update(play);
	}

	if (g_causticsActive) // render caustics

	// Disable flags for cleanup (optional)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();

	// Queue the next frame to be drawn straight away
	glutPostRedisplay();
}


// Reshape function
// 
void reshape(int w, int h) {
    if (h == 0) h = 1;

	g_winWidth = w;
	g_winHeight = h;
    
    // Sets the openGL rendering window to match the window size
    glViewport(0, 0, g_winWidth, g_winHeight);  
}


//-------------------------------------------------------------
// [Assignment 2] :
// Modify the keyboardCallback function and additional files,
// to make your priman pose when the 'p' key is pressed.
//-------------------------------------------------------------

// Keyboard callback
// Called once per button state change
//
void keyboardCallback(unsigned char key, int x, int y) {
	 cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch(key){

		case 'w': //move forward
			g_xPos -= cos(g_xRotation*PI/180)*sin(g_yRotation*PI/180);
			g_yPos += sin(g_xRotation*PI/180);
			g_zPos += cos(g_xRotation*PI/180)*cos(g_yRotation*PI/180);
			break;

		case 'a': //move left
			g_xPos += cos(g_yRotation*PI/180);
			g_zPos += sin(g_yRotation*PI/180);
			break;

		case 's': //move backward
			g_xPos += cos(g_xRotation*PI/180)*sin(g_yRotation*PI/180);
			g_yPos -= sin(g_xRotation*PI/180);
			g_zPos -= cos(g_xRotation*PI/180)*cos(g_yRotation*PI/180);
			break;

		case 'd': //move right
			g_xPos -= cos(g_yRotation*PI/180);
			g_zPos -= sin(g_yRotation*PI/180);
			break;

		case 't': //toggle terrain
			g_terrainActive = !g_terrainActive;
			break;

		case 'f': //toggle fish
			g_fishActive = !g_fishActive;
			break;

		case 'c': //toggle caustics
			g_causticsActive = !g_causticsActive;
			break;

		case 'p': // play/pause fish sim
			play = !play;
			break;

		case 'o': // step 1 frame through fish sim
			g_school->step = true;
			break;
	}
}


// Special Keyboard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	 cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// Not needed for this assignment, but useful to have later on
}


// Mouse Button Callback function
// (x,y) :: (0,0) is top left and (g_winWidth, g_winHeight) is bottom right
// state :: 0 is down, 1 is up
// 
// Called once per button state change
// 
void mouseCallback(int button, int state, int x, int y) {
	cout << "Mouse Callback :: button=" << button << ", state=" << state << ", (" << x << "," << y << ")" << endl;
	switch(button){

		case 0: //left mouse button
			g_lMouseDown = (state==0);
			g_lMousePos = vec2(x, y);
			break;
		case 2: //left mouse button
			g_rMouseDown = (state==0);
			g_rMousePos = vec2(x, y);
			break;
		case 3: //scroll foward/up
			g_xPos -= cos(g_xRotation*PI/180)*sin(g_yRotation*PI/180);
			g_yPos += sin(g_xRotation*PI/180);
			g_zPos += cos(g_xRotation*PI/180)*cos(g_yRotation*PI/180);
			break;
		case 4: //scroll back/down
			g_xPos += cos(g_xRotation*PI/180)*sin(g_yRotation*PI/180);
			g_yPos -= sin(g_xRotation*PI/180);
			g_zPos -= cos(g_xRotation*PI/180)*cos(g_yRotation*PI/180);
			break;
	}
}


// Mouse Motion Callback function
// Called once per frame if the mouse has moved and
// at least one mouse button has an active state
// 
void mouseMotionCallback(int x, int y) {
	cout << "Mouse Motion Callback :: (" << x << "," << y << ")" << endl;
	if (g_lMouseDown) {
		vec2 dif = vec2(x,y) - g_lMousePos;
		g_lMousePos = vec2(x,y);
		g_yRotation += 0.3 * dif.x;
		g_xRotation += 0.3 * dif.y;
	}
	if (g_rMouseDown) {
		vec2 dif = vec2(x,y) - g_rMousePos;
		g_rMousePos = vec2(x,y);
		g_yWorldRotation += 0.3 * dif.x;
		g_xWorldRotation += 0.3 * dif.y;
	}
	if(g_xRotation < -90) {
		g_xRotation = -90;
	} else if(g_xRotation > 90) {
		g_xRotation = 90;
	}
}



//Main program
// 
int main(int argc, char **argv) {

	if(argc > 2){
		cout << "Too many arguments" << endl;
		exit(EXIT_FAILURE);
	}

	// Initialise GL, GLU and GLUT
	glutInit(&argc, argv);
	
	// Setting up the display
	// - RGB color model + Alpha Channel = GLUT_RGBA
	// - Double buffered = GLUT_DOUBLE
	// - Depth buffer = GLUT_DEPTH
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	// Initialise window size and create window
	glutInitWindowSize(g_winWidth, g_winHeight);
	g_mainWindow = glutCreateWindow("COMP308 CGSea");


	// Initilise GLEW
	// must be done after creating GL context (glutCreateWindow in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	// Create terrain
	if(argc == 2) {
		g_terrain = new Terrain(argv[1]);
	} else {
		g_terrain = new Terrain();
	}

	// Fishy stuff
	g_school = new School();

	// Register functions for callback
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);

	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);


	// Create a light on the camera
	initLight();


	// Loop required by OpenGL
	// This will not return until we tell OpenGL to finish
	glutMainLoop();

	// Don't forget to delete all pointers that we made
	delete g_terrain;
	delete g_school;
	return 0;
}