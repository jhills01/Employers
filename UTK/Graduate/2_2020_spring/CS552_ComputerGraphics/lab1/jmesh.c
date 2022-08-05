#include "offreader.h"

jmesh * 
new_jmesh(FILE * fp, struct dirent * file)
{
	jmesh * jm;

	jm = malloc(sizeof(jmesh));

	jm->fileName = strdup(file->d_name); 

	if      ( (strstr(file->d_name, ".off") != NULL) && (load_off_mesh(fp, jm) != -1) ) return jm;
	else if ( (strstr(file->d_name, ".obj") != NULL) && (load_obj_mesh(fp, jm) != -1) ) return jm;
	else if ( (strstr(file->d_name, ".ply") != NULL) && (load_ply_mesh(fp, jm) != -1) ) return jm;
	else
	{
		fprintf(stderr, "Error: failed to load \"%s\" mesh file\n", file->d_name);
		exit(1);
	}
}

void
free_jmarray(jmesh ** jmArr)
{
	//free all member pointers inside jmesh FIRST 
	
	//free jmesh pointer LAST
	free(jmArr);
}
