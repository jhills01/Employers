//NAME:Tom Hills  NETID: jhills

//CS 360: Lab3 - Fakemake

//DESCRIPTION:
//This lab requires the creation of a fakemake program which acts to simulate basic functionality of the "real"...
//...make program. Fakemake compiles using gcc and data supplied by .fm files. 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dllist.h"
#include "fields.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

//frees all memory of Dllist d
void
free_dllist_complete(Dllist d)
{
	Dllist tmpDLL;

	dll_traverse(tmpDLL, d) free(tmpDLL->val.s);
	free_dllist(d);
}

//frees all memory needed immediately before exiting program
void
free_all(Dllist headers, Dllist flags, Dllist sources, Dllist libs, Dllist objects, Dllist executable, IS is, DIR *d)
{
	free_dllist_complete(headers);
	free_dllist_complete(flags);
	free_dllist_complete(sources);
	free_dllist_complete(libs);
	free_dllist_complete(objects);
	free_dllist_complete(executable);
	
	jettison_inputstruct(is);

	closedir(d);
}

//searches through directory specified by dir and returns pointer to file whose name == str parameter.
//otherwise, it returns de == NULL 
struct dirent *
find_return_file(char *str, DIR *d)
{
	struct dirent *de;
	
	//resets directory pointer and loops through all files in current directory
	rewinddir(d);

	for (de = readdir(d); de != NULL; de = readdir(d)) 
	{
		if (strcmp(str, de->d_name) == 0) return de;
	}
		
	return de; //de == NULL at this point
}

// searches dllist for node with str as val.s and returns it. Else, NULL is returned. 
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

//checks each line specified by first character and processes it
void
line_spec_process(IS is, Dllist list, DIR *d)
{
	int i;
	struct dirent *de;

	//"executable" line specification 
	if ( strcmp(is->fields[0], "E") == 0 )
	{
		if (is->NF == 1)
		{
			fprintf(stderr, "ERROR: No executable listed after %s on line %d!\n", is->fields[0], is->line);
			exit(1);
		}
		
		else if (is->NF > 2)
		{
			fprintf(stderr, "ERROR: More than one executable listed after %s on line %d!\n", is->fields[0], is->line);
			exit(1);
		}
		
		else if ( !dll_empty(list) ) //returns true if empty
		{
			fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
			exit(1);
		}

		else dll_append(list, new_jval_s( strdup(is->fields[1]) )); //MALLOC called here!!!
	}

	//"source" line specification 
	else if ( strcmp(is->fields[0], "C") == 0 )
	{
		if (is->NF == 1)
		{
			fprintf(stderr, "ERROR: Nothing listed after %s on line %d!\n", is->fields[0], is->line);
			exit(1);
		}
	
		else 
		{
			for (i = 1; i < is->NF; ++i) dll_append(list, new_jval_s( strdup(is->fields[i]) )); //MALLOC called here!!!
		}
	}
	
	//"header" line specification 
	else if ( strcmp(is->fields[0], "H") == 0 )
	{
		if (is->NF == 1)
		{
			fprintf(stderr, "ERROR: Nothing listed after %s on line %d!\n", is->fields[0], is->line);
			exit(1);
		}
	
		else 
		{
			for (i = 1; i < is->NF; ++i)
			{
				de = find_return_file(is->fields[i], d);
				
				if (de == NULL) 
				{
					fprintf(stderr, "ERROR: header file %s on line %d was not found in directory!\n", is->fields[i], is->line);
					exit(1);
				}

				else dll_append(list, new_jval_s( strdup(is->fields[i]) )); //MALLOC called here!!!
			}
		}
	}

	//E and L lines are appended here
	else 
	{
		for (i = 1; i < is->NF; ++i) dll_append(list, new_jval_s( strdup(is->fields[i]) )); //MALLOC called here!!!
	}
}

//converts the string.c to string.o
char * 
c_to_o(char *c_file)
{
	char *o_file;
	int i;

	//allocated # of bytes == characters in c_file + NULL termaintor
	o_file = malloc(sizeof(char) * (strlen(c_file) + 1));
	
	//copy all characters from c_file to o_file including NULL terminator
	for (i = 0; i <= strlen(o_file); ++i) o_file[i] = c_file[i];

	//replace 'c' right before the null terminator with 'o'
	o_file[strlen(o_file) - 1] = 'o';

	return o_file;
}


//copies string content from src and places it in dest ending in ' ' character. The dest pointer is directed to the byte after ' ' and returned. 
char *
cpy_str_space (char *dest, char *src)
{
	strcpy(dest, src);
	dest += strlen(src);
	*dest = ' ';//NOTE: this line I'm not 100% sure 
	++dest;

	return dest;
}

