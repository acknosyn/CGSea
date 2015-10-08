/////////////////////////////////////////////////////////////////////////////////////////////
//	FileName:	MarchingCubes.cpp
//	Author	:	Michael Y. Polyakov
//	email	:	myp@andrew.cmu.edu  or  mikepolyakov@hotmail.com
//	website	:	www.angelfire.com/linux/myp
//	date	:	July 2002
//	
//	Description:	Marching Cubes Algorithm
/////////////////////////////////////////////////////////////////////////////////////////////

#include "marchingCubes.hpp"

vec3 LinearInterp(vec4 p1, vec4 p2, float value)
{
	vec3 p;

	if(p1.w != p2.w){
		float proportion = (value - p1.w) / (p2.w - p1.w);
		p.x = p1.x + (p2.x - p1.x)*proportion;
		p.y = p1.y + (p2.y - p1.y)*proportion;
		p.z = p1.z + (p2.z - p1.z)*proportion;
	}else { 
		p.x = p1.x; p.y = p1.y; p.z = p1.z;
	}
	return p;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	MARCHING CUBES	//

//  VERSION  1A).  //
TRIANGLE* MarchingCubes(int ncellsX, int ncellsY, int ncellsZ, float minValue, vec4 * points,  
										INTERSECTION intersection, int &numTriangles)
{
	TRIANGLE * triangles = new TRIANGLE[3*ncellsX*ncellsY*ncellsZ];//this should be enough space, if not change 4 to 5
	numTriangles = int(0);

	int YtimeZ = (ncellsY+1)*(ncellsZ+1);
	//go through all the points
	for(int i=0; i < ncellsX; i++)			//x axis
		for(int j=0; j < ncellsY; j++)		//y axis
			for(int k=0; k < ncellsZ; k++)	//z axis
			{
				//initialize vertices
				vec4 verts[8];
				int ind = i*YtimeZ + j*(ncellsZ+1) + k;
   /*(step 3)*/ verts[0] = points[ind];
				verts[1] = points[ind + YtimeZ];
				verts[2] = points[ind + YtimeZ + 1];
				verts[3] = points[ind + 1];
				verts[4] = points[ind + (ncellsZ+1)];
				verts[5] = points[ind + YtimeZ + (ncellsZ+1)];
				verts[6] = points[ind + YtimeZ + (ncellsZ+1) + 1];
				verts[7] = points[ind + (ncellsZ+1) + 1];
				
				//get the index
				int cubeIndex = int(0);
				for(int n=0; n < 8; n++)
   /*(step 4)*/		if(verts[n].w <= minValue) cubeIndex |= (1 << n);

				//check if its completely inside or outside
   /*(step 5)*/ if(!edgeTable[cubeIndex]) continue;
			
				//get intersection vertices on edges and save into the array
   				vec3 intVerts[12];
   /*(step 6)*/ if(edgeTable[cubeIndex] & 1) intVerts[0] = intersection(verts[0], verts[1], minValue);
				if(edgeTable[cubeIndex] & 2) intVerts[1] = intersection(verts[1], verts[2], minValue);
				if(edgeTable[cubeIndex] & 4) intVerts[2] = intersection(verts[2], verts[3], minValue);
				if(edgeTable[cubeIndex] & 8) intVerts[3] = intersection(verts[3], verts[0], minValue);
				if(edgeTable[cubeIndex] & 16) intVerts[4] = intersection(verts[4], verts[5], minValue);
				if(edgeTable[cubeIndex] & 32) intVerts[5] = intersection(verts[5], verts[6], minValue);
				if(edgeTable[cubeIndex] & 64) intVerts[6] = intersection(verts[6], verts[7], minValue);
				if(edgeTable[cubeIndex] & 128) intVerts[7] = intersection(verts[7], verts[4], minValue);
				if(edgeTable[cubeIndex] & 256) intVerts[8] = intersection(verts[0], verts[4], minValue);
				if(edgeTable[cubeIndex] & 512) intVerts[9] = intersection(verts[1], verts[5], minValue);
				if(edgeTable[cubeIndex] & 1024) intVerts[10] = intersection(verts[2], verts[6], minValue);
				if(edgeTable[cubeIndex] & 2048) intVerts[11] = intersection(verts[3], verts[7], minValue);

				//now build the triangles using triTable
				for (int n=0; triTable[cubeIndex][n] != -1; n+=3) {
   /*(step 7)*/ 	triangles[numTriangles].p[0] = intVerts[triTable[cubeIndex][n+2]];
					triangles[numTriangles].p[1] = intVerts[triTable[cubeIndex][n+1]];
					triangles[numTriangles].p[2] = intVerts[triTable[cubeIndex][n]];
   /*(step 8)*/ 	triangles[numTriangles].norm = normalize(cross((triangles[numTriangles].p[1] - 
						triangles[numTriangles].p[0]),(triangles[numTriangles].p[2] - 
						triangles[numTriangles].p[0])));
					numTriangles++;
				}
			
			}	//END OF FOR LOOP
		
		//free all the wasted space
		TRIANGLE * retTriangles = new TRIANGLE[numTriangles];
		for(int i=0; i < numTriangles; i++)
			retTriangles[i] = triangles[i];
		delete [] triangles;
	
	return retTriangles;
}


//	VERSION  1B).  //
TRIANGLE* MarchingCubesLinear(int ncellsX, int ncellsY, int ncellsZ, float minValue, 
									vec4 * points, int &numTriangles)
{
	return MarchingCubes(ncellsX, ncellsY, ncellsZ, minValue, points, LinearInterp, numTriangles);
}


//	VERSION  2A).  //
TRIANGLE* MarchingCubes(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ, 
							int ncellsX, int ncellsY, int ncellsZ, float minValue, 
							FORMULA formula, INTERSECTION intersection, int &numTriangles)
{
	//space is already defined and subdivided, staring with step 3
	//first initialize the points
	vec4 * mcDataPoints = new vec4[(ncellsX+1)*(ncellsY+1)*(ncellsZ+1)];
	vec3 stepSize((mcMaxX-mcMinX)/ncellsX, (mcMaxY-mcMinY)/ncellsY, (mcMaxZ-mcMinZ)/ncellsZ);
	
	int YtimesZ = (ncellsY+1)*(ncellsZ+1);	//for extra speed
	for(int i=0; i < ncellsX+1; i++) {
		int ni = i*YtimesZ;						//for speed
		float vertX = mcMinX + i*stepSize.x;
		for(int j=0; j < ncellsY+1; j++) {
			int nj = j*(ncellsZ+1);				//for speed
			float vertY = mcMinY + j*stepSize.y;
			for(int k=0; k < ncellsZ+1; k++) {
				vec4 vert(vertX, vertY, mcMinZ + k*stepSize.z, 0);
				vec3 vert3d(vertX, vertY, mcMinZ + k*stepSize.z);
				vert.w = formula(vert3d);
   /*(step 3)*/ mcDataPoints[ni + nj + k] = vert;
			}
		}
	}
	//then run Marching Cubes (version 1A) on the data
	return MarchingCubes(ncellsX, ncellsY, ncellsZ, minValue, mcDataPoints, intersection, numTriangles);
}

//	VERSION  2B).  //
TRIANGLE* MarchingCubesLinear(float mcMinX, float mcMaxX, float mcMinY, float mcMaxY, float mcMinZ, float mcMaxZ, 
								int ncellsX, int ncellsY, int ncellsZ, float minValue, 
								FORMULA formula, int &numTriangles)
{
	return MarchingCubes(mcMinX, mcMaxX, mcMinY, mcMaxY, mcMinZ, mcMaxZ, ncellsX, ncellsY, ncellsZ, minValue,
		formula, LinearInterp, numTriangles);
}