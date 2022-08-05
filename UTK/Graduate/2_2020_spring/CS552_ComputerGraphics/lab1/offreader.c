#include "offreader.h"

//
int
load_off_mesh(FILE *fp, jmesh *jm)
{
	char line[1024];
	char * word;
	int i, vOneLineCnt, fTotLineCnt;
	int j, k; 

	//read 1st line which should be "OFF"
	if ( (fgets(line, 1024, fp) == NULL) || (strcmp(line, "OFF\n") != 0) ) 
	{
		fprintf(stderr, "Error: either fgets failed getting 1st line OR %s != OFF/n/0 (NOTE: /n == newline, /0 == null terminator)\n", line);
		return -1;
	}

	//read 2nd line which should be vertex cnt, face cnt, and edge cnt. 
	//allocate 3 * vertex cnt to store x, y, z coordinates for all vertices
	//ignore face and edge cnt
	if ( fgets(line, 1024, fp) == NULL ) 
	{
		fprintf(stderr, "Error: fgets failed getting 2nd line\n");
		return -1;
	}
	sscanf(line, "%d %d", &jm->nvert, &fTotLineCnt); 
	jm->vertices = malloc( (3*jm->nvert) * sizeof(float) );
	
	//count number of triangles. Non-triangle faces will be transformed using fan triangulation. 
	for (i = 0; i < jm->nvert; ++i) fgets(line, 1024, fp); //skip vertex lines 
	jm->ntri = 0;
	while ( fgets(line, 1024, fp) != NULL )	
	{
		sscanf(line, "%d", &vOneLineCnt);
		jm->ntri += vOneLineCnt - 2; 
	}
	jm->triangles = malloc( (3*jm->ntri) * sizeof(int) );
	
	//rewind file pointer to beginning of mesh file and skip first two lines	
	if ( fseek(fp, 0L, SEEK_SET) != 0 ) 
	{
		fprintf(stderr, "Error: fseek back to beginning of %s failed\n", jm->fileName);
		return -1;
	}
	fgets(line, 1024, fp); 
	fgets(line, 1024, fp); 

	//load vertices and process min/max x,y,z and centroid values
	jm->minX = 1.0e20;
	jm->minY = 1.0e20;
	jm->minZ = 1.0e20;
	jm->maxX = 1.0e-20;
	jm->maxY = 1.0e-20;
	jm->maxZ = 1.0e-20;
	for (i = 0; i < jm->nvert; ++i)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed getting vertex coordinates on line %s\n", line);
			return -1;
		}
		
		if ( sscanf(line, "%f %f %f", &jm->vertices[3*i + 0], &jm->vertices[3*i + 1], &jm->vertices[3*i + 2]) != 3 )
		{
			fprintf(stderr, "Error: sscanf failed to read 3 vertex coordinates into jmesh on line: %s\n", line);
			return -1;
		}
		else //check min/max x,y,z values on successful scan
		{
			//check minimum values
			if (jm->vertices[3*i    ] < jm->minX) jm->minX = jm->vertices[3*i    ];	
			if (jm->vertices[3*i + 1] < jm->minY) jm->minY = jm->vertices[3*i + 1];	
			if (jm->vertices[3*i + 2] < jm->minZ) jm->minZ = jm->vertices[3*i + 2];	
			
			//check maximum values
			if (jm->vertices[3*i    ] > jm->maxX) jm->maxX = jm->vertices[3*i    ];	
			if (jm->vertices[3*i + 1] > jm->maxY) jm->maxY = jm->vertices[3*i + 1];	
			if (jm->vertices[3*i + 2] > jm->maxZ) jm->maxZ = jm->vertices[3*i + 2];	
		}
	}
	
	jm->centroid[0] = (jm->minX + jm->maxX) / 2.0; 
	jm->centroid[1] = (jm->minY + jm->maxY) / 2.0; 
	jm->centroid[2] = (jm->minZ + jm->maxZ) / 2.0; 

	//load triangles
	//i == file line index, j == jm->triangles array index, k tracks reading the first 3 vertices of one "face line"
	j = 0;
	for (i = 0, k = 0; i < fTotLineCnt; ++i, k = 0)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed getting vertex coordinates on line %s\n", line);
			return -1;
		}
	
		//remove first word from line and then perform triangle fan algorithm for OFF formatted mesh files
		word = strtok(line, " \n");
		while (1)
		{
			if ( (word = strtok(NULL, " \n")) == NULL ) break;
			
			if ( k < 3 && (sscanf(word, "%d", &jm->triangles[j]) == 1) ) 
			{
				++j;
				++k;
			}
			else if ( strstr(word, ".") == NULL )
			{
				sscanf(word, "%d", &jm->triangles[j + 2]);
				jm->triangles[j] = jm->triangles[j - 3];
				jm->triangles[j + 1] = jm->triangles[j - 1];
				j += 3;
			}
			else break;
		}
	}

	//successfully loaded mesh files
	return 1;
}

