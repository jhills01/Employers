#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct 
{
	//bounding box info
	float minX, maxX, minY, maxY, minZ, maxZ;
	float centroid[3]; //0 == x, 1 == y, 2 == z
	
	int nvert;
	int ntri;
	float * vertices;
	float * normals;
	int * triangles;
	char * fileName; //must include file extension (e.g. bunny.off)

} jmesh;


/* return value: pointer to jmesh upon success, NULL upon failure*/
jmesh * new_jmesh(FILE *, struct dirent *);
void free_jmesh(jmesh **);