//copies string content from src and places it in dest ending in '\0' character. The dest pointer is directed to the byte with '\0' and returned. 
char *
cpy_str_null (char *dest, char *src)
{
	strcpy(dest, src);
	dest += strlen(src);
	
	return dest;
}

void
initialize_string(char * c, int size)
{
	int i; 
	for (i = 0; i < size; ++i) 
	{
		if (i != size - 1) c[i] = 'Z';
		else c[i] = '\0'; 
	}
}

//builds gcc compile strings 
//if c_file_name == NULL, it compiles into executable
char *
build_compile_string(char * c_file_name, Dllist flags, Dllist libs, Dllist objects, Dllist executable)
{
	char *compile, *gcc, *f;
	int byte_size;
	Dllist tmpDLL;

	gcc = "gcc";
	
	//calculate memory common to both modes	
	byte_size = 7; //"gcc " + "-c " (or "-o ")
	dll_traverse(tmpDLL, flags) byte_size += strlen(tmpDLL->val.s) + 1;
	
	//build compile string from source/flags to object files
	if (c_file_name != NULL)
	{
		//determine byte size of compile string
		byte_size += strlen(c_file_name) + 1;
		
		//allocate compile string
		compile = malloc(byte_size);
		initialize_string(compile, byte_size);
		
		f = "-c"; 
		compile = cpy_str_space(compile, gcc);
		compile = cpy_str_space(compile, f);
		dll_traverse(tmpDLL, flags) compile = cpy_str_space(compile, tmpDLL->val.s);
		compile = cpy_str_null(compile, c_file_name); 
	}

	//build compile string from objects/libs to executable file
	else 
	{
		//calculate remaining memory needed for compiling from .o to executable
		dll_traverse(tmpDLL, libs) byte_size += strlen(tmpDLL->val.s) + 1;
		dll_traverse(tmpDLL, objects) byte_size += strlen(tmpDLL->val.s) + 1;
		byte_size += strlen(executable->flink->val.s) + 1; 
		
		//allocate compile string
		compile = malloc(byte_size);
		initialize_string(compile, byte_size);
		
		f = "-o"; 
		compile = cpy_str_space(compile, gcc);
		compile = cpy_str_space(compile, f);
		compile = cpy_str_space(compile, executable->flink->val.s);
		dll_traverse(tmpDLL, flags) compile = cpy_str_space(compile, tmpDLL->val.s);
	
		//NULL terminate final .o file string
		if ( dll_empty(libs) )
		{
			dll_traverse(tmpDLL, objects) 
			{
				if (tmpDLL != objects->blink) compile = cpy_str_space(compile, tmpDLL->val.s);
				else compile = cpy_str_null(compile, tmpDLL->val.s);
			}
		}

		//NULL terminate final lib string
		else
		{
			dll_traverse(tmpDLL, objects) compile = cpy_str_space(compile, tmpDLL->val.s);

			dll_traverse(tmpDLL, libs) 
			{
				if (tmpDLL != libs->blink) compile = cpy_str_space(compile, tmpDLL->val.s);
				else compile = cpy_str_null(compile, tmpDLL->val.s);
			}
		}
	}

	//reset compiler pointer to beginning of array
	compile -= byte_size - 1;

	return compile;
}

