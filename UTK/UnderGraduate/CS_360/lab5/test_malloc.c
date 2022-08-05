// NAME: Tom Hills
// NETID: jhills
//
// CS 360: Lab5 - Jmalloc
//
// DESCRIPTION:
// This program provides the grounds for testing jmalloc programs before porting to header files 

#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "dllist.h"


//global heap buffer variables
//NOTE: extra credit if using ONLY ONE GLOBAL
char *malloc_head = NULL;

typedef struct flist 
{
	unsigned int size;
	struct flist *next;
	struct flist *prev;
} *Flist;

void
print_flist(char *malloc_head)
{
	Flist f;
	//void *p;
	//unsigned long long int diff;

	f = (Flist) malloc_head;

	while(1)
	{
//		diff = ((unsigned long long int)&f->next - (unsigned long long int)&f->size);
//		printf("\n&f->next - &f->size:  %llu bytes\n", diff);
//		diff = ((unsigned long long int)&f->prev - (unsigned long long int)&f->next);
//		printf("&f->prev - &f->next:  %llu bytes\n", diff);
//		p = ((char *)&f->prev + sizeof(f->prev) );
//		diff = ((unsigned long long int)p - (unsigned long long int)&f->prev);
//		printf("p - &f->prev:         %llu bytes\n", diff);

		//printf("f:                    %p\n", f);
		printf("\nf->size:              %u\n", f->size);
		printf("&f->size:             %p\n", &f->size);
	
		if (f->next == NULL && f->prev == NULL)
		{
			printf("&f->next:             %p\n", &f->next); 
			printf("&f->prev:             %p\n", &f->prev);
			printf("end of f->prev:	      %p\n", ((char *)&f->prev + sizeof(f->prev) - 1) );
//			printf("byte after f->prev:   %u\n", ((char *)&f->prev + sizeof(f->prev)) );
			printf("f->next:              NULL\n"); //END OF LIST
			printf("f->prev:              NULL\n\n");
			return; //would "break" be better here?
		}	
		
		else if (f->next == NULL && f->prev != NULL)
		{
			printf("f->next:  NULL\n"); //END OF LIST
			printf("f->prev:  %p\n", f->prev);
			return;
		}
		
		else if (f->next != NULL && f->prev == NULL)
		{
			printf("f->next:  %p\n", f->next);
			printf("f->prev:  NULL\n");
			f = f->next;
		}
		
		else 
		{
			printf("f->next:  %p\n", f->next); 
			printf("f->prev:  %p\n", f->prev);
			f = f->next;
		}
	}
}

void
print_um(void * user_mem, Flist f, unsigned int i)
{
	char *a_start;
	unsigned int size;
	a_start = ((char*)user_mem - sizeof(*f));
	size = *((unsigned int *)a_start);
	
	printf("\nStart of Allocation: %p\n", a_start);
	printf("End of Allocation:   %p\n", (a_start + i - 1) );
	printf("Size of allocation:  %u\n", size);
}

void
print_node(Flist f)
{
	printf("\nf->size:              %u\n", f->size);
	printf("&f->size:             %p\n", &f->size);
	
	if (f->next == NULL && f->prev == NULL)
	{
		printf("&f->next:             %p\n", &f->next); 
		printf("&f->prev:             %p\n", &f->prev);
		printf("end of f->prev:	      %p\n", ((char *)&f->prev + sizeof(f->prev) - 1) );
//			printf("byte after f->prev:   %u\n", ((char *)&f->prev + sizeof(f->prev)) );
		printf("f->next:              NULL\n"); //END OF LIST
		printf("f->prev:              NULL\n\n");
		return; //would "break" be better here?
	}	
	
	else if (f->next == NULL && f->prev != NULL)
	{
		printf("f->next:  NULL\n"); //END OF LIST
		printf("f->prev:  %p\n", f->prev);
		return;
	}
	
	else if (f->next != NULL && f->prev == NULL)
	{
		printf("f->next:  %p\n", f->next);
		printf("f->prev:  NULL\n");
		return;
	}
	
	else 
	{
		printf("f->next:  %p\n", f->next); 
		printf("f->prev:  %p\n", f->prev);
		return;
	}
}

