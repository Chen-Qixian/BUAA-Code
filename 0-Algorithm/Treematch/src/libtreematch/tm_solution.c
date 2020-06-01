#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#if HAVE_LIBSCOTCH
#include <scotch.h>
#endif
#include "tm_config.h"
#include "tm_solution.h"
#include "tm_mt.h"
#include "tm_topology.h"
#include "tm_timings.h"
#include "tm_verbose.h"


#define debug() print_line(__LINE__)/*TODO a suprimer*/


typedef struct {
  int  val;
  long key;
} hash_t;


void print_line(int line_num);
void compute_gain(int *sol,int N,double **gain,double **comm, double **arch);

/* Not Called */
void print_line(int line_num){
  fprintf(stderr,"---> DEBUG line %d\n",line_num);
}

void tm_free_solution(tm_solution_t *sol){
  int i,n;

  n = sol->k_length;

  if(sol->k)
    for(i=0 ; i<n ; i++)
      FREE(sol->k[i]);

  FREE(sol->k);
  FREE(sol->sigma);
  FREE(sol);
}

/*
  Compute the distance in the tree
  between node i and j : the farther away node i and j, the
  larger the returned value.

  The algorithm looks at the largest level, starting from the top,
  for which node i and j are still in the same subtree. This is done
  by iteratively dividing their numbering by the arity of the levels
*/
int distance(tm_topology_tree_t *topology,int i, int j)
{
  int level = 0;
  int arity;
  int f_i, f_j ;
  int depth = topology->nb_levels-1;

  f_i = topology->node_rank[i];
  f_j = topology->node_rank[j];

  tm_verbose_fprintf(DEBUG2,verbose_stdout,"i=%d, j=%d Level = %d f=(%d,%d)\n",i ,j, level, f_i, f_j);

  do{
    level++;
    arity = topology->arity[level];
    if( arity == 0 )
      arity = 1;
    f_i = f_i/arity;
    f_j = f_j/arity;
  } while((f_i!=f_j) && (level < depth));

  tm_verbose_fprintf(DEBUG2, verbose_stdout, "distance(%d,%d):%d\n",topology->node_rank[i], topology->node_rank[j], level);
  /* exit(-1); */
  return level;
}

#if HAVE_LIBSCOTCH
int distance_scotch(tm_topology_scotch_t *topology, int i, int j) {
  int distance, res1, res2;
  SCOTCH_ArchDom* domnptr1;
  SCOTCH_ArchDom* domnptr2;
  domnptr1 = (SCOTCH_ArchDom *)MALLOC(sizeof(SCOTCH_ArchDom));
  domnptr2 = (SCOTCH_ArchDom *)MALLOC(sizeof(SCOTCH_ArchDom));
  res1 = SCOTCH_archDomTerm(&topology->arch, domnptr1, i);
  res2 = SCOTCH_archDomTerm(&topology->arch, domnptr2, j);
  if (res1 && res2) {
    tm_verbose_fprintf(CRITICAL, verbose_stderr, "Scotch domains cannot be created\n");
    exit(-1);
  }
  else {
    distance = SCOTCH_archDomDist(&topology->arch, domnptr1, domnptr2);
  }
  FREE(domnptr1);
  FREE(domnptr2);
  tm_verbose_fprintf(DEBUG2, verbose_stdout, "distance(%d,%d):%d\n",i, j, distance);
  return distance;
}
#endif


double display_sol_sum_com(tm_topology_tree_t *topology, tm_affinity_mat_t *aff_mat, int *sigma)
{
  double a,c,sol;
  int i,j;
  double *cost = topology->common->cost;
  int depth = topology->nb_levels - 1;
  double **mat = aff_mat->mat;
  int N = aff_mat->order;
 
  sol = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++){
      c = mat[i][j];
      /*
	Compute cost in function of the inverse of the distance
	This is due to the fact that the cost matrix is numbered
	from top to bottom : cost[0] is the cost of the longest distance.
      */
      a = cost[depth-distance(topology,sigma[i],sigma[j])];
      tm_verbose_fprintf(DEBUG2,verbose_stdout,"T_%d_%d %f*%f=%f\n",i,j,c,a,c*a);
      sol += c*a;
    }
  
  for (i = 0; i < N; i++) {
    printf("%d", sigma[i]);
    if(i<N-1){
      printf(",");
    }
  }
  printf(" : %g\n",sol);

  return sol;
}

