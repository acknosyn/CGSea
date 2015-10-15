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

float wave(float x, float z);
vec3 gradWave(float x, float z);

float AMPLITUDE = 0.01f;
float WAVESIZE = 0.01f;  // Frequency  default 10, 0.01

float FACTOR = 1.0f;
float SPEED = 1.0f; //default 2
int OCTAVES = 5; //default 5

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vPositionW;

uniform float timer = 1;

void main() {
	//Wave calculations.
	float x = vPositionW.x+100;
	float z = vPositionW.z+100;
	
	float wave = wave(x,z);
	
	vec3 dxdy = gradWave(x,z);
	
	vec3 dxdy2 = gradWave(x-80,z-50);
	
	//Intercept
	vec3 lineP = vPositionW;
	vec3 lineN = dxdy;
	vec3 planeN = vec3(0,1,0);
	float planeD = -0.8;
	
	float distance = (planeD - lineP.y) / lineN.y;
	vec3 intercept = lineP + lineN * distance;
	
	float prop1 = clamp((dxdy.z/length(dxdy)),0.0,1.0);
	float prop2 = clamp((dxdy2.z/length(dxdy2)),0.0,1.0);
	float proportion = clamp((prop1+prop2-0.5)*2/3, 0.0, 1.0);


   //Phong shading.
   vec3 Light = normalize(gl_LightSource[0].position.xyz - vPosition);   
   vec3 Eye = normalize(-vPosition); // we are in Eye Coordinates, so EyePos is (0,0,0)
   vec3 Reflect = normalize(-reflect(Light,vNormal));  
 
   //Ambient:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //Diffuse:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(vNormal,Light), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   //Specular:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(Reflect,Eye),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   
   //write Color:  
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff*proportion + Ispec*proportion;
}

float wave(float x, float z) {
	float y = 0.0f;
	int octaves = OCTAVES;
	float factor = FACTOR;
	float d = sqrt(x * x + z * z);
	do {
		y -= factor * cos(timer * SPEED + (1/factor) * x * z * WAVESIZE);
		factor = factor/2;
		octaves--;
	} while (octaves > 0);
	return 2 * AMPLITUDE * d * y;
}

vec3 gradWave(float x, float z) {
	//Derivative wave function
	float d = sqrt(x * x + z * z);
	float dYx = 0.0f;
	float dYz = 0.0f;
	int octaves = OCTAVES;
	float factor = FACTOR;

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

	return vec3(-2 * AMPLITUDE * dYx, 1,-2 * AMPLITUDE * dYz);
}