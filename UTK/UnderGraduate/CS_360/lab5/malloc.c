// NAME:Tom Hills  
// NETID: jhills

// CS 360: Lab5 - Jmalloc

// DESCRIPTION:
// This program mimics the functionality of of the std lib call malloc, calloc, realloc, and free.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//global variables
char *list_head = NULL;

//struct for free memory list
typedef struct flist
{
    unsigned int size;
    struct flist *next;
    struct flist *prev;
} *Flist;


/*
void
print_flist(char *malloc_head)
{
    Flist f;

    f = (Flist) malloc_head;

    while(1)
    {
        printf("\nf->size:              %u\n", f->size);
        printf("&f->size:             %p\n", &f->size);

        if (f->next == NULL && f->prev == NULL)
        {
            printf("&f->next:             %p\n", &f->next);
            printf("&f->prev:             %p\n", &f->prev);
            printf("end of f->prev:       %p\n", ((char *)&f->prev + sizeof(f->prev) - 1) );
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

//resets malloc_head to the beginning of free list
void
reset_head()
{
    Flist f;

printf("\nreset_head\n");
    
	f = (Flist) malloc_head;

printf("     malloc_head: %p\n", malloc_head);

    //while (f->prev != NULL) f = f->prev;
    
	while (f->prev != NULL) 
	{
printf("               f: %p     f->prev: %p\n", f, f->prev);
if (f->prev == NULL) printf("F->PREV NULL INSIDE WHILE\n");
		f = f->prev;
	}
    
	malloc_head = (char *) f;

printf("     malloc_head: %p\n", malloc_head);
}
*/

void
print_node(Flist f, void *p)
{
	printf("sbrk(0): %p\n", p);
	printf("f: %-15p", f);
	if (f->next == NULL && f->prev == NULL) 
	{
		printf("f->next: NULL           f->prev: NULL           f->size: %-15u&f->size: %-15p\n", f->size, &f->size);
		return;
	}
	else if (f->next == NULL && f->prev != NULL)
	{
		printf("f->next: NULL           f->prev: %-15pf->size: %-15u&f->size: %-15p\n", f->prev, f->size, &f->size);
		return;
	}
	else if (f->next != NULL && f->prev == NULL)
	{
		printf("f->next: %-15pf->prev: NULL           f->size: %-15u&f->size: %-15p\n", f->next, f->size, &f->size);
		if (f != f->next) f = f->next;
		else 
		{
			printf("YIKES!!! f == f->next. Exiting...\n");
			exit(1);
		}
	}
	else
	{
		printf("f->next: %-15pf->prev: %-15pf->size: %-15u&f->size: %-15p\n", f->next, f->prev, f->size, &f->size);
		if (f != f->next) f = f->next;
		else 
		{
			printf("YIKES!!! f == f->next. Exiting...\n");
			exit(1);
		}
	}
	printf("f - sbrk(0): %d", (f - (Flist)p));
}

void
print_flist()
{
    Flist f;

    f = (Flist) list_head;

    while(1)
    {
        printf("f: %-15p", f);
		if (f->next == NULL && f->prev == NULL) 
		{
			printf("f->next: NULL           f->prev: NULL           f->size: %-15u&f->size: %-15p\n", f->size, &f->size);
			return;
		}
		else if (f->next == NULL && f->prev != NULL)
		{
			printf("f->next: NULL           f->prev: %-15pf->size: %-15u&f->size: %-15p\n", f->prev, f->size, &f->size);
			return;
		}
		else if (f->next != NULL && f->prev == NULL)
		{
			printf("f->next: %-15pf->prev: NULL           f->size: %-15u&f->size: %-15p\n", f->next, f->size, &f->size);
			if (f != f->next) f = f->next;
			else 
			{
				printf("YIKES!!! f == f->next. Exiting...\n");
				exit(1);
			}
		}
		else
		{
			printf("f->next: %-15pf->prev: %-15pf->size: %-15u&f->size: %-15p\n", f->next, f->prev, f->size, &f->size);
			if (f != f->next) f = f->next;
			else 
			{
				printf("YIKES!!! f == f->next. Exiting...\n");
				exit(1);
			}
		}
    }
}

