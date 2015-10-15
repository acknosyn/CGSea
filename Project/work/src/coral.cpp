//---------------------------------------------------------------------------
//
// Jun Burden 2015
// Based on COMP308 Assignment 2 skeleton 2015
// by Taehyun Rhee, Joshua Scott, Ben Allen
//
//----------------------------------------------------------------------------

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


Coral::Coral(float x, float y, float z, float length, float radius, int size, int style) {
	m_translation.x = x;
	m_translation.y = y;
	m_translation.z = z;
	if(style==1) {
		createBranch1(length, radius, 0, 0, size);
	}else if(style==2) {
		createBranch2(length, radius, 0, 0, size);
	}
}

int Coral::createBranch1(float length, float radius, float yAngle, float xAngle, int size) {
	branch b = branch();
	b.yRot = yAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0)) - 2.0;
	b.xRot = xAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0)) - 2.0;
	b.length = length*(0.6+(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.4))));
	b.radius = radius;
	if(size > 1) {
		for(int i=0;i<g_numChildren; ++i) {
			int c = createBranch1(length*0.8, radius*0.6, i*360.0/g_numChildren, g_angle, size-1);
			b.children.push_back(c);
		}
		int c = createBranch1(length*0.7, radius*0.6, 0, 90, size-1);
		b.children.push_back(c);
	}
	m_branches.push_back(b);
	return m_branches.size()-1;
}

int Coral::createBranch2(float length, float radius, float yAngle, float xAngle, int size) {
	branch b = branch();
	b.yRot = yAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0)) - 2.0;
	b.xRot = xAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0)) - 2.0;
	b.length = length*(0.6+(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.4))));
	b.radius = radius;
	if(size > 1) {
		for(int i=0;i<g_numChildren; ++i) {
			float yAng = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40.0))-20.0;
			float xDiff = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40.0))-20.0;
			int c = createBranch2(length*0.6, radius*0.5, yAng, i*150.0/g_numChildren+50.0+xDiff, size-1);
			b.children.push_back(c);
		}
	}
	m_branches.push_back(b);
	return m_branches.size()-1;
}

void Coral::changeColour(float r, float g, float b) {
	m_r = r;
	m_g = g;
	m_b = b;
}

void Coral::renderCoral() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//Create a new GLUquadric object; to allow you to draw cylinders
	GLUquadric *quad = gluNewQuadric();
	if (quad == 0) {
		cerr << "Not enough memory to allocate space to draw" << endl;
		exit(EXIT_FAILURE);
	}
	
	// Pick colour
	glShadeModel(GL_SMOOTH);
	glColor3f(m_r/255.0f,m_g/255.0f,m_b/255.0f);
	
	// Translate
	glTranslatef(m_translation.x, m_translation.y, m_translation.z);
	glRotatef(90,0,1,0); // rotate around y
	
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
		// Branch
		glPushMatrix();
			glRotatef(-90,1,0,0); // rotate around x
			// rotate axis
			GLfloat angleY = b->yRot;
			glRotatef(-angleY,0,1,0); // rotate around y
			GLfloat angleX = b->xRot;
			glRotatef(-angleX,1,0,0); // rotate around x
			
			
			// draw
			gluCylinder(q, b->radius/0.7, b->radius, b->length, g_slices, g_stacks); // draw branch
			
			// move coordinate system
			glRotatef(-90,0,1,0); // rotate to align axis
			glTranslatef(b->length,0,0); // translate the length of the branch
			
			// draw sphere
			glutSolidSphere(b->radius, g_slices, g_stacks);
			
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