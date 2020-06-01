#include "tm_config.h"
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
#include <assert.h>

#include "tm_mt.h"
#include "tm_mapping.h"
#include "tm_timings.h"
#include "tm_thread_pool.h"
#include "tm_tree.h"
#include "tm_kpartitioning.h"
#include "tm_scotch.h"


#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#endif


#if HAVE_LIBSCOTCH
#include <scotch.h>
#endif

#include <sys/mman.h>


#define MIN(a,b) (a)<(b)?(a):(b)

#define TEST_ERROR(n) do{				\
    if( (n) != 0 ){					\
      fprintf(stderr,"Error %d Line %d\n",n,__LINE__);	\
      exit(-1);}					\
  }while(0)

#define LINE_SIZE (2000000)


typedef struct {
  double val;
  int key1;
  int key2;
} hash2_t;

tm_affinity_mat_t * new_affinity_mat(double **mat, double *sum_row, int order, long int nnz);
int compute_nb_leaves_from_level(int depth, tm_topology_tree_t *topology);
void depth_first(tm_tree_t *comm_tree, int *proc_list,int *i);
int  fill_tab(int **new_tab,int *tab, int n, int start, int max_val, int shift);
long int init_mat(char *filename,int N, double **mat, double *sum_row);
void map_topology_tree(tm_topology_tree_t *topology,tm_tree_t *comm_tree, int level,
		       int *sigma, int nb_processes, int **k, int nb_compute_units);
int nb_leaves(tm_tree_t *comm_tree);
int nb_lines(char *filename);
void print_1D_tab(int *tab,int N);
tm_solution_t * tm_compute_mapping(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, double *obj_weight, double *com_speed);
void tm_finalize();
void tm_free_affinity_mat(tm_affinity_mat_t *aff_mat);
tm_affinity_mat_t *tm_load_aff_mat(char *filename);
void update_comm_speed(double **comm_speed,int old_size,int new_size);
tm_affinity_mat_t * tm_build_affinity_mat(double **mat, int order);
#if HAVE_LIBSCOTCH
int *convarray_scotchnum_to_int(SCOTCH_Num * array_in , SCOTCH_Num nsc);
#endif
int max_oversub(int * parttab_int , int size , int nb_proc);
int nb_proc_util(int * parttab_int , int size);
void fill_k(int **k , int *parttab_int , int oversub , int nb_proccessus);
//
tm_solution_t * build_mapping_tree(tm_topology_tree_t *topology, tm_affinity_mat_t *aff_mat, double *obj_weight, double *com_speed);
size_t get_filesize(char* filename);
long int init_mat_long(char *filename,int N, double **mat, double *sum_row);
long int init_mat_mmap(char *filename,int N, double **mat, double *sum_row);
char *parse_line(int i, double **mat, double *sum_row, int N, char *data, char *filename, long int *nnz);
void set_val(int *tab, int val, int n);


/* compute the number of leaves of any subtree starting froma node of depth depth*/
int compute_nb_leaves_from_level(int depth,tm_topology_tree_t *topology)
{
  int res = 1;

  while(depth < topology->nb_levels-1)
    res *= topology->arity[depth++];

  return res;
}

void tm_finalize(){
  terminate_thread_pool();
  tm_close_verbose_file();
  tm_mem_check();
}



void print_1D_tab(int *tab,int N)
{
  int i;
  for (i = 0; i < N; i++) {
    fprintf(*verbose_stdout,"%d", tab[i]);
    if( i < (N-1) )
      fprintf(*verbose_stdout, ",");
  }
  fprintf(*verbose_stdout,"\n");
}


/* Not Called */
long int  init_mat(char *filename,int N, double **mat, double *sum_row){
  FILE *pf = NULL;
  char *ptr= NULL;
  char line[LINE_SIZE];
  int i,j;
  long int nnz = 0;

  if(!(pf=fopen(filename,"r"))){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  j = -1;
  i = 0;

  while(fgets(line,LINE_SIZE,pf)){
    char *l = line;
    j = 0;
    sum_row[i] = 0;
    while((ptr=strtok(l," \t"))){
      l = NULL;
      if((ptr[0]!='\n')&&(!isspace(ptr[0]))&&(*ptr)){
	mat[i][j] = atof(ptr);
	if(mat[i][j]) nnz++;
	sum_row[i] += mat [i][j];
	if(mat[i][j]<0){
	  tm_verbose_fprintf(WARNING,verbose_stdout,"Warning: negative value in com matrix! mat[%d][%d]=%f\n",i,j,mat[i][j]);
	}
	j++;
      }
    }
    if( j != N){
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error at %d %d (%d!=%d). Too many columns for %s\n",i,j,j,N,filename);
      exit(-1);
    }
    i++;
  }


  if( i != N ){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error at %d %d. Too many rows for %s\n",i,j,filename);
    exit(-1);
  }

  fclose (pf);
  return nnz;
}


size_t get_filesize(char* filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}


char *parse_line(int i, double **mat, double *sum_row, int N, char *data, char *filename, long int *nnz){
  /* now parse the buffer byte per byte for the current line i until we reach '\n'*/
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
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error at %d %d (%d!=%d). Wrong number of columns line %d for file %s\n",i ,j ,j ,N ,i+1, filename);

    exit(-1);
  }
  data++;
  return data;
}



