//---------------------------------------------------------------------------
//
// Jun Burden 2015
// Based on COMP308 Assignment 2 skeleton 2015
// by Taehyun Rhee, Joshua Scott, Ben Allen
//
//----------------------------------------------------------------------------

#include <unistd.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "comp308.hpp"
#include "coral.hpp"

using namespace std;
using namespace comp308;

// Global variables
GLint g_slices = 10;
GLint g_stacks = 10;

int g_numChildren = 3;
float g_angle = 45.0;


Coral::Coral(float x, float y, float z, float length, int size) {
	createBranch(x, y, z, length, 0, 0, size);
	cout << m_branches.size() << endl;
}

int Coral::createBranch(float x, float y, float z, float length, float yAngle, float xAngle, int size) {
	branch b = branch();
	b.translation.x = x;
	b.translation.y = y;
	b.translation.z = z;
	b.yRot = yAngle;
	b.xRot = xAngle;
	b.length = length;
	if(size > 1) {
		for(int i=0;i<g_numChildren; ++i) {
			int c = createBranch(0, 0, 0, length*0.6, i*360.0/g_numChildren, g_angle, size-1);
			b.children.push_back(c);
		}
	}
	m_branches.push_back(b);
	return m_branches.size()-1;
}
// [Assignment2] you may need to revise this function
void Coral::renderCoral() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//Create a new GLUquadric object; to allow you to draw cylinders
	GLUquadric *quad = gluNewQuadric();
	if (quad == 0) {
		cerr << "Not enough memory to allocate space to draw" << endl;
		exit(EXIT_FAILURE);
	}

	//Actually draw the coral
	renderBranch(&m_branches[m_branches.size()-1], quad);

	// Clean up
	gluDeleteQuadric(quad);
	glPopMatrix();
}

void Coral::renderBranch(branch *b, GLUquadric* q) {
	// YOUR CODE GOES HERE
	// ...
	glPushMatrix();
		// Translate
		glTranslatef(b->translation.x, b->translation.y, b->translation.z);
		// Branch
		glPushMatrix();
			glRotatef(-90,1,0,0); // rotate around x
			// rotate axis
			GLfloat angleY = b->yRot;
			glRotatef(-angleY,0,1,0); // rotate around y
			GLfloat angleX = b->xRot;
			glRotatef(-angleX,1,0,0); // rotate around x
			
			
			// draw
			glColor3f(1.0,0.0,0.0);
			gluCylinder(q, b->length/4, b->length/4, b->length, g_slices, g_stacks); // draw branch
			
			// move coordinate system
			glRotatef(-90,0,1,0); // rotate to align axis
			glTranslatef(b->length,0,0); // translate the length of the branch
			
			// draw sphere
			glutSolidSphere(b->length/4, g_slices, g_stacks);
			
			// undo rotation
			//glRotatef(90+angleY,0,1,0); // rotate to align axis
			//glRotatef(angleX,1,0,0); // rotate around x
			glRotatef(-90,0,1,0); // rotate around z
			
			unsigned int i;
			for(i = 0; i < b->children.size(); i++){
			  renderBranch(&(m_branches[b->children[i]]), q);
			}
		glPopMatrix();
	glPopMatrix();
}