//
int
load_obj_mesh(FILE *fp, jmesh *jm)
{
	char line[1024];
	char lineCpy[1024];
	char * word;
	int i, fTotLineCnt;
	int j, k, m; 

	//read in and count vertices and triangles and allocate memory
	jm->nvert = 0;
	jm->ntri = 0;

	while ( fgets(line, 1024, fp) )
	{
		//read in first word delimited by white space
		if ( (word = strtok(line, " \n")) == NULL ) continue;
		
		//line contains geometric vertex coordinate 
		if ( strcmp(word, "v") == 0 ) ++jm->nvert;
		
		//line contains face vertices
		if ( strcmp(word, "f") == 0 ) 
		{
			i = 0;
			while ( strtok(NULL, " \n") ) ++i;
			jm->ntri += i - 2;
		}
	}
			
	jm->vertices = malloc( (3*jm->nvert) * sizeof(float) );
	jm->triangles = malloc( (3*jm->ntri) * sizeof(int) );
	
	//rewind file pointer to beginning of mesh file 
	if ( fseek(fp, 0L, SEEK_SET) != 0 ) 
	{
		fprintf(stderr, "Error: fseek back to beginning of %s failed\n", jm->fileName);
		return -1;
	}
	
	//read in geometric vertex coordinates with min/max x,y,z values and triangulated faces 
	jm->minX = 1.0e20;
	jm->minY = 1.0e20;
	jm->minZ = 1.0e20;
	jm->maxX = 1.0e-20;
	jm->maxY = 1.0e-20;
	jm->maxZ = 1.0e-20;
	i = -1;
	j = 0;
	while ( fgets(line, 1024, fp) )
	{
		//copy line --- strtok alters original line!
		strcpy(lineCpy, line);

		//read in first word delimited by white space
		if ( (word = strtok(line, " \n")) == NULL ) continue;
		
		//line contains geometric vertex coordinates 
		if ( strcmp(word, "v") == 0) 
		{
			m = 0;
			while (m < 3) 
			{
				++i;
				++m;
				word = strtok(NULL, " \n");
				jm->vertices[i] = atof(word);
			}
			
			//check minimum values
			if (jm->vertices[i - 2] < jm->minX) jm->minX = jm->vertices[i - 2];	
			if (jm->vertices[i - 1] < jm->minY) jm->minY = jm->vertices[i - 1];	
			if (jm->vertices[i    ] < jm->minZ) jm->minZ = jm->vertices[i    ];	
			
			//check maximum values
			if (jm->vertices[i - 2] > jm->maxX) jm->maxX = jm->vertices[i - 2];	
			if (jm->vertices[i - 1] > jm->maxY) jm->maxY = jm->vertices[i - 1];	
			if (jm->vertices[i    ] > jm->maxZ) jm->maxZ = jm->vertices[i    ];	
		}
		
		//line contains face vertices
		else if ( strcmp(word, "f") == 0) 
		{
			//get total number of vertices for this face
			fTotLineCnt = 0;
			while ( strtok(NULL, " \n") ) ++fTotLineCnt; 
	
			//re-start token process on line
			strcpy(line, lineCpy);
			word = strtok(line, " \n");
		
			//triangulate given face
			for (i = 0, k = 0, m = 0; i < fTotLineCnt; ++i, k = 0)
			{
				while (1)
				{
					if ( (word = strtok(NULL, " \n")) == NULL ) break;

					//replace first occurence of '/' in "word" to '\0'
					//if ' ' or '\0' are found before '/', "word" remains unaltered
					while(1)
					{
						if ( word[m] == '/' ) {word[m] = '\0'; break;}
						if ( word[m] == ' ' || word[m] == '\0' ) {break;}
						++m;
					}
					
					if ( k < 3 && (sscanf(word, "%d", &jm->triangles[j]) == 1) ) 
					{
						++j;
						++k;
					}
					else if ( strstr(word, ".") == NULL )
					{
						sscanf(word, "%d", &jm->triangles[j + 2]);
						jm->triangles[j] = jm->triangles[j - 3];
						jm->triangles[j + 1] = jm->triangles[j - 1];
						j += 3;
					}
					else break;
				}
			}
		}
	}

	jm->centroid[0] = (jm->minX + jm->maxX) / 2.0; 
	jm->centroid[1] = (jm->minY + jm->maxY) / 2.0; 
	jm->centroid[2] = (jm->minZ + jm->maxZ) / 2.0; 

	//successfully loaded mesh files
	return 1;
}

