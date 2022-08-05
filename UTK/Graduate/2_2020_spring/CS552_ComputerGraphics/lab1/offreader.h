#include "utils.h"//utils.h includes "GL/glut.h", jmesh.h
#include <string.h>

/* return value: number of vertices upon success, -1 upon failure*/
int load_off_mesh(FILE *, jmesh *);
int load_obj_mesh(FILE *, jmesh *);
int load_ply_mesh(FILE *, jmesh *);