//resets malloc_head to the beginning of the list
void
reset_head()
{
	Flist f;

	f = (Flist) malloc_head;
	while (f->prev != NULL) f = f->prev;
	malloc_head = (char *) f;
}

//TAs are grading based on 64 bit!! Pointers are 8 bytes!
void *
jmalloc2(unsigned int size)
{
	Flist f, f_tmp; 
	void *user_mem; 
	//char *tmp;
	unsigned int a_size, f_size;
	int diff, buff_size;
	size_t bk_size;

	//check if requested size == 0
	if (size == 0) return NULL;

	//set malloc's buffer and bookkeeping size
	buff_size = 8192;
	bk_size = sizeof(*f);

	//calculate allocation size (a_size)
	//if requested size (size) is not a multiple of 8, upgrade to the next highest multiple of 8
	//using "sizeof(*f)" for portability
	if (size % 8 != 0) 
	{
		size = ((size / 8) + 1) * 8;
		a_size = size + bk_size;
	}
	else a_size = size + bk_size;

	//FIRST CALL TO MALLOC
	if (malloc_head == NULL)
	{
		//requested memory size less than one page --- free list empty
		if (a_size < buff_size)
		{
			//process user memory
			user_mem = sbrk(buff_size);
			if (user_mem == (void *) -1)
			{
				fprintf(stderr, "malloc_begin was NULL - sbrk(%d) failed: ", buff_size);	
				perror("");
				exit(0);
			}

			*((unsigned int *)user_mem) = a_size; 			
			user_mem += bk_size;

print_um(user_mem, f, a_size);

			//NOTE: make this a "insert_node" function. Remember to distinguish between "malloc_head == NULL or not" cases
			//add first node to flist
			malloc_head = (char *)user_mem + size;//NOTE: using user_mem as pointer ref b/c malloc_head is NULL at this point
			f = (Flist) malloc_head;
			f->size	= buff_size - a_size; 
			f->next = NULL; 
			f->prev = NULL;

print_flist(malloc_head);
			
			return user_mem;;
		}
		
		//requested memory size greater than/equal to one page --- free list empty
		//malloc_head not used. Remains NULL.
		else 
		{
			//process user memory
			user_mem = sbrk(a_size);

printf("user_mem (free list empty):       %p\n", user_mem);

			if (user_mem == (void *) -1)
			{
				fprintf(stderr, "a_size >= buff_size when free list was empty - sbrk(%u) failed: ", a_size);	
				perror("");
				exit(0);
			}

			*((unsigned int *)user_mem) = a_size; 			
			user_mem += bk_size;

print_um(user_mem, f, a_size);		

			return user_mem;
		}
	}

	//SUBSEQUENT CALLS TO MALLOC
	else
	{
		//requested memory size less than one page --- free list NOT empty
		if (a_size < buff_size)
		{
			//traverse through list of free memory until proper allocation can be returned to user 
			while (1) 
			{	
				// obtain difference between size of 1st node in free list to requested a_size
				f_size = *( (unsigned int *)malloc_head );
				diff = f_size - a_size;

				if (diff >= 32 ) 
				{
					//NOTE: resize_node()

printf("\nif (diff >= 32)\n");

					//process user memory
					user_mem = malloc_head;
					f_tmp = (Flist) user_mem;
					*((unsigned int *)user_mem) = a_size; 		
					user_mem += bk_size;

print_um(user_mem, f, a_size);		

					//create bookkeeping bytes of new free block
					//NOTE: it's still the same node, except the size has changed. that's it! Don't have to check next/prev for NULL or anything
					malloc_head += a_size; 
					f = (Flist) malloc_head;
					f->size = diff;
					f->next = f_tmp->next;
					f->prev = f_tmp->prev;
					
					//adjust previous node's next pointer, if existing, to point to leftover free memory
					if (f_tmp->prev != NULL) f_tmp->prev->next = f;
					
					//NOTE: not sure why, but this caused segfault. But above method seems to work!
					//f->next = *(&f->next - a_size);
					//f->prev = *(&f->prev - a_size);

print_node(f);

					reset_head();
					return user_mem; 
				}

				//upgrade a_size to f_size. 
				//There wouldn't be enough memory left to create the minimum required size of a free node (bookkeeping + 8)
				else if (diff < 32 && diff >= 0) 
				{

printf("\nelse if (diff < 32 && diff >= 0)\n");

					//process user memory
					user_mem = malloc_head;
					user_mem += bk_size;

print_um(user_mem, f, a_size);		

					//adjust free list
					f = (Flist) malloc_head;
					if (f->prev == NULL)  
					{	
						f->next->prev = NULL; //removing first node from free list
						malloc_head = (char *) f->next; 
					}
					else if (f->next == NULL) f->prev->next = NULL; //removing last node from free list
					else
					{
						f->prev->next = f->next; //set the next pointer of f's previous node to f's next node
						f->next->prev = f->prev; //set the prev pointer of f's next node to f's previous node
					}
					
print_node(f);

					reset_head();
					return user_mem;
				}

				//requested a_size is larger than available f_size. Move to next node (if present) and try again
				else if (diff < 0 && f->next != NULL) 
				{

printf("\nelse if (diff < 0 && f->next != NULL)\n");

					f = (Flist) malloc_head;
					malloc_head = (char *)f->next;//NOTE: BUG POTENTIAL. This makes me worry if address is large enough such that casting alters address

print_node(f);
//print_flist(malloc_head);

				}

				//Reached the end of the free list without finding free node of adequate size. 
				//Create new page and attach left over memory to end of free list
//				else if (diff < 0 && f->next == NULL) //NOTE: shouldn't need this, but leaving it as a reminder just in case
				else
				{

printf("\nelse (end of tree)\n");

					user_mem = sbrk(buff_size);
					if (user_mem == (void *) -1)
					{
						fprintf(stderr, "(a_size < %u) appending node to end of free list - sbrk(%u) failed: ", buff_size, buff_size);	
						perror("");
						exit(0);
					}
					
					f_tmp = (Flist) user_mem;
					f = (Flist) malloc_head; //malloc_head points to last node of free list

					f_tmp->size = a_size;
					f_tmp->next = NULL;
					f_tmp->prev = (Flist) malloc_head;
					f->next = f_tmp;

					reset_head();
					user_mem += bk_size;

print_um(user_mem, f, a_size);		
print_node(f);

					return user_mem;
				}
			}
		}

		//requested memory size greater than/equal to one page --- free list NOT empty
		else 
		{
			//NOTE: code logic is the same as above when malloc was first called. NO altering of the flist
			
			//process user memory
			user_mem = sbrk(a_size);

printf("user_mem (free list NOT empty):       %p\n", user_mem);

			if (user_mem == (void *) -1)
			{
				fprintf(stderr, "a_size >= buff_size when free list not empty - sbrk(%u) failed: ", a_size);	
				perror("");
				exit(0);
			}

			*((unsigned int *)user_mem) = a_size; 			
			user_mem += bk_size;

print_um(user_mem, f, a_size);		

			return user_mem;
		}
	}
	
	//malloc failed
	return NULL;
}

