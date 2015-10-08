//---------------------------------------------------------------------------
//
// Code from Victoria University COMP308 2015 Assignment 1
// Created by Taehyun Rhee, Joshua Scott, Ben Allen
// Modified by Jun Burden
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "geometry.hpp"

using namespace std;
using namespace comp308;


Geometry::Geometry(string filename) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
	}
}

Geometry::Geometry(vector<vec3> points, vector<triangle> triangles) {
	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	m_points = points;
	m_triangles = triangles;
	cout << "points size: " << m_points.size() << endl;
	cout << "triangles size: " << m_triangles.size() << endl;
	createNormals();
	cout << "normals size: " << m_normals.size() << endl;
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
	}
}

void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));


	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					objLine >> v.p;		// Scan in position index
					if(m_normals.size()>1) {
						objLine.ignore(1);	// Ignore the '/' character
						if(m_uvs.size()>1) {
							objLine >> v.t;		// Scan in uv (texture coord) index
						}
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index
					}
					verts.push_back(v);
				}

				// IFF we have 3 verticies, construct a triangle
				if(verts.size() == 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}
	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}

void Geometry::createNormals() {
	std::vector<comp308::vec3> face_normals;
	//     create array of int lists with size points.size,
	//     list of the index number of triangles which relate to a point
	std::vector<std::vector<int>> triangles_which_intersect_point;
	unsigned int i;
	unsigned int j;
	for(i=0;i<m_points.size();++i){
	  std::vector<int> a;
	  triangles_which_intersect_point.push_back(a);
	  m_normals.push_back(comp308::vec3(0));
	}
	
	for(i=0;i<m_triangles.size();++i) {
	// create the face normals for each tri using cross product
	  comp308::vec3 vector1 = m_points[m_triangles[i].v[1].p] - m_points[m_triangles[i].v[0].p];
	  comp308::vec3 vector2 = m_points[m_triangles[i].v[2].p] - m_points[m_triangles[i].v[1].p];
	  face_normals.push_back(normalize(cross(vector1, vector2)));
	  
	// for each vertex in triangle, add triangle index to list under array of points
	  triangles_which_intersect_point[m_triangles[i].v[0].p].push_back(i);
	  triangles_which_intersect_point[m_triangles[i].v[1].p].push_back(i);
	  triangles_which_intersect_point[m_triangles[i].v[2].p].push_back(i);
	//     now you have an array of lists, the index relates to the index of a point,
	//     and the list os of the indexes of the triangles which intersect this point
	}
	
	// Trying to do smooth shading not working
	// for each point calculate it's normal
	for(i=0;i<m_points.size();++i){
	  // add vectors up
	  comp308::vec3 norm = comp308::vec3(0);
	  for(j=0;j<triangles_which_intersect_point[i].size();++j){
	    norm = norm + face_normals[triangles_which_intersect_point[i][j]];
	  }
	  // normalize and save
	  if(triangles_which_intersect_point[i].size()>0){
	    m_normals[i] = normalize(norm);
	  }
	}
	// for each triangle add normals to the vertexes
	for(i=0;i<m_triangles.size();++i){
	  m_triangles[i].v[0].n=m_triangles[i].v[0].p;
	  m_triangles[i].v[1].n=m_triangles[i].v[1].p;
	  m_triangles[i].v[2].n=m_triangles[i].v[2].p;
	}
}

void Geometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	m_displayListPoly = glGenLists(1);
	
	glNewList(m_displayListPoly, GL_COMPILE);
	  glPolygonMode(GL_FRONT, GL_FILL);
          glPolygonMode(GL_BACK, GL_FILL);
	  unsigned int i;
	  for(i = 0;i<m_triangles.size();++i){
	    glBegin(GL_TRIANGLES);
	      
	      vertex vert = m_triangles[i].v[0];
	      vec3 norm = m_normals[vert.n];
	      glNormal3f(norm.x, norm.y, norm.z);
	      vec3 points = m_points[vert.p];
	      glVertex3f(points.x, points.y, points.z);
	      
	      vert = m_triangles[i].v[1];
	      norm = m_normals[vert.n];
	      glNormal3f(norm.x, norm.y, norm.z);
	      points = m_points[vert.p];
	      glVertex3f(points.x, points.y, points.z);
	      
	      vert = m_triangles[i].v[2];
	      norm = m_normals[vert.n];
	      glNormal3f(norm.x, norm.y, norm.z);
	      points = m_points[vert.p];
	      glVertex3f(points.x, points.y, points.z);
	      
	    glEnd();
	  }
	glEndList();
}

void Geometry::renderGeometry() {
	glShadeModel(GL_SMOOTH);
	glCallList(m_displayListPoly);
}