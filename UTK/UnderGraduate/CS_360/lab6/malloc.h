// NAME: Tom Hills 
// NETID: jhills
//
// CS 360: Lab6 - Jmalloc
//
// DESCRIPTION:
// This program mimics the behavior of the std lib call malloc. 

#ifndef _MALLOC_H
#define _MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

	void *jmalloc(unsigned int size);
	void *jcalloc(unsigned int nmemb, unsigned int size);
	void *jrealloc(void *ptr, unsigned int size);
	void jfree(void *ptr);

#ifdef __cplusplus
}  /* extern C */
#endif

#endif