//adds memory block pointed to by ptr to the free list as the first node
void 
jfree2(void *ptr)
{
	Flist f, f1;

	ptr -= sizeof(*f);

//printf("*( (unsigned int *)ptr ): %u\n", *( (unsigned int *)ptr ));	
printf("ptr(in jfree2): %p\n", ptr );	
	
	f = (Flist) malloc_head;
	f1 = (Flist) ptr;

	if (f != NULL)
	{
		f->prev = f1;
		f1->next = f;
		f1->prev = NULL; 
	}
		
	//lastly, set malloc_head to the new 1st node
	malloc_head = (char *) f1;

//print_flist(malloc_head);
}


//void *
//jcalloc(unsigned int nmemb, unsigned int size)
//{
//    char *arr[nmemb];
//    int i;
//
//    if (nmemb == 0 || size == 0) return NULL;
//
//	
//
////    for (i = 0; i < nmemb; i++)
////    {
////        arr[i] = (char *) jmalloc2(size);
////        bzero(arr[i], size);
////
////printf("arr[%d]: %p     string value: %s\n", i, arr[i], arr[i]);
////    }
//
//    return arr[0];
//}

//NOTE: Realloc frees before returns
int
main()
{
	unsigned int i_input, i;//, i_input2;
	char s_input[100];
	void * p;
	Dllist alloc, tmpDLL, tmpDLL2;

	alloc = new_dllist();

	while (1)
	{
		//allocation
alloc_restart:
		for (i = 0; i < 100; i++) s_input[i] = '0'; //initialize s_input to accept new user input
		i = 0;
		printf("\n------------------------------------------------------------------------------------------\n");
		printf("Input number of bytes to allocate(\"exit\", \"skip\", \"flist\", \"alloc\"):\n");
		fscanf(stdin, "%s", s_input);
		if (strcmp(s_input, "exit") == 0) break; 
		if (strcmp(s_input, "skip") == 0) goto free_restart; 
		if (strcmp(s_input, "alloc") == 0) 
		{
			dll_traverse(tmpDLL, alloc) 
			{
				printf("index: %d     tmpDLL->val.v: %p     &f->size: %p     f->size: %u\n", i, tmpDLL->val.v, (tmpDLL->val.v - sizeof(struct flist)), *((unsigned int *)(tmpDLL->val.v - sizeof(struct flist))));
				i++;
			}
			i = 0;
			goto alloc_restart;
		}
		if (strcmp(s_input, "flist") == 0)
		{
			print_flist(malloc_head);
			continue;
		}
		i_input = atoi(s_input);
		printf("i_input: %u\n", i_input);
		p = jmalloc2(i_input);	
		printf("\naddress returned to user: %p\n", p);
		if (p != NULL) dll_append(alloc, new_jval_v(p));
		goto alloc_restart;

		//deallocation
free_restart:
		for (i = 0; i < 100; i++) s_input[i] = '0'; //initialize s_input to accept new user input
		i = 0;
		printf("\n---------------------------------------------------------------------------------------\n");
		printf("Provide index to allocated block you wish to free(\"exit\", \"skip\", \"flist\", \"alloc\"):\n");
		fscanf(stdin, "%s", s_input);
		if (strcmp(s_input, "exit") == 0) break; 
		if (strcmp(s_input, "skip") == 0) continue; 
		if (strcmp(s_input, "alloc") == 0) 
		{
			dll_traverse(tmpDLL, alloc) 
			{
				//printf("index: %d     tmpDLL->val.v: %p     &f->size: %p\n", i, tmpDLL->val.v, (tmpDLL->val.v - sizeof(struct flist)));
				printf("index: %d     tmpDLL->val.v: %p     &f->size: %p     f->size: %u\n", i, tmpDLL->val.v, (tmpDLL->val.v - sizeof(struct flist)), *((unsigned int *)(tmpDLL->val.v - sizeof(struct flist))));
				i++;
			}
			i = 0;
			goto free_restart;
		}
		if (strcmp(s_input, "flist") == 0)
		{
			print_flist(malloc_head);
			goto free_restart;
		}
		i_input = atoi(s_input);
		printf("i_input: %u\n", i_input);
		dll_traverse(tmpDLL, alloc) 
		{
			if (i_input == i)
			{
				jfree2(tmpDLL->val.v);
				print_flist(malloc_head);
				tmpDLL2 = tmpDLL->flink;
				dll_delete_node(tmpDLL);
				tmpDLL = tmpDLL2;
			}
			i++;
		}
		i = 0;
		goto free_restart;
	}
	
//	printf("\n\njcalloc test:\n");
//	printf("Enter nmemb:\n");
//	fscanf(stdin, "%s", s_input);
//	i_input = atoi(s_input);
//	printf("Enter nmemb:\n");
//	fscanf(stdin, "%s", s_input);
//	i_input2 = atoi(s_input);
//
//	p = jcalloc(i_input, i_input2);
//	printf("returned %p", p);
	
	
	exit(0); 
}