void 
jfree(void *ptr)
{
	Flist f, f1;

	if (ptr == NULL) return; //NOTE: Alex told me to add this line for fakemake

    ptr -= sizeof(*f);
    //ptr -= 8;

    f = (Flist) list_head;
    f1 = (Flist) ptr;

	//check for empty list 
	if (f != NULL)
	{
		f->prev = f1;
		f1->next = f;
		f1->prev = NULL;
	}
    
	//reset list_head to the new 1st node
    list_head = (char *) f1;
}

void *
jmalloc(unsigned int size)
{
	Flist f, f_tmp;
    void *user_mem;
    char *malloc_head;
    unsigned int a_size, f_size;
    int diff, buff_size;
    size_t bk_size;

    //check if requested size == 0
    if (size == 0) return NULL;

    //initialize local variables
    buff_size = 8192;
    bk_size = sizeof(*f);
	malloc_head = list_head; 

//printf("size(b4): %u\n", size); //test
    //calculate allocation size (a_size)
    //if requested size (size) is not a multiple of 8, upgrade to the next highest multiple of 8
    if (size % 8 != 0)
    {
        size = ((size / 8) + 1) * 8;
        a_size = size + bk_size;
    }
    else a_size = size + bk_size;

//printf("a_size: %u     size(after): %u     bk_size: %u\n", a_size, size, bk_size); //test
	
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
                exit(1);
            }

            *((unsigned int *)user_mem) = a_size;
            user_mem += bk_size;

            //NOTE: make this a "insert_node" function. Remember to distinguish between "malloc_head == NULL or not" cases
            //add first node to flist
            malloc_head = (char *)user_mem + size;//NOTE: using user_mem as pointer ref b/c malloc_head is NULL at this point
            f = (Flist) malloc_head;
            f->size = buff_size - a_size;
            f->next = NULL;
            f->prev = NULL;
            list_head = malloc_head;

            return user_mem;;
        }

        //requested memory size greater than/equal to one page --- free list empty
        //list_head not used. Remains NULL.
        else
        {
            //process user memory
            user_mem = sbrk(a_size);

            if (user_mem == (void *) -1)
            {
                fprintf(stderr, "a_size >= buff_size when free list was empty - sbrk(%u) failed: ", a_size);
                perror("");
                exit(1);
            }

            *((unsigned int *)user_mem) = a_size;
            user_mem += bk_size;

            return user_mem;
        }
    }
	
   //SUBSEQUENT CALLS TO MALLOC --- MALLOC_HEAD != NULL
    else
    {
        //requested memory size less than one page --- free list NOT empty
        if (a_size < buff_size)
        {
			//traverse through list of free memory until proper allocation can be returned to user
            while (1)
            {
				// obtain difference between size of node in free list to requested a_size
                f_size = *( (unsigned int *)malloc_head );
                diff = f_size - a_size;

                if (diff >= 32 )
                {
//printf("[diff >= 32]\n"); //test
//printf("malloc_head: %p\n", malloc_head); //test
                    //process user memory
                    user_mem = malloc_head;
                    f_tmp = (Flist) user_mem;
                    f_tmp->size = a_size;
                    user_mem += bk_size;
                    
					//create bookkeeping bytes of new free block
                    malloc_head += a_size;
                    f = (Flist) malloc_head;
                    f->size = diff;
                    f->next = f_tmp->next;
                    f->prev = f_tmp->prev;

                    //adjust surrounding nodes' pointers, if existing, to point to leftover free memory
					if (f->next != NULL) f->next->prev = f; 
                    if (f->prev != NULL) f->prev->next = f;
					
					//reset list_head if 1st free node was modified
					if (f->prev == NULL) list_head = (char *) f;

                    return user_mem;
                }

                //upgrade a_size to f_size.
                //There wouldn't be enough memory left to create the minimum required size of a free node (bookkeeping + 8)
                else if (diff < 32 && diff >= 0)
                {
//printf("[0 <= diff < 32]\n");//test
//printf("malloc_head: %p\n", malloc_head); //test
					//process user memory
                    user_mem = malloc_head;
                    user_mem += bk_size;

                    //adjust free list
                    f = (Flist) malloc_head;

//printf("[0 <= diff < 32] f: %p     f_size: %u      a_size: %u     diff: %d     f->next: %p     f->prev: %p\n", f, f_size, a_size, diff, f->next, f->prev);//test

                    if (f->prev == NULL)
                    {
                        f->next->prev = NULL; //removing first node from free list
                        list_head = (char *) f->next;
                    }
                    else if (f->next == NULL) f->prev->next = NULL; //removing last node from free list
                    else
                    {
                        f->prev->next = f->next; //set the next pointer of f's previous node to f's next node
                        f->next->prev = f->prev; //set the prev pointer of f's next node to f's previous node
                    }

                    return user_mem;
                }
	
                //requested a_size is larger than available f_size. Move to next node (if present) and try again
                else if (diff < 0 && f->next != NULL)
                {
                    f = (Flist) malloc_head;

//printf("[diff < 0] f: %p     f_size: %u      a_size: %u     diff: %d     f->next: %p     f->prev: %p\n", f, f_size, a_size, diff, f->next, f->prev);
                    if (f->next != NULL) malloc_head = (char *)f->next;
                }

                //Reached the end of the free list without finding free node of adequate size.
                //Create new page and attach left over memory to end of free list
                else
                {

//printf("sbrk(0):        %p\n", sbrk(0));
//printf("sbrk(0) - 8192: %p\n", sbrk(0) - 8192);
//print_flist();
//Flist f3;
//f3 = (Flist) list_head;
//printf("\n[end of list] f: %p     f_size: %u      a_size: %u     diff: %d     f->next: %p     f->prev: %p\n", f, f_size, a_size, diff, f->next, f->prev);
					
					//process user memory
					user_mem = sbrk(buff_size);
					if (user_mem == (void *) -1)
                    {
                        fprintf(stderr, "(a_size < %u) appending node to end of free list - sbrk(%u) failed: ", buff_size, buff_size);
                        perror("");
                        exit(1);
                    }

                    f_tmp = (Flist) user_mem;
					f_tmp->size = a_size;
                    user_mem += bk_size;
//printf("f_tmp: %p\n", f_tmp);

                    //add leftover memory onto the end of the free list
                    f = (Flist) list_head; 
					//f_tmp += a_size; //NOTE: bug that cost me MAJOR TIME on this lab. This code moves f_tmp pointer a_size*24 bytes instead of just a_size!!!!!
					f_tmp = (Flist)( (char *)f_tmp + a_size );
//printf("a_size: %u\n", a_size);
//printf("f_tmp: %p\n", f_tmp);
					f_tmp->size = buff_size - a_size;
					f_tmp->next = f;
                    f_tmp->prev = NULL;
                    f->prev = f_tmp;
					list_head = (char*) f_tmp;
//printf("\n");
//printf("sbrk(0):        %p\n", sbrk(0));
//printf("sbrk(0) - 8192: %p\n", sbrk(0) - 8192);
//print_flist();
//exit(0);

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

            if (user_mem == (void *) -1)
            {
                fprintf(stderr, "a_size >= buff_size when free list not empty - sbrk(%u) failed: ", a_size);
                perror("");
                exit(1);
            }

            *((unsigned int *)user_mem) = a_size;
            user_mem += bk_size;

            return user_mem;
        }
    }

    //malloc failed
    return NULL;
}


void *
jcalloc(unsigned int nmemb, unsigned int size)
{
	//char *arr[nmemb];
	//int i;
	void *p;
	
	if (nmemb == 0 || size == 0) return NULL;

	p = jmalloc(nmemb * size);
	bzero(p, nmemb * size);
	return p;
	
	//for (i = 0; i < nmemb; i++) 
	//{
	//	arr[i] = (char *) jmalloc(size);
	//	bzero(arr[i], size);
	//}
	//
	//return arr;
}


void *
jrealloc(void *ptr, unsigned int size)
{
	void *dest;
	
	if (ptr == NULL) dest = jmalloc(size);
	else if (size == 0) return NULL;
	else
	{
		dest = jmalloc(size);
		bcopy(ptr, dest, size);
		jfree(ptr);
	}
	
	return dest;
}



