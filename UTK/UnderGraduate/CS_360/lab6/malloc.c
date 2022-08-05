// NAME:Tom Hills  
// NETID: jhills

// CS 360: Lab6 - Jmalloc

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

//frees memory allocated by jmalloc
void 
jfree(void *ptr)
{
	Flist f, f1;

	if (ptr == NULL) return; 
	ptr -= 8; 

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

//allocates returns pointer to memory of size bytes for use while maintaining a list of buffered free memory  
void *
jmalloc(unsigned int size)
{
	Flist f, f_tmp;
    void *user_mem;
    char *malloc_head;
    unsigned int a_size, f_size;
    int diff, buff_size;
    size_t bk_size;
	int min_size;

    //check if requested size == 0
    if (size == 0) return NULL;

    //initialize local variables
    buff_size = 8192;
    bk_size = sizeof(*f);
	malloc_head = list_head; 
	min_size = bk_size + sizeof(void *);

    //calculate allocation size (a_size)
    //if requested size (size) is not a multiple of 8, upgrade to the next highest multiple of 8
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
                exit(1);
            }

			*((unsigned int *)user_mem) = a_size;
			user_mem += 8;

            //add first node to flist
            malloc_head = (char *)user_mem + size;
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
			user_mem += 8;

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
				// obtain difference between size of node in free list to requested a_size
                f_size = *( (unsigned int *)malloc_head );
                diff = f_size - a_size;

				if ( diff >= min_size )
                {
                    //process user memory
                    user_mem = malloc_head;
                    f_tmp = (Flist) user_mem;
                    f_tmp->size = a_size;
					user_mem += 8;
                    
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
                else if (diff < min_size && diff >= 0)
                {
					//process user memory
                    user_mem = malloc_head;
					user_mem += 8;

                    //adjust free list
                    f = (Flist) malloc_head;
					
					//removing first node from free list
                    if (f->prev == NULL)
                    {
                        f->next->prev = NULL; 
                        list_head = (char *) f->next;
                    }
                    //removing last node from free list
					else if (f->next == NULL) f->prev->next = NULL; 
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
                    if (f->next != NULL) malloc_head = (char *)f->next;
                }

                //Reached the end of the free list without finding free node of adequate size.
                //Create new page and attach left over memory to end of free list
                else
                {

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
					user_mem += 8;

                    //add leftover memory onto the end of the free list
                    f = (Flist) list_head; 
					f_tmp = (Flist)( (char *)f_tmp + a_size );
					f_tmp->size = buff_size - a_size;
					f_tmp->next = f;
                    f_tmp->prev = NULL;
                    f->prev = f_tmp;
					list_head = (char*) f_tmp;

                    return user_mem;
                }
            }
        }

        //requested memory size greater than/equal to one page --- free list NOT empty
        else
        {
            //process user memory
            user_mem = sbrk(a_size);
            if (user_mem == (void *) -1)
            {
                fprintf(stderr, "a_size >= buff_size when free list not empty - sbrk(%u) failed: ", a_size);
                perror("");
                exit(1);
            }

            *((unsigned int *)user_mem) = a_size;
			user_mem += 8;

            return user_mem;
        }
    }

    //malloc failed
    return NULL;
}


void *
jcalloc(unsigned int nmemb, unsigned int size)
{
	void *p;
	
	if (nmemb == 0 || size == 0) return NULL;

	p = jmalloc(nmemb * size);
	bzero(p, nmemb * size);
	return p;
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
