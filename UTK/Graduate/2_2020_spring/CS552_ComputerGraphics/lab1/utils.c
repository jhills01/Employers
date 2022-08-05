/* utils.c */
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "offreader.h"

typedef struct
{
	//[0] == x component, [1] == y component, [2] == z component 
	float orbitCurrDegree[3]; 
	float orbitDegreeChange[3]; 
	
	float orbitCurrDist[3];
	//float orbitDistRate[3];
	
	float spinCurrDegree[3];
	float spinDegreeChange[3];  
} Cube;

static graphics_state * current_gs;
static unsigned char *pixels;
static FILE *fp;
static Cube cubes[9];
static float zoom;

void
createHeavenlyBodies(void)
{
	int m, n; 
	float distOffset;//, distOffsetRateChange;
	float orbitOffset, orbitOffsetRateChange;
	float spinOffset, spinOffsetRateChange;

	//increasing array indices correlates with distance from sun: 
	//Sun, Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
	distOffset = 2.0;
	//distOffsetRateChange = 0.01;
	
	orbitOffset = 1.0;
	orbitOffsetRateChange = 0.01;

	spinOffset = 2.0;
	spinOffsetRateChange = 2.0;
	
	//configure SUN
	m = 0;
	
	n = 0;
	while (n < 3) {cubes[m].orbitCurrDegree[n] = 0; ++n;}
	n = 0;
	while (n < 3) {cubes[m].orbitDegreeChange[n] = 0; ++n;}
	n = 0;
	while (n < 3) {cubes[m].orbitCurrDist[n] = 0; ++n;}
	n = 0;
	while (n < 3) {cubes[m].spinCurrDegree[n] = 0; ++n;}
	n = 0;
	while (n < 3) {cubes[m].spinDegreeChange[n] = 0; ++n;}
	
	//configure NEPTUNE
	m = 8;
	float test;
	test = 1.0;
	
	n = 0;
	while (n < 3) {cubes[m].orbitCurrDegree[n] = test; ++n;}
	n = 0;
	while (n < 3) {cubes[m].orbitDegreeChange[n] = test; ++n;}
	n = 0;
	while (n < 3) {cubes[m].orbitCurrDist[n] = 20.0*test; ++n;}
	n = 0;
	while (n < 3) {cubes[m].spinCurrDegree[n] = 10.0*test; ++n;}
	n = 0;
	while (n < 3) {cubes[m].spinDegreeChange[n] = test; ++n;}
	
	//for (m = 0; m < 9; ++m)
	//configure other PLANETS
	for (m = 7; m > 0; --m)
	{
		//TRANSLATIONS	
		cubes[m].orbitCurrDist[0] = cubes[m + 1].orbitCurrDist[0] - (1.3*distOffset); // x
		cubes[m].orbitCurrDist[1] = 0.0; // y
		cubes[m].orbitCurrDist[2] = 0.0; // z
		
		//ROTATIONS
		cubes[m].orbitCurrDegree[0] = 1.0; // x
		cubes[m].orbitCurrDegree[1] = 1.0; // y
		cubes[m].orbitCurrDegree[2] = 1.0; // z

		cubes[m].orbitDegreeChange[0] = 1.0; // x
		cubes[m].orbitDegreeChange[1] = cubes[m + 1].orbitDegreeChange[1] + orbitOffset; // y
		cubes[m].orbitDegreeChange[2] = 1.0; // z
		
		cubes[m].spinCurrDegree[0] = 1.0; // x 
		cubes[m].spinCurrDegree[1] = 1.0; // y
		cubes[m].spinCurrDegree[2] = 1.0; // z
		
		cubes[m].spinDegreeChange[0] = 1.0; // x
		cubes[m].spinDegreeChange[1] = cubes[m + 1].spinDegreeChange[1] - spinOffset; // y
		cubes[m].spinDegreeChange[2] = 1.0; // z

		//distOffset += distOffsetRateChange;
		orbitOffset += orbitOffsetRateChange;
		spinOffset += spinOffsetRateChange;
	}
}

void 
init(graphics_state * gs)
{
	current_gs = gs;
	zoom = 20.0;

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black BKG color
	glClearColor(1.0f, 0.5f, 0.0f, 1.0f); //orange BKG color
	
	createHeavenlyBodies();

	//used with glReadPixels
	pixels = malloc(3 * current_gs->width * current_gs->height);
}