#if HAVE_LIBSCOTCH
double display_sol_sum_com_scotch(tm_topology_scotch_t *topology, tm_affinity_mat_t *aff_mat, int *sigma)
{

  double c,sol;
  int i,j;
  double **mat = aff_mat->mat;
  int N = aff_mat->order;
  int dis;


  sol = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++){
      c = mat[i][j];
      /*
	Compute cost in function of the inverse of the distance
	This is due to the fact that the cost matrix is numbered
	from top to bottom : cost[0] is the cost of the longest distance.
      */

      dis = distance_scotch(topology,sigma[i],sigma[j]);
      if (dis != -1) {
	sol += c*dis;
      }
      else {
	tm_verbose_fprintf(CRITICAL, verbose_stderr, "Negative distance for (%d, %d)!\n", sigma[i], sigma[j]);
	exit(-1);
      }
    }
     
  for (i = 0; i < N; i++) {
    printf("%d", sigma[i]);
    if(i<N-1){
      printf(",");
    }
  }
  printf(" : %g\n",sol);

  return sol;
}
#endif


double display_sol_max_com(tm_topology_tree_t *topology, tm_affinity_mat_t *aff_mat, int *sigma)
{
  double a,c,sol;
  int i,j;
  double *cost = topology->common->cost;
  double **mat = aff_mat->mat;
  int N = aff_mat->order;
  int depth = topology->nb_levels - 1;

  sol = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++){
      c = mat[i][j];
      /*
	Compute cost in funvtion of the inverse of the distance
	This is due to the fact that the cost matrix is numbered
	from top to bottom : cost[0] is the cost of the longest distance.
      */
      a = cost[depth-distance(topology,sigma[i],sigma[j])];
      tm_verbose_fprintf(DEBUG2,verbose_stdout,"T_%d_%d %f*%f=%f\n",i,j,c,a,c*a);
      if(c*a > sol)
	sol = c*a;
    }

  for (i = 0; i < N; i++) {
    printf("%d", sigma[i]);
    if(i<N-1)
      printf(",");
  }
  printf(" : %g\n",sol);

  return sol;
}


#if HAVE_LIBSCOTCH
double display_sol_max_com_scotch(tm_topology_scotch_t *topology, tm_affinity_mat_t *aff_mat, int *sigma)
{
  double dis,c,sol;
  int i,j;
  double **mat = aff_mat->mat;
  int N = aff_mat->order;

  sol = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++){
      c = mat[i][j];
      /*
	Compute cost in funvtion of the inverse of the distance
	This is due to the fact that the cost matrix is numbered
	from top to bottom : cost[0] is the cost of the longest distance.
      */
      dis = distance_scotch(topology, sigma[i], sigma[j]);
      tm_verbose_fprintf(DEBUG2, verbose_stdout, "T_%d_%d %f*%f=%f\n",i ,j ,c ,dis, c*dis);
      if(c*dis > sol)
	sol = c*dis;
    }

  for (i = 0; i < N; i++) {
    printf("%d", sigma[i]);
    if(i<N-1)
      printf(",");
  }
  printf(" : %g\n",sol);

  return sol;
}
#endif

double display_sol_hop_byte(tm_topology_tree_t *topology, tm_affinity_mat_t *aff_mat, int *sigma)
{
  double c,sol;
  int nb_hops;
  int i,j, dis;
  double **mat = aff_mat->mat;
  int N = aff_mat->order;

  sol = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++){
      c = mat[i][j];
      dis = distance(topology,sigma[i],sigma[j]);
      nb_hops = 2*dis;
	 
      tm_verbose_fprintf(DEBUG2, verbose_stdout, "T_%d_%d %f*%d=%f\n",i,j,c,nb_hops,c*nb_hops);
      
      sol += c*nb_hops;
    }

  for (i = 0; i < N; i++) {
    printf("%d", sigma[i]);
    if(i<N-1){
      printf(",");
    }
  }
  printf(" : %g\n",sol);

  return sol;
}


/* TODO  :expliquer qu'il n'y a que deux metriques pour SCOTCH */


