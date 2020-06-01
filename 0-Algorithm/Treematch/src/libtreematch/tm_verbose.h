#ifndef __TM_VERBOSE_H__
#define __TM_VERBOSE_H__

#include <stdio.h>

#define NONE     0
/* output in stderr*/
#define CRITICAL 1
#define ERROR    2
/* output in stdout*/
#define WARNING  3
#define TIMING   4
#define INFO     5
#define DEBUG    6
#define DEBUG2   7


/* return 0 on errror and  1 on success */
int          tm_open_verbose_file(char *filename);
int          tm_close_verbose_file(void);
void         tm_set_verbose_level(unsigned int level);
unsigned int tm_get_verbose_level(void);
char         *verbose_level_to_str(unsigned int level);


#define tm_verbose_fprintf(level, output, ...) level <= verbose_level? fprintf(*output, "%s: ", verbose_level_to_str(level)),fprintf(*output, __VA_ARGS__):0


#endif