void
drawMyCube(void)
{
	int i, j;

	glBegin ( GL_TRIANGLES );
		for (i = 0; i < current_gs->jm->ntri; ++i)
			for (j = 0; j < 3; ++j)
				glVertex3fv(&current_gs->jm->vertices[current_gs->jm->triangles[i*3 + j]*3]);
	glEnd();
}

void 
displaySolarSystem(void)
{
	int i; 
	
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//MODEL VIEW
	glMatrixMode(GL_MODELVIEW);
	for (i = 0; i < 8; ++i)
	{
		glLoadIdentity();

		//IMPORTANT NOTE: Transformations are processed bottom to top (i.e., reverse order) from what's listed below. It's a stack!
		//-----ORBIT
		glRotatef(cubes[i].orbitCurrDegree[1], 0.0, 1.0, 0.0); //y -- actual value
		cubes[i].orbitCurrDegree[1] = (int)(cubes[i].orbitCurrDegree[1] + cubes[i].orbitDegreeChange[1]) % 360 ; //y
		
		glTranslatef( cubes[i].orbitCurrDist[0], cubes[i].orbitCurrDist[1], cubes[i].orbitCurrDist[2]);

		//----SPIN 
		glRotatef(cubes[i].spinCurrDegree[1], 0.0, 1.0, 0.0); //y -- actual value
		cubes[i].spinCurrDegree[1] = (int)(cubes[i].spinCurrDegree[1] + cubes[i].spinDegreeChange[1]) % 360 ; //y

		glTranslatef( -current_gs->jm->centroid[0], -current_gs->jm->centroid[1], -current_gs->jm->centroid[2]);  
	
		//----DRAW
		drawMyCube();
	}
	
	//PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, 1.0, 0.1, 100.0); //2nd paramter == 1 b/c 512/512
	gluLookAt(0,zoom,0,   current_gs->jm->centroid[0],current_gs->jm->centroid[1],current_gs->jm->centroid[2],   0,1,0);
	
	glutSwapBuffers();
}

void 
displayLoadedMesh(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawMyCube();

	//MODEL VIEW
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, 1.0, 0.1, 100.0); //2nd paramter == 1 b/c 512/512
	gluLookAt(0,0,zoom,   current_gs->jm->centroid[0],current_gs->jm->centroid[1],current_gs->jm->centroid[2],   0,1,0);
	
	glutSwapBuffers();
}

void 
reshape(int w, int h)
{

}

void 
mouse_handler(int button, int button_state, int x, int y)
{
  
}

void 
trackMotion(int x, int y) 
{

}

void 
keys(unsigned char c, int x, int y) 
{
	int i, j;
	float deltaZ;
	
	deltaZ = 2.0;
	switch (c)
	{
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
		case 'x': case 'X':
			zoom += deltaZ;
			if (zoom > 100) { zoom = 100; printf("zoom out at maximum value of 100!\n"); } 
			break;
		case 'z': case 'Z':
			zoom -= deltaZ;
			if (zoom < 1) { zoom = 1; printf("zoom out at minimum value of 1!\n"); } 
			break;
		case 's': case 'S':
			//open screenshot ppm file
			if ( (fp = fopen("screenshot.ppm","w")) == NULL )
			{
				fprintf(stderr, "Error: failed to open \"screenshot.ppm\". Exiting...\n");
				exit(1);
			}
		
			//get RBG data from frame buffer
			glReadPixels(0, 0, current_gs->width, current_gs->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

			//write out ppm data
			j = 3 * current_gs->width * (current_gs->height - 1); 
			
			fprintf(fp, "P3\n%d %d\n255\n", current_gs->width, current_gs->height);
			
			for (i = current_gs->height; i >= 1; --i, j = 3 * current_gs->width * (i - 1))
			{
				for ( ; j < 3*i*current_gs->width; j += 3)
				{
					fprintf(fp, "%d %d %d  ", pixels[j], pixels[j + 1], pixels[j + 2]);	
				}
				fprintf(fp, "\n");	
			}
			
			fclose(fp);
			break;
	}
}

//CODE STORAGE
//printf("%-2d: spin: %-10f orbit: %-10f\n", i, cubes[i].spinCurrDegree[1], cubes[i].orbitCurrDegree[1]);