double display_sol(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, int *sigma, tm_metric_t metric){
  switch (metric){
  case TM_METRIC_SUM_COM:
    if(topology->topology_type==TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
      return display_sol_sum_com_scotch(topology->topology.scotch, aff_mat, sigma);
#else
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error:\tTrying to use a non tleaf Scotch file for the topology while this library is not linked with Scotch.\n\tWithout Scotch, this library is only able to read tleaf files.\n\tExiting!\n");
      exit(-1);
#endif
    }
    else
      return display_sol_sum_com(topology->topology.tree, aff_mat, sigma);

  case TM_METRIC_MAX_COM:
    if(topology->topology_type==TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
      return display_sol_max_com_scotch(topology->topology.scotch, aff_mat, sigma);
#else
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error:\tTrying to use a non tleaf Scotch file for the topology while this library is not linked with Scotch.\n\tWithout Scotch, this library is only able to read tleaf files.\n\tExiting!\n");
      exit(-1);
#endif
    }
    else
      return display_sol_max_com(topology->topology.tree, aff_mat, sigma);

  case TM_METRIC_HOP_BYTE:
    if(topology->topology_type==TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
      tm_verbose_fprintf(WARNING ,verbose_stdout, "There are only two metrics for scotch (max_com & sum_com)\n");
      tm_verbose_fprintf(WARNING ,verbose_stdout, "Printing by default: the solution for metric sum_com\n");
      return display_sol_sum_com_scotch(topology->topology.scotch, aff_mat, sigma);
#else
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error:\tTrying to use a non tleaf Scotch file for the topology while this library is not linked with Scotch.\n\tWithout Scotch, this library is only able to read tleaf files.\n\tExiting!\n");
      exit(-1);
#endif
    }
    else
      return display_sol_hop_byte(topology->topology.tree, aff_mat, sigma);

  default:
    tm_verbose_fprintf(ERROR, verbose_stderr,"Error printing solution: metric %d not implemented\n", metric);
  }
  return -1;
}

double tm_display_solution(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, tm_solution_t *sol,
			   tm_metric_t metric){
  int i,j;
  int **k = sol->k;

  if(verbose_level >= DEBUG){
    tm_verbose_fprintf(DEBUG, verbose_stdout, "k: \n");
    for( i = 0 ; i < nb_processing_units(topology) ; i++ ){
   
      if(k[i][0] != -1){
	tm_verbose_fprintf(DEBUG, verbose_stdout, "\tProcessing unit %d: ",i);
	for (j = 0 ; j<sol->oversub_fact; j++){/*utilise oversubfact de sol plutot que de*/
	  if( k[i][j] == -1)
	    break;
	  fprintf(*verbose_stdout, "%d ",k[i][j]);
	}
	fprintf(*verbose_stdout, "\n");
      }
    }
  }

  if (topology->topology_type == TM_TOPOLOGY_TREE || topology->topology_type == TM_TOPOLOGY_SCOTCH){
    return display_sol(topology, aff_mat, sol->sigma, metric);
  }else{
    tm_verbose_fprintf(CRITICAL, verbose_stderr, "Unknow topology type line %d of file %s\n",__LINE__, __FILE__);
    exit(-1);
  }
}

void tm_display_other_heuristics(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, tm_metric_t metric)
{
  int *sigma = NULL;
  int N  = aff_mat->order;

  sigma = (int*)MALLOC(sizeof(int)*N);
  if (topology->topology_type == TM_TOPOLOGY_TREE){
    map_Packed(topology->topology.tree, N, sigma);
    printf("Packed: ");
    display_sol(topology, aff_mat, sigma, metric);

    map_RR(topology->topology.tree, N, sigma);
    printf("RR: ");
    display_sol(topology, aff_mat, sigma, metric);
  }

  /* double duration; */
  /* CLOCK_T time1,time0; */
  /* CLOCK(time0); */
  /* map_MPIPP(topology->topology.tree,1,N,sigma,aff_mat->mat,aff_mat->mat); */
  /* CLOCK(time1); */
  /* duration=CLOCK_DIFF(time1,time0); */
  /* printf("MPIPP-1-D:%10.4f\n",duration*1000); */
  /* //printf("MPIPP-1: "); */
  /* /\* */
  /*   if (TGT_flag == 1) */
  /*   print_sigma_inv(N,sigma,comm,arch); */
  /*   else */
  /*   print_sigma(N,sigma,comm,arch); */
  /* *\/ */

  /* CLOCK(time0); */
  /* map_MPIPP(topology->topology.tree,5,N,sigma,aff_mat->mat,aff_mat->mat); */
  /* CLOCK(time1); */
  /* duration=CLOCK_DIFF(time1,time0); */
  /* printf("MPIPP-5-D:%10.4f\n",duration*1000); */
  /* //printf("MPIPP-5: "); */
  /* /\* */
  /*   if (TGT_flag == 1) */
  /*   print_sigma_inv(N,sigma,comm,arch); */
  /*   else */
  /*   print_sigma(N,sigma,comm,arch); */
  /* *\/ */
  FREE(sigma);

}


