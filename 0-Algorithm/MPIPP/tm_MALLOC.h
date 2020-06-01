#ifndef _TM_MALLOC_H_
#define _TM_MALLOC_H_

#include <stdlib.h>
void *tm_malloc(size_t size, char *, int);
void *tm_calloc(size_t count, size_t size, char *, int);
void *tm_realloc(void *ptr, size_t size, char *, int);
void tm_free(void *ptr);
unsigned long genrand_int32(void); 
void init_genrand(unsigned long s);

#define MALLOC(x) tm_malloc(x,__FILE__,__LINE__)
#define FREE   tm_free
#define CALLOC(x,y) tm_calloc(x,y,__FILE__,__LINE__)
#define REALLOC(x,y) tm_realloc(x,y,__FILE__,__LINE__)

#endif