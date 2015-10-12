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

#version 120

float VTXSIZE = 0.01f;   // Amplitude

float WAVESIZE = 0.01f;  // Frequency  default 10, 0.01

float FACTOR = 1.0f;
float SPEED = 2.0f; //default 2
int OCTAVES = 5; //default 5

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vPositionW;

uniform float timer = 1;

void main() {
	//Now, do wave calculations.
	float y = 0.0f;
	float octaves = OCTAVES;
	float factor = FACTOR;
	float x = vPositionW.x+40;
	float z = vPositionW.z+40;
	float d = sqrt(x * x + z * z);
	do {
		y -= factor * cos(timer * SPEED + (1/factor) * x * z * WAVESIZE);
		factor = factor/2;
		octaves--;
	} while (octaves > 0);
	float wave = 2 * VTXSIZE * d * y;
	
	//Derivative wave function
	float dYx = 0.0f;
	float dYz = 0.0f;
	octaves = OCTAVES;
	factor = FACTOR;

	do {
		dYx += d * sin(timer * SPEED + (1/factor) * x * z * WAVESIZE) *
             z * WAVESIZE - factor *
             cos(timer * SPEED + (1/factor) * x * z * WAVESIZE) * x/d;
		dYz += d * sin(timer * SPEED + (1/factor) * x * z * WAVESIZE) *
             x * WAVESIZE - factor *
             cos(timer * SPEED + (1/factor) * x * z * WAVESIZE) * z/d;
		factor = factor/2;
		octaves--;
	} while (octaves > 0);

	vec3 dxdy = vec3(-2 * VTXSIZE * dYx, 1,-2 * VTXSIZE * dYz);
	
	//Intercept
	vec3 lineP = vPosition;
	vec3 lineN = dxdy;
	vec3 planeN = vec3(0,1,0);
	float planeD = -0.8;
	
	float distance = (planeD - lineP.y) / lineN.y;
	vec3 intercept = lineP + lineN * distance;
	
	
	float proportion=lineN.z/length(lineN)/0.8; //the 20 should be length(lineN.z)
	proportion = clamp(proportion, 0.0, 1.0);


   //Phong shading.
   vec3 L = normalize(gl_LightSource[0].position.xyz - vPosition);   
   vec3 E = normalize(-vPosition); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,vNormal));  
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(vNormal,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
	
	// write Total Color:  
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff*proportion + Ispec*proportion;
}