int in_tab(int *tab, int n, int val){
  int i;
  for( i = 0; i < n ; i++){
    if(tab[i] == val){
      return 1;
    }
  }

  return 0;
}

void map_Packed(tm_topology_tree_t *topology, int N, int *sigma)
{
  size_t i;
  int j = 0,depth;

  depth = topology->nb_levels-1;

  for( i = 0 ; i < topology->nb_nodes[depth] ; i++){
    /* printf ("%d -> %d\n",objs[i]->os_index,i); */
    if((!topology->common->constraints) || (in_tab(topology->common->constraints, topology->common->nb_constraints, topology->node_id[i]))){
      tm_verbose_fprintf(DEBUG,verbose_stdout,"%lu: %d -> %d\n", i, j, topology->node_id[i]);
      sigma[j++]=topology->node_id[i];
      if(j == N)
	break;
    }
  }
}

void map_RR(tm_topology_tree_t *topology, int N,int *sigma)
{
  int i;

  for( i = 0 ; i < N ; i++ ){
    if(topology->common->constraints)
      sigma[i]=topology->common->constraints[i%topology->common->nb_constraints];
    else
      sigma[i]=i%topology->common->nb_proc_units;
    tm_verbose_fprintf(DEBUG,verbose_stdout,"%d -> %d (%d)\n",i,sigma[i],topology->common->nb_proc_units);
  }
}

/* Not Called */
int hash_asc(const void* x1,const void* x2)
{
  hash_t *e1 = NULL,*e2 = NULL;

  e1 = ((hash_t*)x1);
  e2 = ((hash_t*)x2);

  return (e1->key < e2->key) ? -1 : 1;
}

/* Not Called */
int *generate_random_sol(tm_topology_tree_t *topology,int N, int seed)
{
  hash_t *hash_tab = NULL;
  int *sol = NULL;
  //int *nodes_id= NULL;
  int i;

  //nodes_id = (int)MALLOC(sizeof(int)*N);
  //nodes_id = topology->node_id;

  hash_tab = (hash_t*)MALLOC(sizeof(hash_t)*N);
  sol = (int*)MALLOC(sizeof(int)*N);

  init_genrand(seed);

  for( i = 0 ; i < N ; i++ ){
    hash_tab[i].val = topology->node_id[i];
    hash_tab[i].key = genrand_int32();
  }

  qsort(hash_tab,N,sizeof(hash_t),hash_asc);
  for( i = 0 ; i < N ; i++ )
    sol[i] = hash_tab[i].val;

  FREE(hash_tab);
  return sol;
}

/* Not Called */
double eval_sol(int *sol,int N,double **comm, double **arch)
{
  double a,c,res;
  int i,j;

  res = 0;
  for ( i = 0 ; i < N ; i++ )
    for ( j = i+1 ; j < N ; j++ ){
      c = comm[i][j];
      a = arch[sol[i]][sol[j]];
      res += c/a;
    }

  return res;
}

/* Not Called */
void exchange(int *sol,int i,int j)
{
  int tmp;
  tmp = sol[i];
  sol[i] = sol[j];
  sol[j] = tmp;
}

/* Not Called */
double gain_exchange(int *sol,int l,int m,double eval1,int N,double **comm, double **arch)
{
  double eval2;
  if( l == m )
    return 0;
  exchange(sol,l,m);
  eval2 = eval_sol(sol,N,comm,arch);
  exchange(sol,l,m);

  return eval1-eval2;
}

