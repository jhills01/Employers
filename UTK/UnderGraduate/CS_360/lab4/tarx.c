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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

//struct to hold file information
typedef struct
{
	char *f_name;
	char *f_contents;
	char *str_inode;
	unsigned long long int inode, m_time, f_byte_size;
	int mode, hard_link;
} File;

//completely frees given Dllist
void
free_File_dllist(Dllist l)
{
	Dllist tmpDLL;
	File *f;

	dll_traverse(tmpDLL, l)
	{
		f = (File *)tmpDLL->val.v;
		if ( strcmp(f->f_name, "DEFAULT --------- DEFAULT") != 0 ) free(f->f_name);
		if ( strcmp(f->f_contents, "DEFAULT --------- DEFAULT") != 0 ) free(f->f_contents);
		if ( strcmp(f->str_inode, "DEFAULT --------- DEFAULT") != 0 ) free(f->str_inode);
		free(f);
	}

	free_dllist(l);
}

//frees all memory
void
free_all(File *f, Dllist l, JRB tree, int free_all_three)
{
	if (free_all_three == 1)
	{
		jrb_free_tree(tree);
		free_File_dllist(l);
		free(f);
	}
	else
	{
		jrb_free_tree(tree);
		free_File_dllist(l);
	}
}

//File struct constructor
File *
File_constructor(File *f)
{
	f->f_name =      "DEFAULT --------- DEFAULT";
	f->f_contents =  "DEFAULT --------- DEFAULT";
	f->str_inode =   "DEFAULT --------- DEFAULT";
	f->inode = 0; 
	f->m_time = 0;
	f->f_byte_size = 0;
	f->mode = 0;
	f->hard_link = 0;
	return f;
}

//test print for Dllists holding File structs as values
void
dll_print_File(Dllist l)
{
	Dllist tmpDLL;
	File *f;

	dll_traverse(tmpDLL, l)
	{
		f = (File *)tmpDLL->val.v;
	
		printf("\nf->f_name:          %s\n", f->f_name);
		printf("f->f_contents:      %s\n", f->f_contents);
		printf("f->inode:           %llu\n", f->inode);
		printf("f->m_time(secs):    %llu\n", f->m_time);
		printf("f->f_byte_size:     %llu\n", f->f_byte_size);
		printf("f->mode:            %d\n", f->mode);
		printf("f->hard_link:       %d\n", f->hard_link);
	}
}

//re-sets correct mode for all tarc files in Dllist. Returns 1 if successful. 0 otherwise. 
int
set_mode(Dllist d)
{
	Dllist tmpDLL;
	File *f;
	int retval;

	dll_traverse(tmpDLL, d)
	{
		f = (File *)tmpDLL->val.v;
		
		if ( !f->hard_link )
		{
			retval = chmod(f->f_name, f->mode); 
			if (retval < 0)
			{
				fprintf(stderr, "chmod(%s, %d): ", f->f_name, f->mode);
				perror("");
				return 0;
			}
		}
	}

	return 1;
}

//re-sets correct mode for all tarc files in Dllist. Returns 1 if successful. 0 otherwise. 
int
set_mod_times(Dllist d)
{
	Dllist tmpDLL;
	struct timeval times[2];
	File *f;
	int retval;

	//initialize access and modification times of "times" 
	times[0].tv_sec = time(NULL);
	times[0].tv_usec = 0;
	times[1].tv_sec = time(NULL);
	times[1].tv_usec = 0;
	
	dll_traverse(tmpDLL, d)
	{
		f = (File *)tmpDLL->val.v;
		
		//if file is not a hard-link, re-create modification time 
		if ( !f->hard_link )
		{
			times[1].tv_sec = f->m_time;
			
			retval = utimes(f->f_name, times);
			if (retval < 0)
			{
				fprintf(stderr, "utimes(%s, times): ", f->f_name);
				perror("");
				return 0;
			}
		}
	}

	return 1;
}

//re-creates contents of tarc files except mode. Returns 1 if successful. 0 otherwise. 
int
recreate_files(File *f, JRB tree)
{
	int retval;
	FILE *file;
	JRB tmpJRB;
	File *non_hl_file;

	//re-create non-hard link directory. mode and modification time will be re-set later 
	if ( S_ISDIR(f->mode) && !f->hard_link )
	{
		retval = mkdir(f->f_name, 0777);
		if (retval < 0) 
		{
			fprintf(stderr, "mkdir(%s, 0777): ", f->f_name);
			perror("");
			return 0;
		}
	}

	//re-create non-hard link/non-directory file. mode and modification time will be re-set later
	else if ( !f->hard_link )
	{
		file = fopen(f->f_name, "w");
		if (file == NULL) 
		{
			fprintf(stderr, "fopen(%s, \"w\"): ", f->f_name);
			perror("");
			return 0;
		}
		
		//re-create file contents and close file
		fwrite(f->f_contents, 1, f->f_byte_size, file);
		fclose(file);
	}

	//file is a hard link
	//node in JRB tree will have original file name that File f needs to link to
	else
	{
		tmpJRB = jrb_find_str(tree, f->str_inode);
		if ( tmpJRB == NULL )
		{
			fprintf(stderr, "%s not found in JRB inodes when making hard link\n", f->str_inode);
			return 0;
		}
		else
		{
			non_hl_file = (File *)tmpJRB->val.v;			
			link(non_hl_file->f_name, f->f_name); 
		}
	}

	return 1;
}

