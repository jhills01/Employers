// NAME:Tom Hills  NETID: jhills

// CS 360: Lab4 - tarc and tarx

// DESCRIPTION:
// This program mimics the functionality of the program tar, which bundles up files and directories...
// ...into a single file, and which can then use that file to recreate the files and directories.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dllist.h"
#include "jrb.h"
#include "fields.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/resource.h>

//struct to hold file information
typedef struct
{
	char *full_path; //This hold the full path to the file 
	char *rel_path; //This will be used in tar --- omit NULL when writing to stdout
	struct stat buf;
} File;

//frees all memory of Dllist d
void
free_dllist_complete(Dllist d)
{
	Dllist tmpDLL;
	File *f;

	dll_traverse(tmpDLL, d) 
	{
		f = (File *)tmpDLL->val.v;	
		
		free(f->full_path);
		free(f->rel_path);
		free(f);
	}
	
	free_dllist(d);
}

void
free_all(Dllist d1, Dllist d2)
{
	free_dllist_complete(d1);
	free_dllist_complete(d2);
}

//free File struct
void
free_file(File *f)
{
	free(f->full_path);
	free(f->rel_path);
	free(f);
}

//searches dllist for node with str as val.s and returns it. Else, NULL is returned. 
Dllist
dll_find_str(Dllist d, char *str)
{
	Dllist tmpDLL;
	dll_traverse(tmpDLL, d)
	{
		if ( strcmp(tmpDLL->val.s, str) == 0 ) return tmpDLL;
	}
	return NULL;
}

//copies string content from src and places it in dest ending in '/' character. The dest pointer is directed to the byte after '/' and returned. 
char *
cpy_str_slash (char *dest, char *src)
{
	strcpy(dest, src);
	dest += strlen(src);
	*dest = '/';
	++dest;

	return dest;
}

//appends '/' followed by "append_to_path" string to the end of "path"
char *
extend_path(char *path, char *append_to_path)
{
	char *new_path;
	
	//allocate memory for new_path
	new_path = malloc( sizeof(char) * (strlen(path) + strlen(append_to_path) + 2) );
	new_path = cpy_str_slash(new_path, path);
	new_path = strcpy(new_path, append_to_path);
	new_path -= (strlen(path) + 1); 

	return new_path;
}

//cycles through all files within full_path recursively and stores their File structs in corresponding Dllists
void
store_files(char *full_path, char *rel_path, Dllist directories, Dllist files)
{
	File *f; 
	DIR *d;
	struct dirent *de;
	int exists;
	Dllist tmpDLL, sub_directories;

	//process all files in "full path"  
	d = opendir(full_path);
	if (d == NULL)
	{
		fprintf(stderr, "opendir(%s): ", full_path);
		perror("");
		exit(1);
	}

	sub_directories = new_dllist();

	for (de = readdir(d); de != NULL; de = readdir(d))
	{
		if ( strcmp(de->d_name,".") != 0 && strcmp(de->d_name,"..") != 0 ) 	
		{
			//create File struct for new file
			f = (File *) malloc(sizeof(File));
			f->full_path = extend_path(full_path, de->d_name);
			f->rel_path = extend_path(rel_path, de->d_name);
			exists = lstat(f->full_path, &f->buf); 
			
			if (exists < 0) 
			{
				fprintf(stderr, "ERROR: %s file doesn't not exist!\n", f->full_path);
				free_file(f);
				exit(1);
			}
			
			//file is symbolic link --- ignore
			else if (S_ISLNK(f->buf.st_mode)) free_file(f); 

			//file is directory
			else if (S_ISDIR(f->buf.st_mode)) 
			{
				dll_append(directories, new_jval_v( (void *)f ));
				dll_append(sub_directories, new_jval_v( (void *)f ));
			}
			
			//file is file
			else dll_append(files, new_jval_v( (void *)f ));
		}
	}
	
	//close directory so we don't surpass 10 open file limit
	closedir(d);

	//recursive call for subdirectories
	dll_traverse(tmpDLL, sub_directories) 
	{
		f = (File *)tmpDLL->val.v;
		store_files(f->full_path, f->rel_path, directories, files);
	}

	//free local list and exit
	free_dllist(sub_directories);
}

//writes contents of File f to stdout
void
write_file_contents(File *f)
{
	FILE *file;
	char file_buffer[f->buf.st_size];
	
	file = fopen(f->full_path, "r");

	if (file == NULL) 
	{
		fprintf(stderr, "%s: ", f->full_path);
		perror("");
		exit(1);
	}

	else 
	{
		fread(file_buffer, 1, f->buf.st_size, file);
		fwrite(file_buffer, 1, f->buf.st_size, stdout); 
	}

	fclose(file);
}