/* buffered read with mmap of the file */
long int init_mat_mmap(char *filename, int N, double **mat, double *sum_row){
  int i;
  size_t filesize = get_filesize(filename);
  int fd = open(filename, O_RDONLY, 0);
  long int nnz = 0;
  int res;
  char *data, *saved_data;

  if(fd == -1){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  tm_verbose_fprintf(DEBUG,verbose_stdout,"Open %s: success for %ld bytes in %d lines!\n", filename, filesize, N);

  data = (char*) mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
  saved_data = data;

  if(data == MAP_FAILED){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot mmap %s\n",filename);
    exit(-1);
  }

  tm_verbose_fprintf(DEBUG,verbose_stdout,"Mmap %s: success (%p)!\n",filename, (void*)data);


  i = 0;
  while(i<N){
    data = parse_line(i, mat, sum_row, N, data, filename, &nnz);
    i++;
  }
  tm_verbose_fprintf(DEBUG,verbose_stdout,"All data read (%p, %ld)!\n", (void*)data, filesize);

  res = munmap(saved_data, filesize);
  if(res == -1){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Unmmap failed\n");
    exit(-1);
  }

  close (fd);
  return nnz;
}



tm_affinity_mat_t * new_affinity_mat(double **mat, double *sum_row, int order, long int nnz){
  tm_affinity_mat_t * aff_mat;

  aff_mat = (tm_affinity_mat_t *) MALLOC(sizeof(tm_affinity_mat_t));
  aff_mat -> mat     = mat;
  aff_mat -> sum_row = sum_row;
  aff_mat -> order   = order;
  aff_mat -> nnz      = nnz;

  return aff_mat;
}

/* Not Called */
tm_affinity_mat_t * tm_build_affinity_mat(double **mat, int order){
  double *sum_row = NULL;
  int i,j;
  long int nnz = 0;
  sum_row = (double*)MALLOC(order*sizeof(double));

  for( i = 0 ; i < order ; i++){
    sum_row[i] = 0;
    for(j = 0 ; j < order ; j++){
      if(mat[i][j]){
	nnz++;
	sum_row[i] += mat [i][j];
      }
    }
  }

  return new_affinity_mat(mat, sum_row, order, nnz);
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


int nb_lines_pos(char *filename, long  start_pos, long end_pos)
{
  FILE *pf = NULL;
  char *line;
  char *p;
  int N = 0;
  int size;
  long remain_size;
    
  if(!(pf = fopen(filename,"r"))){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  line = (char*)MALLOC(sizeof(char*)*LINE_SIZE);
  if(!line){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate line!\n");
    exit(-1);
  }


  fseek(pf, start_pos, SEEK_SET);
  
  remain_size = end_pos - ftell(pf);
  size = MIN(LINE_SIZE, remain_size);
  p=line+size-1;
  *p = '\n';
  while(remain_size>0){
    fgets(line, size, pf);
    /* printf("N= %d, sp=%ld, ep= %ld, ft=%ld, rs = %ld, s=%p\n",N ,start_pos, end_pos, ftell(pf), remain_size, s); */
    /* if the last character is not a newline, it means that we have not finished reading the current line*/
    if((*p == '\n') || (*(p-1)== '\n')){
      N++;
    } 
    remain_size = end_pos - ftell(pf);
    /* printf("%d (%d): %s| (rs= %d)\n",N, (int)*p, line, remain_size); */
    size = MIN(LINE_SIZE, remain_size+2); /* remain_size + 2 : because we need space for fgets to trail the \0*/
    p=line+size-1;
    *p = '\n';
  }

  
  tm_verbose_fprintf(DEBUG, verbose_stdout, "Number of lines of file %s = %d\n",filename,N);

  fclose(pf);
  /* printf("(%ld->%ld)Read %d lines\n",start_pos, end_pos, N); */
  FREE(line);
  return N;
}



void partial_nb_lines (int nb_args, void **args, int thread_id){
  long inf = *(long*)args[0];
  long sup = *(long*)args[1];
  char *filename = (char*)args[2];
  int *res=(int*)args[3];

  if(nb_args != 4){
    tm_verbose_fprintf(CRITICAL ,verbose_stderr, "(Thread: %d) Wrong number of args in %s: %d\n",thread_id, __func__, nb_args);
    exit(-1);
  }

  *res = nb_lines_pos(filename, inf, sup);
}



int nb_lines(char *filename){
  int nb_threads, id;
  work_t **works;
  long *inf;
  long *sup;
  int res, *tab_res;
  size_t filesize = get_filesize(filename);


  
  if(filesize < 1){
    return nb_lines_pos(filename, 0, filesize); 
  } else{
    nb_threads = get_nb_threads();
  }

  works = (work_t**)MALLOC(sizeof(work_t*)*nb_threads);
  if(!works){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate works!\n");
    exit(-1);
  }

  inf = (long*)MALLOC(sizeof(long)*nb_threads);
  if(!inf){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate inf!\n");
    exit(-1);
  }

  sup = (long*)MALLOC(sizeof(long)*nb_threads);
  if(!sup){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate sup!\n");
    exit(-1);
  }

  tab_res = (int*)MALLOC(sizeof(int)*nb_threads);
  if(!tab_res){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate tab_res!\n");
    exit(-1);
  }

  
  for(id=0 ; id < nb_threads ; id++){
    void **args=(void**)MALLOC(sizeof(void*)*4);
    if(!args){
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate args!\n");
      exit(-1);
    }

    inf[id]=id*filesize/nb_threads;
    sup[id]=(id+1)*filesize/nb_threads-1;
    if(id == nb_threads-1) sup[id]=filesize;
    /* fprintf(stderr,"%d/%d (%ld->%ld)\n", id, nb_threads, inf[id], sup[id]); */
    args[0]=(void*)(inf+id);
    args[1]=(void*)(sup+id);
    args[2]=(void*)filename;
    args[3]=(void*)(tab_res+id);

    works[id] = create_work(4,args,partial_nb_lines);
    tm_verbose_fprintf(DEBUG,verbose_stdout,"Executing %p\n",(void *)works[id]);
    
    submit_work(works[id], id);
  }

  res = 0;
  for(id=0;id<nb_threads;id++){
    wait_work_completion(works[id]);
    res+=tab_res[id];
    FREE(works[id]->args);
    destroy_work(works[id]);
  }

  FREE(inf);
  FREE(sup);
  FREE(tab_res);
  FREE(works);

  return res;
}

tm_affinity_mat_t *tm_load_aff_mat(char *filename)
{
  double **mat = NULL;
  double *sum_row = NULL;
  int i, order;
  long int nnz;

  tm_verbose_fprintf(INFO,verbose_stdout,"Reading matrix file: %s\n",filename);

  TIC;
  order = nb_lines(filename);
  double duration = TOC;
  tm_verbose_fprintf(TIMING, verbose_stdout, "reading %d lines in %.2fms\n",order, duration*1000);
  
  sum_row = (double*)MALLOC(order*sizeof(double));
  mat = (double**)MALLOC(order*sizeof(double*));
  for( i = 0 ; i < order ; i++)
    /* the last column stores the sum of the line*/
    mat[i] = (double*)MALLOC((order)*sizeof(double));
  /* on my mac  parsing large file is better done with fopen than mmap */
#ifdef __MACH__
  if (get_filesize(filename) > 1024*1024*1014) {
    nnz = init_mat_long(filename,order, mat, sum_row);
    tm_verbose_fprintf(DEBUG,verbose_stdout,"New parser\n");


  }else{
    nnz = init_mat_mmap(filename,order, mat, sum_row);
    tm_verbose_fprintf(DEBUG,verbose_stdout,"MMap parser\n");
  }
#else
  nnz = init_mat_mmap(filename,order, mat, sum_row);
  tm_verbose_fprintf(DEBUG,verbose_stdout,"MMap parser\n");
#endif

  /* TIC; */
  /* init_mat(filename,order, mat, sum_row); */
  /* double duration_fl = TOC; */
  /* printf("Old parser = %.3f\n",duration_fl); */

  tm_verbose_fprintf(INFO,verbose_stdout, "Affinity matrix built from %s!\n",filename);

  return new_affinity_mat(mat, sum_row, order, nnz);

}





/* void map_tree(tm_tree_t* t1,tm_tree_t *t2) */
/* { */
/*  double x1,x2;
    if((!t1->left)&&(!t1->right)){
    printf ("%d -> %d\n",t1->id,t2->id);
    sigma[t2->id]=t1->id;
    return;
    }
    x1=t2->right->val/t1->right->val+t2->left->val/t1->left->val;
    x2=t2->left->val/t1->right->val+t2->right->val/t1->left->val;
    if(x1<x2){
    map_tree(t1->left,t2->left);
    map_tree(t1->right,t2->right);
    }else{
    map_tree(t1->right,t2->left);
    map_tree(t1->left,t2->right);
    }*/
/* } */

void depth_first(tm_tree_t *comm_tree, int *proc_list,int *i)
{
  int j;
  if(!comm_tree->child){
    proc_list[(*i)++] = comm_tree->id;
    return;

  }

  for( j  = 0 ; j < comm_tree->arity ; j++ )
    depth_first(comm_tree->child[j],proc_list,i);
}

int nb_leaves(tm_tree_t *comm_tree)
{
  int j,n=0;

  if(!comm_tree->child)
    return 1;

  for( j = 0 ; j < comm_tree->arity ; j++)
    n += nb_leaves(comm_tree->child[j]);

  return n;
}

/* find the first '-1 in the array of size n and put the value there*/
void set_val(int *tab, int val, int n){
  int i = 0;

  while (i < n ){
    if(tab[i] ==- 1){
      tab[i] = val;
      return;
    }
    i++;
  }

  tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error while assigning value %d to k\n",val);

  exit(-1);

}
/*Map topology to cores for a tree:
  sigma_i is such that  process i is mapped on core sigma_i
  k_i is such that core i exectutes process k_i

  size of sigma is the number of process "nb_processes"
  size of k is the number of cores/nodes "nb_compute_units"

  We must have numbe of process<=number of cores

  k_i =-1 if no process is mapped on core i
*/

void map_topology_tree(tm_topology_tree_t *topology,tm_tree_t *comm_tree, int level,
		       int *sigma, int nb_processes, int **k, int nb_compute_units)
{
  int *nodes_id = NULL;
  int *proc_list = NULL;
  int i,j,N,M,block_size;

  M = nb_leaves(comm_tree);
  nodes_id = topology->node_id;
  N = topology->nb_nodes[level];

  tm_verbose_fprintf(INFO,verbose_stdout,"nb_leaves=%d\nlevel=%d, nodes_id=%p, N=%d\nN=%d,nb_compute_units=%d\n",M,level,(void *)nodes_id,N,N,nb_compute_units);


  /* The number of node at level "level" in the tree should be equal to the number of processors*/
  assert(N==nb_compute_units*topology->common->oversub_fact);

  proc_list = (int*)MALLOC(sizeof(int)*M);
  i = 0;
  depth_first(comm_tree,proc_list,&i);

  block_size = M/N;

  if(k){/*if we need the k vector*/
    tm_verbose_fprintf(INFO,verbose_stdout,"M=%d, N=%d, BS=%d\n",M,N,block_size);

    for( i = 0 ; i < nb_processing_units_tree(topology) ; i++ )
      for(j = 0 ; j < topology->common->oversub_fact ; j++){
	k[i][j] = -1;
      }

    for( i = 0 ; i < M ; i++ )
      if(proc_list[i] != -1){
	tm_verbose_fprintf(DEBUG,verbose_stdout,"%d->%d\n",proc_list[i],nodes_id[i/block_size]);

	if( proc_list[i] < nb_processes ){
	  sigma[proc_list[i]] = nodes_id[i/block_size];
	  set_val(k[nodes_id[i/block_size]], proc_list[i], topology->common->oversub_fact);
	}
      }
  }else{
    tm_verbose_fprintf(INFO,verbose_stdout,"M=%d, N=%d, BS=%d\n",M,N,block_size);
    for( i = 0 ; i < M ; i++ )
      if(proc_list[i] != -1){
	tm_verbose_fprintf(DEBUG,verbose_stdout,"%d->%d\n",proc_list[i],nodes_id[i/block_size]);
	if( proc_list[i] < nb_processes )
	  sigma[proc_list[i]] = nodes_id[i/block_size];
      }
  }

  if((verbose_level >= DEBUG) && (k)){
    tm_verbose_fprintf(DEBUG, verbose_stdout, "k: \n");
    for( i = 0 ; i < nb_processing_units_tree(topology) ; i++ ){
          tm_verbose_fprintf(DEBUG, verbose_stdout, "Procesing unit %d: ",i);
      for (j = 0 ; j<topology->common->oversub_fact; j++){
	if( k[i][j] == -1)
	  break;
	fprintf(*verbose_stdout,"%d ",k[i][j]);
      }
      fprintf(*verbose_stdout,"\n");
    }
  }

  FREE(proc_list);
}

tm_solution_t * build_mapping_tree(tm_topology_tree_t *topology, tm_affinity_mat_t *aff_mat, double *obj_weight, double *com_speed){
  size_t sigma_length;
  size_t k_length;
  size_t i;
  int *sigma, **k;
  tm_tree_t *comm_tree=NULL;
  tm_solution_t *solution =  (tm_solution_t *)MALLOC(sizeof(tm_solution_t));
  comm_tree     = tm_build_tree_from_topology(topology, aff_mat, obj_weight, com_speed);
  sigma_length  = comm_tree->nb_processes;
  k_length      = nb_processing_units_tree(topology);
  sigma         =  (int*)  MALLOC(sizeof(int) * sigma_length);
  k             =  (int**) MALLOC(sizeof(int*) * k_length);

  for (i=0 ; i < k_length ; i++){
    k[i] =  (int*) MALLOC(sizeof(int) * topology->common->oversub_fact);
  }
  map_topology_tree(topology, comm_tree, topology->nb_levels-1, sigma, sigma_length ,k, k_length);


  solution->sigma         = sigma;
  solution->sigma_length  = sigma_length;
  solution->k             = k;
  solution->k_length      = k_length;
  solution->oversub_fact  = topology->common->oversub_fact;

  tm_free_tree(comm_tree);
  return solution;
}

tm_solution_t * tm_compute_mapping(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, double *obj_weight, double *com_speed)
{

  if(topology->topology_type==TM_TOPOLOGY_TREE){
    return build_mapping_tree(topology->topology.tree, aff_mat, obj_weight, com_speed);
  }else if (topology->topology_type==TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
    // printf("%d\n", HAVE_LIBSCOTCH);
    return build_mapping_with_scotch(topology->topology.scotch, aff_mat);
#else
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error: Using a Scotch Topology while not linked with Scotch\n");
    
    exit(-1);
#endif
  }else{
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Unkonwn topology type %d in compute_mapping file %s, line %d\n",topology->topology_type, __FILE__, __LINE__);
    exit(-1);
  }

}


/* Not Called */
void update_comm_speed(double **comm_speed,int old_size,int new_size)
{
  double *old_tab = NULL,*new_tab= NULL;
  int i;

  tm_verbose_fprintf(DEBUG,verbose_stdout,"comm speed [%p]: ",(void *)*comm_speed);
  old_tab = *comm_speed;
  new_tab = (double*)MALLOC(sizeof(double)*new_size);
  *comm_speed = new_tab;

  for( i = 0 ; i < new_size ; i++ ){
    if( i < old_size)
      new_tab[i] = old_tab[i];
    else
      new_tab[i] = new_tab[i-1];

    tm_verbose_fprintf(DEBUG,verbose_stdout,"%f ",new_tab[i]);
  }
  tm_verbose_fprintf(DEBUG,verbose_stdout,"\n");
}






/*
  copy element of tab in *new_tab from start to end and shift negativeley them
  allocates *new_tab
*/
int  fill_tab(int **new_tab,int *tab, int n, int start, int max_val, int shift)
{
  int *res = NULL,i,j,end;

  if(!n){
    *new_tab = NULL;
    return 0;
  }
  end = start;

  /* find how many cell to copy*/
  while( end < n ){
    if(tab[end] >= max_val)
      break;
    end++;
  }

  /* if none return */
  if( start == end ){
    *new_tab = NULL;
    return end;
  }

  /* allocate the result*/
  res = (int*) MALLOC (sizeof(int)*(end-start));

  /* copy and shift*/
  j = 0;
  for( i = start ; i < end ; i++ ){
    res[j] = tab[i] - shift;
    j++;
  }

  /* set the pointer passed in parameter and return */
  *new_tab = res;
  return end;
}