/* Not Called */
void select_max(int *l,int *m,double **gain,int N,int *state)
{
  double max;
  int i,j;

  max = -DBL_MAX;

  for( i = 0 ; i < N ; i++ )
    if(!state[i])
      for( j = 0 ; j < N ; j++ )
	if( (i != j) && (!state[j]) ){
	  if(gain[i][j] > max){
	    *l = i;
	    *m = j;
	    max=gain[i][j];
	  }
	}
}

/* Not Called */
void compute_gain(int *sol,int N,double **gain,double **comm, double **arch)
{
  double eval1;
  int i,j;

  eval1 = eval_sol(sol,N,comm,arch);
  for( i = 0 ; i < N ; i++ )
    for( j = 0 ; j <= i ; j++)
      gain[i][j] = gain[j][i] = gain_exchange(sol,i,j,eval1,N,comm,arch);
}


/* Randomized Algorithm of
   Hu Chen, Wenguang Chen, Jian Huang ,Bob Robert,and H.Kuhn. Mpipp: an automatic profile-guided
   parallel process placement toolset for smp clusters and multiclusters. In
   Gregory K. Egan and Yoichi Muraoka, editors, ICS, pages 353-360. ACM, 2006.
*/

/* Not Called */

void map_MPIPP(tm_topology_tree_t *topology,int nb_seed,int N,int *sigma,double **comm, double **arch)
{
  int *sol = NULL;
  int *state = NULL;
  double **gain = NULL;
  int **history = NULL;
  double *temp = NULL;
  int i,j,t,l=0,m=0,seed=0;
  double max,sum,best_eval,eval;


  gain = (double**)MALLOC(sizeof(double*)*N);
  history = (int**)MALLOC(sizeof(int*)*N);
  for( i = 0 ; i < N ; i++){
    gain[i] = (double*)MALLOC(sizeof(double)*N);
    history[i] = (int*)MALLOC(sizeof(int)*3);
  }

  state = (int*)MALLOC(sizeof(int)*N);
  temp = (double*)MALLOC(sizeof(double)*N);

  sol = generate_random_sol(topology, N, seed++);
  for( i = 0 ; i < N ; i++)
    sigma[i] = sol[i];

  best_eval = DBL_MAX;
  while(seed <= nb_seed){
    do{
      for( i =  0 ; i < N ; i++ ){
	state[i] = 0;
	/* printf("%d ",sol[i]); */
      }
      /* printf("\n"); */
      compute_gain(sol,N,gain,comm,arch);
      /*
	display_tab(gain,N);
	exit(-1);
      */
      for( i = 0 ; i < N/2 ; i++ ){
	select_max(&l,&m,gain,N,state);
	/* printf("%d: %d <=> %d : %f\n",i,l,m,gain[l][m]); */
	state[l] = 1;
	state[m] = 1;
	exchange(sol,l,m);
	history[i][1] = l;
	history[i][2] = m;
	temp[i] = gain[l][m];
	compute_gain(sol,N,gain,comm,arch);
      }

      t = -1;
      max = 0;
      sum = 0;
      for(i = 0 ; i < N/2 ; i++ ){
	sum += temp[i];
	if( sum > max ){
	  max = sum;
	  t = i;
	}
      }
      /*for(j=0;j<=t;j++)
	printf("exchanging: %d with %d for gain: %f\n",history[j][1],history[j][2],temp[j]); */
      for( j = t+1 ; j < N/2 ; j++ ){
	exchange(sol,history[j][1],history[j][2]);
	/* printf("Undoing: %d with %d for gain: %f\n",history[j][1],history[j][2],temp[j]);  */
      }
      /* printf("max=%f\n",max); */

      /*for(i=0;i<N;i++){
	printf("%d ",sol[i]);
	}
	printf("\n");*/
      eval = eval_sol(sol,N,comm,arch);
      if(eval < best_eval){
	best_eval = eval;
	for(i = 0 ; i < N ; i++)
	  sigma[i] = sol[i];
	/* print_sol(N); */
      }
    }while( max > 0 );
    FREE(sol);
    sol=generate_random_sol(topology, N, seed++);
  }


  FREE(sol);
  FREE(temp);
  FREE(state);
  for( i = 0 ; i < N ; i++){
    FREE(gain[i]);
    FREE(history[i]);
  }
  FREE(gain);
  FREE(history);
}

