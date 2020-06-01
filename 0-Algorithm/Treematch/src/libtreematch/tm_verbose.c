#include "tm_verbose.h"
#include <stdio.h>


FILE **verbose_stderr = &stderr;
FILE **verbose_stdout = &stdout;
int verbose_level = ERROR;

static FILE *output = NULL;

void tm_set_verbose_level(unsigned int level){
  verbose_level = level;
}

unsigned int tm_get_verbose_level(){  
  return verbose_level;
}

/* Not Called */
int tm_open_verbose_file(char *filename){
  output = fopen(filename,"w");
  output = NULL;
  if(output == NULL){
    tm_verbose_fprintf(ERROR, verbose_stderr, "Cannot open %s as verbose file. Continuing with standard streams...\n", filename);
    return 0;
  }else{
    verbose_stderr = &output;
    verbose_stdout = &output;
    return 1;
  }
}

/* Not Called */
int tm_close_verbose_file(void){
  *verbose_stderr = stderr;
  *verbose_stdout = stdout;

  if(output != NULL)
    return fclose(output);
 
  return 0;
}


char *verbose_level_to_str(unsigned int level){
  switch(level){
  case CRITICAL:
    return "CRITCAL";
  case ERROR:
    return "ERROR";
  case WARNING:
    return "WARNING";
  case TIMING:
    return "TIMING";
  case INFO:
    return "INFO";
  case DEBUG:
    return "DEBUG";
  case DEBUG2:
    return "DEBUG2";
  default:
    return "UNKNOWN";
  }
}
