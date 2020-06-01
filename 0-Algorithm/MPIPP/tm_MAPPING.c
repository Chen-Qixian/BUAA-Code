/*
* @Author: kingofwolf
* @Date:   2019-03-07 17:13:08
* @Last Modified by:   kingofwolf
* @Last Modified time: 2019-03-09 15:42:42
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include "tm_MAPPING.h"
#include "tm_TOPOLOGY.h"

#define MIN(a,b) (a)<(b)?(a):(b)


#define LINE_SIZE (1000000)


tm_affinity_mat_t * new_affinity_mat(double **mat, double *sum_row, int order, long int nnz){
  tm_affinity_mat_t * aff_mat;

  aff_mat = (tm_affinity_mat_t *) MALLOC(sizeof(tm_affinity_mat_t));
  aff_mat -> mat     = mat;
  aff_mat -> sum_row = sum_row;
  aff_mat -> order   = order;
  aff_mat -> nnz      = nnz;

  return aff_mat;
}

void tm_free_affinity_mat(tm_affinity_mat_t *aff_mat){
  int i;
  int n = aff_mat->order;

  for(i = 0 ; i < n ; i++)
    FREE(aff_mat->mat[i]);

  FREE(aff_mat->mat);
  FREE(aff_mat->sum_row);
  FREE(aff_mat);
}

size_t get_filesize(char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int nb_lines(char *filename)
{
  FILE *pf = NULL;
  char line[LINE_SIZE];
  int N = 0;

  if(!(pf = fopen(filename,"r"))){
    //if(tm_get_verbose_level() >= CRITICAL)
      fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  while(fgets(line,LINE_SIZE,pf))
    N++;

  // if(tm_get_verbose_level() >= DEBUG)
  //   printf("Number of lines of file %s = %d\n",filename,N);

  fclose(pf);
  return N;
}

char *parse_line(int i, double **mat, double *sum_row, int N, char *data, char *filename, long int *nnz){
  /* now parse the buffer byte per byte for the current line i until we reach '\n'*/
  //unsigned int vl = tm_get_verbose_level();
  long val;
  sum_row[i] = 0;
  int j = 0;
  while(*data != '\n'){
    while(*data ==' ' || *data == '\t')
      data++;
    if(*data != '\n'){
      val = 0;
      while(*data !=' ' && *data != '\t' && *data != '\n'){
	val = val*10 + *data-'0';
	data++;
      }
      mat[i][j] = val;
      /* printf("mat[%d][%d] = %ld\n",i,j, val); */
      if (val){
	(*nnz)++; 
	sum_row[i] += val;
      }
      j++;
    }
  }
  if( j != N){
    //if(vl >= CRITICAL)
      fprintf(stderr,"Error at %d %d (%d!=%d). Wrong number of columns line %d for file %s\n",i ,j ,j ,N ,i+1, filename);
    exit(-1);
  }
  data++;
  return data;
}

/* buffered read with mmap of teh file */
long int init_mat_mmap(char *filename,int N, double **mat, double *sum_row){
  int i;
  //unsigned int vl = tm_get_verbose_level();
  size_t filesize = get_filesize(filename);
  int fd = open(filename, O_RDONLY, 0);
  long int nnz = 0;

  if(fd == -1){
    //if(vl >= CRITICAL)
      fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  char* data = (char*) mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
  
  if(data == MAP_FAILED){
    //if(vl >= CRITICAL)
      fprintf(stderr,"Cannot mmap %s\n",filename);
    exit(-1);
  }
   
  i = 0;
  while(i<N){
    data = parse_line(i, mat, sum_row, N, data, filename, &nnz);
    i++;
  }

  munmap(data, filesize);
  /* fprintf(stderr,"DONE!\n"); */
  close (fd);
  return nnz;
}


tm_affinity_mat_t *tm_load_aff_mat(char *filename)
{
  double **mat = NULL;
  double *sum_row = NULL;
  int i, order;
  long int nnz;
  
  // if(tm_get_verbose_level() >= INFO)
  //   printf("Reading matrix file: %s\n",filename);

  order = nb_lines(filename);

  sum_row = (double*)MALLOC(order*sizeof(double));
  mat = (double**)MALLOC(order*sizeof(double*));
  for( i = 0 ; i < order ; i++)
    /* the last column stores the sum of the line*/
    mat[i] = (double*)MALLOC((order)*sizeof(double));

    nnz = init_mat_mmap(filename,order, mat, sum_row); 
    // if(tm_get_verbose_level() >= DEBUG)
    //   printf("MMap parser\n");

  
  /* TIC; */
  /* init_mat(filename,order, mat, sum_row); */
  /* double duration_fl = TOC; */
  /* printf("Old parser = %.3f\n",duration_fl); */



    //if(tm_get_verbose_level() >= INFO)
    //printf("Affinity matrix built from %s!\n",filename);

    return new_affinity_mat(mat, sum_row, order, nnz);


}