//outputs to stdout the contents of the tarfile starting with directories and then files
void
output_tarfile(Dllist directories, Dllist files)
{
	JRB tmpJRB, inodes;
	Dllist tmpDLL;
	File *f;
	int rel_path_size, mode;
	unsigned long long int ulli_inode, mod_time, f_byte_size; 
	char *str_inode;

	inodes = make_jrb();

	//output diretories first
	dll_traverse(tmpDLL, directories)
	{
		//prepare all output common to directories
		f = (File *)tmpDLL->val.v;
		rel_path_size = strlen(f->rel_path);

		ulli_inode = f->buf.st_ino;
		str_inode = malloc(sizeof(ulli_inode) + 1);
		sprintf(str_inode, "%lld", ulli_inode);

		//directory inode not seen previously
		if ( jrb_find_str(inodes, str_inode) == NULL )
		{
			jrb_insert_str(inodes, str_inode, JNULL);
			
			//additional info for newly discovered inode
			mode = f->buf.st_mode;
			mod_time = f->buf.st_mtime;

			//write to stdout
			fwrite(&rel_path_size, 1, 4, stdout);
			fwrite(f->rel_path, 1, rel_path_size, stdout);
			fwrite(&ulli_inode, 1, 8, stdout);
			fwrite(&mode, 1, 4, stdout);
			fwrite(&mod_time, 1, 8, stdout);
		}
		
		//directory seen before --- nothing to add
		else 
		{
			//write to stdout
			fwrite(&rel_path_size, 1, 4, stdout);
			fwrite(f->rel_path, 1, rel_path_size, stdout);
			fwrite(&ulli_inode, 1, 8, stdout);
		}
	}

	//free and restart inode tree
	jrb_traverse(tmpJRB, inodes) free(tmpJRB->key.s);
	jrb_free_tree(inodes);
	inodes = make_jrb();

	//output non-directory files 
	dll_traverse(tmpDLL, files)
	{
		//prepare all output common to non-directory files
		f = (File *)tmpDLL->val.v;
		rel_path_size = strlen(f->rel_path);

		ulli_inode = f->buf.st_ino;
		str_inode = malloc(sizeof(ulli_inode) + 1);
		sprintf(str_inode, "%lld", ulli_inode);
		f_byte_size = f->buf.st_size;

		//non-directory inode not seen previously
		if ( jrb_find_str(inodes, str_inode) == NULL )
		{
			jrb_insert_str(inodes, str_inode, JNULL);
		
			//additional info for newly discovered inode
			mode = f->buf.st_mode;
			mod_time = f->buf.st_mtime;

			//write to stdout
			fwrite(&rel_path_size, 1, 4, stdout);
			fwrite(f->rel_path, 1, rel_path_size, stdout);
			fwrite(&ulli_inode, 1, 8, stdout);
			fwrite(&mode, 1, 4, stdout);
			fwrite(&mod_time, 1, 8, stdout);
			fwrite(&f_byte_size, 1, 8, stdout);
			write_file_contents(f);
		}
		
		//non-directory seen before --- nothing to add
		else 
		{
			//write to stdout
			fwrite(&rel_path_size, 1, 4, stdout);
			fwrite(f->rel_path, 1, rel_path_size, stdout);
			fwrite(&ulli_inode, 1, 8, stdout);
		}
	}

	//free inode tree
	jrb_traverse(tmpJRB, inodes) free(tmpJRB->key.s);
	jrb_free_tree(inodes);
}

int
main(int argc, char **argv)
{
	
	File *f; 
	char *rel_path;
	int exists;
	Dllist directories, files; 

	//malloc all dllists
	directories = new_dllist();
	files = new_dllist();
	
	if (argc < 2) 
	{
		fprintf(stderr, "No directory name (argv[1]) provided\n");
		exit(1);
	}

	//process command line directory (CLD)
	rel_path = strrchr(argv[1], '/');
	if (rel_path == NULL) rel_path = argv[1]; 
	else rel_path += 1; //move pointer past '/' to get only the starting directory	
	
	f = (File *) malloc(sizeof(File));
	f->rel_path = strdup(rel_path); 
	f->full_path = strdup(argv[1]);
	exists = lstat(argv[1], &f->buf); 
	if (exists < 0) 
	{
		fprintf(stderr, "ERROR: %s directory doesn't not exist!\n", argv[1]);
		exit(1);
	}
	else dll_append(directories, new_jval_v( (void *)f ));
	
	//process all sub files and directories of CLD recursively into appropriate Dllists			
	store_files(argv[1], rel_path, directories, files);

	//final output for tarfile
	output_tarfile(directories, files);

	//free leftover Dllist memory and exit
	free_all(directories, files);
	exit(0);	
}	
