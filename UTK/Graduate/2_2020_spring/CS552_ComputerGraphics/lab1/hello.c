/* hello.c */
#include "offreader.h" //includes "jmesh.h", "utils.h", and "string.h"
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

//returns 1 if s == "off", "obj", "ply", or "all"
//exits if s == "ext"
//0 otherwise
int
inputChk(char *s)
{
	if (strcmp(s, "all") == 0 || strcmp(s, "obj") == 0 || strcmp(s, "off") == 0 || strcmp(s, "ply") == 0)
		return 1;
	else if (strcmp(s, "ext") == 0) 
	{
		printf("\nExiting...\n\n");
		exit(0);
	}
	else return 0;
}

//concatenates dPath, "/",  and file->d_name strings to create relative path to mesh file 
char *
build_path(struct dirent * file, char * dPath)
{
	char * pathToFile;
	int strSize;
	
	strSize = strlen(dPath) + strlen(file->d_name) + 2; //"+ 2" for extra '/' and null terminating characters

	pathToFile = malloc(strSize); 
	strcpy(pathToFile, dPath);
	pathToFile += strlen(dPath);
	strcpy(pathToFile, "/");
	pathToFile += 1;
	strcpy(pathToFile, file->d_name);
	pathToFile -= strlen(dPath) + 1;

	return pathToFile;
}				

//input is relative path to a sub-directory within execuatable directory containing mesh files
int
count_meshes(char * dPath)
{
	DIR * fd;
	struct dirent * file;
	int fileCnt, i, strSize;
	char * dirNoSlash; //e.g., "./off" to ".off"
	char * dirNoSlashDot; //e.g., "./off" to "off" 

	//make strings related to dPath
	strSize = strlen(dPath) + 1; //"+ 1" for '\0' 
	dirNoSlash = malloc(strSize - 1); //"strSize - 1" to not count '/'
	dirNoSlashDot = malloc(strSize - 2); //"strSize - 2" to not count './'
	dirNoSlash[0] = '.';
	for (i = 1; i < strSize; ++i) dirNoSlash[i] = dPath[i + 1]; 
	for (i = 2; i < strSize; ++i) dirNoSlashDot[i] = dPath[i + 2]; 
	
	//count files 
	if ( (fd = opendir (dPath)) == NULL ) 
	{
		fprintf(stderr, "Error : Failed to open %s directory - %s\n", dirNoSlashDot, strerror(errno));
		exit(1);
	}

	fileCnt = 0;
	while ( (file = readdir(fd)) != NULL ) 
	{
		//don't count ".", "..", or files with incorrect ext.
		//case of files extensions is assumed to be lowercase (e.g., ".off")
		if ( (strcmp (file->d_name, ".") == 0) || (strcmp (file->d_name, "..") == 0) || (strstr(file->d_name, dirNoSlash) == NULL) ) continue;
		++fileCnt;
	}

	//if "readdir(fd) == NULL" did NOT occur due to EOF, then errno != 0
	if (errno != 0) 
	{
		fprintf(stderr, "Error : readdir failed to read all files in \"%s\" directory - %s\n", dirNoSlashDot, strerror(errno));
		exit(1);
	}

	//clean up file descriptors and strings
	closedir(fd);
	free(dirNoSlash);
	free(dirNoSlashDot);
	
	return fileCnt;
}

//creates array of jmesh objects requested by user. 
//This array is ordered by mesh file type:
//	1) .off
//	2) .obj
//	3) .ply
jmesh **
load_meshes(char ** dPaths, int meshCnt)
{
	FILE * fp;
	DIR * fd;
	struct dirent * file;
	jmesh ** jmArr;
	int i, j, k, strSize;
	char * dirNoSlash; //e.g., "./off" to ".off"
	char * dirNoSlashDot; //e.g., "./off" to "off" 
	char * pathToFile; //e.g., "./off/bunny.off"

	jmArr = malloc( meshCnt * sizeof(jmesh *) );
	i = 0;

	for (j = 0; j < 3; ++j)
	{
		if ( strcmp(dPaths[j],"-") != 0 )
		{
			//make strings related to dPath
			strSize = strlen(dPaths[j]) + 1; //"+ 1" for '\0' 
			dirNoSlash = malloc(strSize - 1); //"strSize - 1" to not count '/'
			dirNoSlashDot = malloc(strSize - 2); //"strSize - 2" to not count './'
			dirNoSlash[0] = '.';
			for (k = 1; k < strSize; ++k) dirNoSlash[k] = dPaths[j][k + 1]; 
			for (k = 2; k < strSize; ++k) dirNoSlashDot[k] = dPaths[j][k + 2]; 
			
			if ( (fd = opendir (dPaths[j])) == NULL ) 
			{
				fprintf(stderr, "Error : Failed to open \"%s\" directory - %s\n", dPaths[j], strerror(errno));
				exit(1);
			}

			while ( (file = readdir(fd)) )
			{
				//don't open ".", "..", or mesh files with incorrect extension 
				if ( (strcmp (file->d_name, ".") == 0) || (strcmp (file->d_name, "..") == 0) || (strstr(file->d_name, dirNoSlash) == NULL) ) continue;

				//open OFF file in read-only
				pathToFile = build_path(file, dPaths[j]); //malloc called!
	
				fp = fopen(pathToFile, "r");
				if ( fp == NULL )
				{
					fprintf(stderr, "Error : Failed to open \"%s\" - %s\n", pathToFile, strerror(errno));
					exit(1);
				}

				//allocate OFF mesh array and load mesh data
				jmArr[i] = new_jmesh(fp, file);
				++i;
				
				//close file 
				fclose(fp);
			}

			//if "readdir(fd) == NULL" did NOT occur due to EOF, then errno != 0
			if (errno != 0) 
			{
				fprintf(stderr, "Error : readdir failed to read all files in \"%s\" directory - %s\n", dirNoSlashDot, strerror(errno));
				exit(1);
			}

			//clean up file descriptors and strings
			closedir(fd);
			free(dirNoSlash);
			free(dirNoSlashDot);
			free(pathToFile);
		}
	}

	return jmArr;
}