void
extract_tarc()
{
	char *str_inode;
	int f_name_size, retval; 
	File *f;
	Dllist files;
	JRB tmpJRB, inodes;

	//initialize Dllist and JRB
	files = new_dllist();
	inodes = make_jrb();

	//read loop from stdin
	while(1) 
	{
		//allocate and construct File stuct
		f = malloc(sizeof(File));
		f = File_constructor(f);
		
		//get file name size but also check for EOF
		retval = fread(&f_name_size, 1, 4, stdin);
		if (feof(stdin)) 
		{
			free(f);
			break;
		}
		else if (retval != 4)
		{
			fprintf(stderr, "Return value from fread for mode doesn't match 4 (%d != 4)\n", retval);
			//free memory and exit
			free_all(f, files, inodes, 1);
			exit(1);
		}

		//create file name (i.e., relative file path)
		f->f_name = malloc(f_name_size + 1); //+ 1 for NULL
		retval = fread(f->f_name, 1, f_name_size, stdin);
		if (retval != f_name_size)
		{
			fprintf(stderr, "Return value from fread doesn't match f_name_size (%d != %d)\n", retval, f_name_size);
			//free memory and exit
			free(f->f_name);
			free_all(f, files, inodes, 1);
			exit(1);
		}
		f->f_name[f_name_size] = '\0';

		//create inode
		retval = fread(&f->inode, 1, 8, stdin);
		if (retval != 8)
		{
			fprintf(stderr, "Return value from fread for inode doesn't match 8 (%d != 8)\n", retval);
			//free memory and exit
			free(f->f_name);
			free_all(f, files, inodes, 1);
			exit(1);
		}
		str_inode = malloc(sizeof(f->inode) + 1);
		sprintf(str_inode, "%lld", f->inode);
		f->str_inode = str_inode;
		
		//inode NOT seen before
		tmpJRB = jrb_find_str(inodes, str_inode);
		if ( tmpJRB == NULL) 
		{
			//insert new inode in tree and read mode and modification time
			jrb_insert_str(inodes, str_inode, new_jval_v( (void *)f ));
			retval = fread(&f->mode, 1, 4, stdin);
			if (retval != 4)
			{
				fprintf(stderr, "Return value from fread for mode doesn't match 4 (%d != 4)\n", retval);
				//free memory and exit
				free(f->f_name);
				free(f->str_inode);
				free_all(f, files, inodes, 1);
				exit(1);
			}
			retval = fread(&f->m_time, 1, 8, stdin);
			if (retval != 8)
			{
				fprintf(stderr, "Return value from fread for mod time doesn't match 8 (%d != 8)\n", retval);
				//free memory and exit
				free(f->f_name);
				free(f->str_inode);
				free_all(f, files, inodes, 1);
				exit(1);
			}

			//file is NOT directory
			if ( !S_ISDIR(f->mode) ) 
			{
				//read file contents
				retval = fread(&f->f_byte_size, 1, 8, stdin);
				if (retval != 8)
				{
					fprintf(stderr, "Return value from fread for f_byte_size doesn't match 8 (%d != 8)\n", retval);
					//free memory and exit
					free(f->f_name);
					free(f->str_inode);
					free_all(f, files, inodes, 1);
					exit(1);
				}
				else if ( f->f_byte_size > 4294967296 ) 
				{
					fprintf(stderr, "f->f_byte_size > 4,294,967,296 bytes (virtual address limit for 32-bit process): %llu\n", f->f_byte_size);
					//free memory and exit
					free(f->f_name);
					free(f->str_inode);
					free_all(f, files, inodes, 1);
					exit(1);
				}

				f->f_contents = malloc(f->f_byte_size + 1);
				retval = fread(f->f_contents, 1, f->f_byte_size, stdin);
				if (retval != f->f_byte_size)
				{
					fprintf(stderr, "Return value from fread for contents doesn't match f_byte_size (%d != %d)\n", retval, f_name_size);
					//free memory and exit
					free(f->f_name);
					free(f->str_inode);
					free(f->f_contents);
					free_all(f, files, inodes, 1);
					exit(1);
				}
				f->f_contents[f->f_byte_size] = '\0';
			}
		}

		//inode seen before --- item is hard-link to file or directory
		else f->hard_link = 1;
		
		//store File in Dllist
		dll_append(files, new_jval_v( (void *)f ));

		//re-creates file names, hard links, and file contents 
		if ( !recreate_files(f, inodes) )
		{
			//free memory and exit
			free_all(f, files, inodes, 0);
			exit(1);
		}
	} //end of WHILE

	//re-set the mode and modification times of all files in Dllist
	//re-setting modification times must be done LAST (i.e., last condition in IF)
	if ( !set_mode(files) || !set_mod_times(files) )
	{
		//free memory and exit with error
		free_all(f, files, inodes, 0);
		exit(1);
	}
	
	else free_all(f, files, inodes, 0);
}

int
main(int argc, char **argv)
{
	//extract contents of given tarc file
	extract_tarc();

	exit(0);
}