int
main(int argc, char **argv)
{
	IS is;
	struct stat buf;
    int exists, c_compiled, sys_check;
	DIR *d;
	struct dirent *de;
	char *f_name, *compile;
	time_t executable_t, maximum_head, maximum_object, source_t, object_t;

	Dllist tmpDLL, headers, flags, sources, libs, objects, executable; 

	//malloc all JRB trees
	headers = new_dllist();
	flags = new_dllist();
	sources = new_dllist();
	libs = new_dllist();
	objects = new_dllist();
	executable = new_dllist();

	//intialize other variables
	executable_t = 0;
	maximum_head = 0;
	maximum_object = 0;
	source_t = 0;
	object_t = 0;
	c_compiled = 0;

	//open file. If neither exist, throw error. 
	if (argv[1] == NULL) is = new_inputstruct("fmakefile");
	
	else is = new_inputstruct(argv[1]);
	
	if (is == NULL) 
	{
		perror(argv[1]); 
		exit(1); 
	}
	
	//open directory stream
	d = opendir("."); 
	
	if (d == NULL) 
	{
		fprintf(stderr, "Couldn't open \".\"\n");
		free_all(headers, flags, sources, libs, objects, executable, is, d);
		exit(1);
	}

	//start pulling in lines from file stored in is		
	while (get_line(is) >= 0) 
	{
		//execute if NOT blank line	
		if (is->NF > 0)
		{
			if (strcmp(is->fields[0], "E") == 0 ) line_spec_process(is, executable, d);
			else if (strcmp(is->fields[0], "H") == 0 ) line_spec_process(is, headers, d);
			else if (strcmp(is->fields[0], "F") == 0 ) line_spec_process(is, flags, d);
			else if (strcmp(is->fields[0], "C") == 0 ) line_spec_process(is, sources, d);
			else if (strcmp(is->fields[0], "L") == 0 ) line_spec_process(is, libs, d);
			
			//exit with error if a line was present in fakemake that didn't have a specification character
			else 
			{
				fprintf(stderr, "ERROR: line %d of %s file did NOT have specifier!\n", is->line, argv[1]);
				free_all(headers, flags, sources, libs, objects, executable, is, d);
				exit(1);
			}
		}
	}

	//if no executable was specified, exit with error. 
	if ( dll_empty(executable) ) 
	{
		fprintf(stderr, "No executable specified\n");
		free_all(headers, flags, sources, libs, objects, executable, is, d);
		exit(1);
	}

	//find maximum time from H files
	dll_traverse(tmpDLL, headers)
	{
		exists = stat( tmpDLL->val.s, &buf);
		
		if (exists < 0) fprintf(stderr, "Couldn't stat %s\n", tmpDLL->val.s);
		else if (buf.st_mtime > maximum_head) maximum_head = buf.st_mtime; 
	}

	//process c files and check if compilation is needed
	dll_traverse(tmpDLL, sources)
	{
		exists = stat(tmpDLL->val.s, &buf);
		source_t = buf.st_mtime;

		if (exists < 0) 
		{
			fprintf(stderr, "fmakefile: %s: ", tmpDLL->val.s);
			perror("");
			free_all(headers, flags, sources, libs, objects, executable, is, d);
			exit(1);
		}
		
		//look for corresponding .o file
		else 
		{
			f_name = c_to_o(tmpDLL->val.s); //converts name.c to name.o
			exists = stat(f_name, &buf);
			free(f_name); //no longer needed
			object_t = buf.st_mtime; 

			// .o file doesn't exist or is less recent than either source or any of the header files, we need to compile
			// having "exists < 0" be the first compare ensures that the following two compares are well defined by existing file!
			if (exists < 0 || object_t < source_t || object_t < maximum_head)
			{
				compile = build_compile_string(tmpDLL->val.s, flags, libs, objects, executable);//malloc called here
				printf("%s\n", compile);
				
				sys_check = system(compile);
				if (sys_check != 0) 
				{
					fprintf(stderr, "Command failed.  Exiting\n");
					free(compile);
					free_all(headers, flags, sources, libs, objects, executable, is, d);
					exit(1);
				}

				c_compiled = 1;

				//free compile string before moving on
				free(compile);
			}

			//find maximum time from .o files
			else if (object_t > maximum_object) maximum_object = object_t;  
		}
	}

	//look for existing object files. If found and not already in objects list, insert them
	dll_traverse(tmpDLL, sources)
	{
		f_name = c_to_o(tmpDLL->val.s); //malloc used here
		de = find_return_file(f_name, d);
		if ( de != NULL ) dll_append(objects, new_jval_s(f_name) );
		else free(f_name);
	}

	//process o files and check if compilation is needed
	exists = stat(executable->flink->val.s, &buf); 
	executable_t = buf.st_mtime;
	
	//if executable doesn't exist OR any .o file is more recent OR any .c files have been recompiled, then remake executable 
	if (exists < 0 || maximum_object > executable_t || c_compiled)
	{
		compile = build_compile_string(NULL, flags, libs, objects, executable);//malloc called here	
		
		printf("%s\n", compile);
		
		sys_check = system(compile);

		if (sys_check != 0) 
		{
			fprintf(stderr, "Command failed.  Fakemake exiting\n");
			free(compile);
			free_all(headers, flags, sources, libs, objects, executable, is, d);
			exit(1);
		}
		//free compile string before moving on
		free(compile);
	}

	else
	{
		printf("%s up to date\n",executable->flink->val.s);
		free_all(headers, flags, sources, libs, objects, executable, is, d);
		exit(1);
	}

	//clean up memory and exit
	free_all(headers, flags, sources, libs, objects, executable, is, d);
	exit(0);
}