void
writeMesh(char * fileName, jmesh *jm)
{
	FILE *fp;
	int i;

	fp = fopen(fileName, "w");

	//write header information depending on type of file
	if ( strstr(jm->fileName, "off") != NULL )
	{
		fprintf(fp, "OFF\n");	
		fprintf(fp, "%d %d %d\n", jm->nvert, jm->ntri, 0);	
		
		//write vertices
		for (i = 0; i < 3*jm->nvert; i += 3) fprintf(fp, "%f %f %f\n", jm->vertices[i], jm->vertices[i + 1], jm->vertices[i + 2]);
		
		//write triangles
		for (i = 0; i < 3*jm->ntri; i += 3) fprintf(fp, "%d %d %d %d\n", 3, jm->triangles[i], jm->triangles[i + 1], jm->triangles[i + 2]);
	}
	else if ( strstr(jm->fileName, "obj") != NULL )
	{
		//write vertices
		for (i = 0; i < 3*jm->nvert; i += 3) fprintf(fp, "v %f %f %f\n", jm->vertices[i], jm->vertices[i + 1], jm->vertices[i + 2]);
		
		//write triangles
		for (i = 0; i < 3*jm->ntri; i += 3) fprintf(fp, "f %d %d %d %d\n", 3, jm->triangles[i], jm->triangles[i + 1], jm->triangles[i + 2]);
	}
	else if ( strstr(jm->fileName, "ply") != NULL )
	{
		fprintf(fp, "ply\n");	
		fprintf(fp, "format ascii 1.0\n");	
		fprintf(fp, "element vertex %d\n", jm->nvert);	
		fprintf(fp, "property float x\n");	
		fprintf(fp, "property float y\n");	
		fprintf(fp, "property float z\n");	
		fprintf(fp, "element face %d\n", jm->ntri);
		fprintf(fp, "property list uchar int vertex_indices\n");	
		fprintf(fp, "end_header\n");	
		
		//write vertices
		for (i = 0; i < 3*jm->nvert; i += 3) fprintf(fp, "%f %f %f\n", jm->vertices[i], jm->vertices[i + 1], jm->vertices[i + 2]);
		
		//write triangles
		for (i = 0; i < 3*jm->ntri; i += 3) fprintf(fp, "%d %d %d %d\n", 3, jm->triangles[i], jm->triangles[i + 1], jm->triangles[i + 2]);
	}

	fclose(fp);
}