//
int
load_ply_mesh(FILE *fp, jmesh *jm)
{
	char line[1024];
	char * word;
	int i, vOneLineCnt, fTotLineCnt;
	int j, k; 

	//read 1st line which should be "OFF"
	if ( (fgets(line, 1024, fp) == NULL) || (strcmp(line, "ply\n") != 0) ) 
	{
		fprintf(stderr, "Error: either fgets failed getting 1st line OR %s != \"ply/n\" (NOTE: /n == newline)\n", line);
		return -1;
	}

	//read through header and store vertex/face numbers
	//allocate float array for vertex coordinates
	while (1)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed while reading header information\n");
			return -1;
		}
		
		word = strtok(line, " \n");
			
		if ( strcmp(word, "element") == 0)
		{
			word = strtok(NULL, " \n");
			if (strcmp(word, "vertex") == 0) 
			{
				word = strtok(NULL, " \n");
				sscanf(word, "%d", &jm->nvert);
			}
			else if (strcmp(word, "face") == 0) 
			{
				word = strtok(NULL, " \n");
				sscanf(word, "%d", &fTotLineCnt);
			}
		}
		else if ( strcmp(word, "end_header") == 0) break; 
	}
	
	jm->vertices = malloc( (3*jm->nvert) * sizeof(float) );

	//count number of triangles. Non-triangle faces will be transformed using fan triangulation. 
	for (i = 0; i < jm->nvert; ++i) fgets(line, 1024, fp); //skip vertex lines 
	jm->ntri = 0;
	while ( fgets(line, 1024, fp) != NULL )	
	{
		sscanf(line, "%d", &vOneLineCnt);
		jm->ntri += vOneLineCnt - 2; 
	}
	jm->triangles = malloc( (3*jm->ntri) * sizeof(int) );
	
	//rewind file pointer to beginning of mesh file and skip first two lines	
	if ( fseek(fp, 0L, SEEK_SET) != 0 ) 
	{
		fprintf(stderr, "Error: fseek back to beginning of %s failed\n", jm->fileName);
		return -1;
	}
	
	//re-read through header information until vertex coordinates are reached
	while (1)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed while RE-reading header information\n");
			return -1;
		}
		
		word = strtok(line, " \n");
		if ( strcmp(word, "end_header") == 0) break; 
	}

	//load vertices and process min/max x,y,z and centroid values
	jm->minX = 1.0e20;
	jm->minY = 1.0e20;
	jm->minZ = 1.0e20;
	jm->maxX = 1.0e-20;
	jm->maxY = 1.0e-20;
	jm->maxZ = 1.0e-20;
	for (i = 0; i < jm->nvert; ++i)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed getting vertex coordinates on line %s\n", line);
			return -1;
		}
		
		if ( sscanf(line, "%f %f %f", &jm->vertices[3*i + 0], &jm->vertices[3*i + 1], &jm->vertices[3*i + 2]) != 3 )
		{
			fprintf(stderr, "Error: sscanf failed to read 3 vertex coordinates into jmesh on line: %s\n", line);
			return -1;
		}
		else //check min/max x,y,z values on successful scan
		{
			//check minimum values
			if (jm->vertices[3*i    ] < jm->minX) jm->minX = jm->vertices[3*i    ];	
			if (jm->vertices[3*i + 1] < jm->minY) jm->minY = jm->vertices[3*i + 1];	
			if (jm->vertices[3*i + 2] < jm->minZ) jm->minZ = jm->vertices[3*i + 2];	
			
			//check maximum values
			if (jm->vertices[3*i    ] > jm->maxX) jm->maxX = jm->vertices[3*i    ];	
			if (jm->vertices[3*i + 1] > jm->maxY) jm->maxY = jm->vertices[3*i + 1];	
			if (jm->vertices[3*i + 2] > jm->maxZ) jm->maxZ = jm->vertices[3*i + 2];	
		}
	}
	
	jm->centroid[0] = (jm->minX + jm->maxX) / 2.0; 
	jm->centroid[1] = (jm->minY + jm->maxY) / 2.0; 
	jm->centroid[2] = (jm->minZ + jm->maxZ) / 2.0; 

	//load triangles
	//i == file line index, j == jm->triangles array index, k tracks reading the first 3 vertices of one "face line"
	j = 0;
	for (i = 0, k = 0; i < fTotLineCnt; ++i, k = 0)
	{
		if ( fgets(line, 1024, fp) == NULL ) 
		{
			fprintf(stderr, "Error: fgets failed getting vertex coordinates on line %s\n", line);
			return -1;
		}
	
		//remove first word from line and then perform triangle fan algorithm for OFF formatted mesh files
		word = strtok(line, " \n");
		while (1)
		{
			if ( (word = strtok(NULL, " \n")) == NULL ) break;
			
			if ( k < 3 && (sscanf(word, "%d", &jm->triangles[j]) == 1) ) 
			{
				++j;
				++k;
			}
			else if ( strstr(word, ".") == NULL )
			{
				sscanf(word, "%d", &jm->triangles[j + 2]);
				jm->triangles[j] = jm->triangles[j - 3];
				jm->triangles[j + 1] = jm->triangles[j - 1];
				j += 3;
			}
			else break;
		}
	}

	//successfully loaded mesh files
	return 1;
}