int 
main(int argc, char **argv)
{
	jmesh * jm;
	jmesh ** jmArr; //this will hold pointers to all loaded mesh files of any tyep
	char input[4];
	char * dPaths[3]; //3 == number of different mesh file types. 
	int i, offCnt, objCnt, plyCnt, totCnt; //these counts will allow quicker access in jmArr since it's ordered (off files 1st, obj 2nd, ply 3rd) 
	
	//instruct user
	printf("\nAll mesh files of a common type should be grouped together in folders conforming to\n");
	printf("the following naming convention:\n\n");
	printf("\tOFF files in \"off\"\n");
	printf("\tPLY files in \"ply\"\n");
	printf("\tOBJ files in \"obj\"\n\n");
	printf("\tNOTE: If you wish to exclude mesh files from loading, place them in subfolders within\n");
	printf("\t      their corresponding mesh folders (e.g., ./off/exclude/bunny.off).\n\n");
	
	//request mesh file type from user, convert to lower case and check for correctness
requestMeshes:
	printf("Which mesh files do you wish to load? (\"off\", \"obj\", \"ply\", or \"all\" - \"ext\" to quit)\n");
	scanf("%s", input);
	input[3] = '\0';
	for (i = 0; i < 3; ++i) input[i] = tolower(input[i]);
	if( !inputChk(input) ) 
	{
		printf("\n\"%s\" is not valid input!\n\n", input);
		goto requestMeshes; 
	}

	//load requested meshes
	offCnt = 0;
	objCnt = 0;
	plyCnt = 0;
	errno = 0;
	
	if ( strcmp ("off", input) == 0 ) 
	{
		offCnt = count_meshes("./off");
		dPaths[0] = "./off";
		dPaths[1] = "-";
		dPaths[2] = "-";
		jmArr = load_meshes(dPaths, offCnt);
	}
	
	else if ( strcmp ("obj", input) == 0 ) 
	{  
		objCnt = count_meshes("./obj");
		dPaths[0] = "-";
		dPaths[1] = "./obj";
		dPaths[2] = "-";
		jmArr = load_meshes(dPaths, objCnt);
	}
	else if ( strcmp ("ply", input) == 0 ) 
	{  
		plyCnt = count_meshes("./ply");
		dPaths[0] = "-";
		dPaths[1] = "-";
		dPaths[2] = "./ply";
		jmArr = load_meshes(dPaths, plyCnt);
	}
	else                                   
	{  
		offCnt = count_meshes("./off");
		objCnt = count_meshes("./obj");
		plyCnt = count_meshes("./ply");
		dPaths[0] = "./off";
		dPaths[1] = "./obj";
		dPaths[2] = "./ply";
		jmArr = load_meshes(dPaths, (offCnt + objCnt + plyCnt));
	}

	//request specfic mesh from user
	totCnt = offCnt + objCnt + plyCnt;
requestMesh:
	printf("\nSelect mesh to process (\"ext\" to quit):\n");
	for (i = 0; i < totCnt; ++i) printf("\t%-4d: %s\n", i, jmArr[i]->fileName);
	scanf("%s", input); 
	if ( strcmp(input, "ext") == 0 ) exit(0);
	i = atoi(input);
	if ( i >= totCnt || i < 0 ) 
	{
		printf("\ninvalid input!\n\n");
		goto requestMesh; 
	}
	
	//process selected mesh file
	jm = jmArr[i];
	printf("\nno. of vertices:  %d\n", jm->nvert);
	printf("no. of triangles: %d\n", jm->ntri);
	printf("Bounding box:\n");
	printf("minX: %f    maxX: %f\n", jm->minX, jm->maxX);
	printf("minY: %f    maxY: %f\n", jm->minY, jm->maxY);
	printf("minZ: %f    maxZ: %f\n", jm->minZ, jm->maxZ);
	printf("Centroid: x: %f y: %f z: %f\n\n", jm->centroid[0], jm->centroid[1], jm->centroid[2]);

	printf("Do you wish to write mesh file to disk (\"y\" to write, anything else to skip)?\n");
	scanf("%s", input);
	printf("\n\n");
	if ( (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) && (strstr(jm->fileName, "off") != NULL) ) writeMesh("TEST.off", jm);
	if ( (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) && (strstr(jm->fileName, "obj") != NULL) ) writeMesh("TEST.obj", jm);
	if ( (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) && (strstr(jm->fileName, "ply") != NULL) ) writeMesh("TEST.ply", jm);

lmORss:
	printf("Do you wish to display loaded mesh (\"lm\") OR solar system (\"ss\")?\n");
	scanf("%s", input);
	printf("\n");
	if ( strcmp(input, "lm") == 0 || strcmp(input, "ss") == 0 ) {/*DO NOTHING*/} 
	else goto lmORss;
	printf("Use \"z\" to zoom in and \"x\" to zoom out!\n");

// -----------------------------------------------------------------------------------------------------

	graphics_state gs;

	gs.height=512;
	gs.width =512;
	gs.jm = jm;

	/* check for command line arguments */
	if(argc != 2)
	{
		printf("Usage: executable cubesize\n");
		exit(1);
	}

	gs.cubesize = strtod(argv[1], NULL);

	{ /* GLUT initialization */
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB);
		glutInitWindowSize(gs.width,gs.height);
		glutInitWindowPosition(100,100);
		glutCreateWindow(argv[0]);

		if (strcmp(input, "lm") == 0)
		{
			glutDisplayFunc(displayLoadedMesh);
			glutIdleFunc(displayLoadedMesh);
		}
		else
		{
			glutDisplayFunc(displaySolarSystem);
			glutIdleFunc(displaySolarSystem);
		}
		glutReshapeFunc(reshape);

		glutKeyboardFunc(keys);
		glutMouseFunc(mouse_handler);
		glutMotionFunc(trackMotion);
	}

	init(&gs);

	glutMainLoop();

	return 0;
}
