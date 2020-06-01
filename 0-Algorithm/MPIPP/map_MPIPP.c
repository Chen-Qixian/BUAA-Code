#include <stdlib.h>
#include <stdio.h>
#include "tm_MALLOC.h"
#include <float.h>
#include <ctype.h>
#include "tm_TIMINGS.h"
#include "tm_MAPPING.h"
#include "map_MPIPP.h"


typedef struct {
  int  val;
  long key;
} hash_t;


int hash_asc(const void* x1,const void* x2)
{
  hash_t *e1 = NULL,*e2 = NULL;

  e1 = ((hash_t*)x1);
  e2 = ((hash_t*)x2);

  return (e1->key < e2->key) ? -1 : 1;
}

int *generate_random_sol(tm_topology_t *topology,int N, int seed)
{
  hash_t *hash_tab = NULL;
  int *sol = NULL;
  int *nodes_id= NULL;
  int i;

  nodes_id = topology->node_id;

  hash_tab = (hash_t*)MALLOC(sizeof(hash_t)*N);
  sol = (int*)MALLOC(sizeof(int)*N);

  init_genrand(seed);

  for( i = 0 ; i < N ; i++ ){
    hash_tab[i].val = nodes_id[i];
    hash_tab[i].key = genrand_int32();
  }

  qsort(hash_tab,N,sizeof(hash_t),hash_asc);
  for( i = 0 ; i < N ; i++ )
    sol[i] = hash_tab[i].val;

  FREE(hash_tab);
  return sol;
}

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

void exchange(int *sol,int i,int j)
{
  int tmp;
  tmp = sol[i];
  sol[i] = sol[j];
  sol[j] = tmp;
}

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

void compute_gain(int *sol,int N,double **gain,double **comm, double **arch)
{
  double eval1;
  int i,j;

  eval1 = eval_sol(sol,N,comm,arch);
  for( i = 0 ; i < N ; i++ )
    for( j = 0 ; j <= i ; j++)
      gain[i][j] = gain[j][i] = gain_exchange(sol,i,j,eval1,N,comm,arch);
}

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

void map_MPIPP(tm_topology_t *topology,int nb_seed,int N,int *sigma,double **comm, double **arch)
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



//upstair codes is done

double generate_cost(tm_topology_t *topology, int node1, int node2)
{
  int i;
  int arity_group=1;

  if (node1 == node2) return 0.0;
  for (i = topology->nb_levels-2; i >= 0; i--)
  {
    arity_group *= topology->arity[i];
    if (node1/arity_group == node2/arity_group) return (topology->cost[i] * 2);
  }
  exit(-1);
}


void topology_2_arch(tm_topology_t *topology, double **arch)
{
  int N = topology->nb_nodes[topology->nb_levels-1];
  int i,j;
  int node1,node2;
  for (i = 0; i < N; i++)
  {
    node1 = topology->node_id[i];
    for (j = i; j < N; j++)
    {
      node2 = topology->node_id[j];
      arch[i][j] = generate_cost(topology,node1,node2);
      arch[j][i] = arch[i][j];
      //printf("%d,%d:%.0f\n",i,j,arch[i][j]);
    }
  }
}


void aff_mat_2_comm(tm_affinity_mat_t *aff_mat, double **comm)
{
  int i,j;
  int N=aff_mat->order;
  for (i = 0; i < N; i++){
    for (j = 0; j < N; j++){
      comm[i][j] = aff_mat->mat[i][j];
    }
  }
}

void file_2_arch(double **arch)
{
  FILE * fp=NULL;
  char * filepath=Net_File;
  int c;
  int i,j;
  int m,n;
  int ct=Net_Node[NB_CT],nb_node=Net_Node[NB_NODE],nb_core=Net_Node[NB_CORE];

  fp=fopen(filepath,"r");

  if(!fp){
    //if(tm_get_verbose_level() >= CRITICAL)
      fprintf(stderr,"Cannot open %s\n",filepath);
    exit(-1);
  }
  for (i = 0; i < nb_node; i++){
    for (j = 0; j < nb_node; j++){
      if (feof(fp)) {
        fprintf(stderr, "file analysis to matrix error in line %d,column %d\n", i, j);
        exit(-1);
      }
      fscanf(fp,"%d ",&c);
      for (m = i*nb_core; (m < (i+1)*nb_core) && (m < ct); m++){
        for (n = j*nb_core; (n < (j+1)*nb_core) && (n < ct); n++){
          arch[m][n]=c;//printf("(%d,%d)=%d ",m,n,c);
        }
      }
    }
  }
}

void print_sigma(int N, int *sigma, double **comm, double **arch)
{
  int i;
  FILE *fpout=NULL;
  fpout=fopen("MPIPP_Result.ST","w");
  for (i = 0; i < N; i++)
  {
    printf("%d,", sigma[i]);
    fprintf(fpout, "%d\n", sigma[i]);
  }
  printf("\n");
  fclose(fpout);
}
void print_comm(double **comm, int N)
{
  int i,j;
  printf("comm:\n");
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++){
      printf("%.0f,", comm[i][j]);
    }
    printf("\n");
  }
}
void print_arch(double **arch, int N)
{
  int i,j;
  printf("arch:\n");
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++){
      printf("%.0f,", arch[i][j]);
    }
    printf("\n");
  }
}

void tm_display_other_heuristics(tm_topology_t *topology, tm_affinity_mat_t *aff_mat, tm_metric_t metric)
{
  int *sigma = NULL;
  int N  = aff_mat->order;
  int Nb = topology->nb_nodes[topology->nb_levels - 1];
  double duration;
  double **comm;
  double **arch;
  int TGT_flag=0;
  int i;

  sigma = (int*)MALLOC(sizeof(int)*N);


  // map_Packed(topology, N, sigma);
  // printf("Packed: ");
  // display_sol(topology, aff_mat, sigma, metric);

  // map_RR(topology, N, sigma);
  // printf("RR: ");
  // display_sol(topology, aff_mat, sigma, metric);

//topology ==> arch 
//aff_mat ==> comm
   arch = (double**)MALLOC(Nb*sizeof(double*));
   for (i = 0; i < Nb; i++) arch[i] = (double*)MALLOC((Nb)*sizeof(double));
   comm = (double**)MALLOC(N*sizeof(double*));
   for (i = 0; i < N; i++) comm[i] = (double*)MALLOC((N)*sizeof(double));
   if(Net_Node[NB_FLAG]==1) file_2_arch(arch);
   else topology_2_arch(topology,arch);
   aff_mat_2_comm(aff_mat,comm);
   //printf("%d,%d,%d,%d\n",Net_Node[NB_CT],Net_Node[NB_NODE],Net_Node[NB_CORE],Net_Node[NB_FLAG]);
   //print_arch(arch,Nb);
   //print_comm(comm,N);

   CLOCK_T time1,time0; 
   CLOCK(time0); 
   map_MPIPP(topology,1,N,sigma,comm,arch); 
   CLOCK(time1); 
   duration=CLOCK_DIFF(time1,time0); 
   printf("MPIPP-1-D:%f\n",duration); 
   printf("MPIPP-1: "); 
   // if (TGT_flag == 1)  
   //   print_sigma_inv(N,sigma,comm,arch); 
   // else 
   print_sigma(N,sigma,comm,arch); 

   // CLOCK(time0); 
   // map_MPIPP(topology,5,N,sigma,comm,arch); 
   // CLOCK(time1); 
   // duration=CLOCK_DIFF(time1,time0); 
   // printf("MPIPP-5-D:%f\n",duration); 
   // printf("MPIPP-5: "); 
   // // if (TGT_flag == 1)  
   // //   print_sigma_inv(N,sigma,comm,arch); 
   // // else 
   // print_sigma(N,sigma,comm,arch); 

  FREE(sigma);
   for (i = 0; i < Nb; i++) FREE(arch[i]);
   for (i = 0; i < N; i++) FREE(comm[i]);
   FREE(arch);
   FREE(comm);
}


// int test_mapping(char *arch_filename, tm_file_type_t arch_file_type, char *com_filename, int bind_flag,
//      char *bind_filename, int opt_topo_flag, tm_metric_t metric, int oversub_fact) 
int test_mapping(char *arch_filename,tm_file_type_t arch_file_type,char *com_filename,tm_metric_t metric)
{
   //tm_tree_t      *comm_tree = NULL;
   tm_topology_t  *topology;
   //int            verbose_level = tm_get_verbose_level();
   tm_affinity_mat_t *aff_mat;
   //tm_solution_t  *sol;
   //TIC;
   //TIC;
   int j;
   /* Parse the arch file according to its type XML or TGT or Matrix*/
   if (arch_file_type==TM_FILE_TYPE_TGT || arch_file_type==TM_FILE_TYPE_XML){
      topology = tm_load_topology(arch_filename, arch_file_type);
   }
   else{
    topology                 = (tm_topology_t*)MALLOC(sizeof(tm_topology_t));
    topology->nb_constraints = 0;
    topology->oversub_fact   = 1;
    topology->constraints    = NULL;
    topology->nb_levels       = 1;
    topology->nb_nodes  = (size_t*) MALLOC(sizeof(size_t)*topology->nb_levels);
    topology->nb_nodes[topology->nb_levels - 1] = Net_Node[NB_CT];
    topology->node_rank      = (int*)MALLOC(sizeof(int)*Net_Node[NB_CT]);
    topology->node_id        = (int*)MALLOC(sizeof(int)*Net_Node[NB_CT]);
    for( j = 0 ; j < Net_Node[NB_CT] ; j++ ){
      topology->node_id[j]   = j;
      topology->node_rank[j] = j;
    }
    //topology->nb_levels      = atoi(strtok(s," "))+1;
    //topology->arity          = (int*)MALLOC(sizeof(int)*topology->nb_levels);
   }

   /* build affinity matrices and get the number of processes*/
   aff_mat = tm_load_aff_mat(com_filename);

   /* If we have binding constraints store them in the topology */
   // if (bind_flag) {
   //   if(!tm_topology_add_binding_constraints(bind_filename, topology))
   //     exit(-1);
   // }
   //double io_dur = TOC;

   // if(verbose_level == INFO)
   //   tm_display_arity(topology);
   // else if(verbose_level >= DEBUG)
   //   tm_display_topology(topology);

   //TIC;

   /* Manage the case where oversubscribing is requested */
   // if(oversub_fact>1){
   //   tm_enable_oversubscribing(topology, oversub_fact);
   // }

   /* optimize the toplogy in order to decompose arity of the tree into subtrees.
      Usualy it speeds up tm_build_tree_from_topology
    */
   // if(opt_topo_flag)
   //   tm_optimize_topology(&topology);
   //double    optimize_dur = TOC;

   // if(verbose_level == INFO)
   //   tm_display_arity(topology);
   // else if(verbose_level >= DEBUG)
   //   tm_display_topology(topology);

   /* The core of the algorithm: group processes according to the topology tree*/
   //TIC;
   // comm_tree = tm_build_tree_from_topology(topology, aff_mat, NULL, NULL);
   //double    build_tree_dur = TOC;

   //TIC;

   /* based on the build_tree_from_topology result, construct the perumtation and store it*/
   //sol = tm_compute_mapping(topology, comm_tree);

   //double    map_dur = TOC;
   //double    duration = TOC;

   //tm_verbose_printf(TIMING, "I/O:            %10.4f ms\n", io_dur * 1000);
   // if(verbose_level >= TIMING){
   //   printf("Optimize:       %10.4f ms\n", optimize_dur * 1000);
   //   printf("Building tree:  %10.4f ms\n", build_tree_dur * 1000);
   //   printf("Mapping:        %10.4f ms\n", map_dur * 1000);
   //   printf("Total duration: %10.4f ms\n", duration * 1000);
   // }

   /* display the solution. Warnig: use the oiginal topology not the optimized one*/
   //printf("TreeMatch: ");
   //tm_display_solution(topology, aff_mat, sol, metric);

   /* display the solution of other heuristics*/
   tm_display_other_heuristics(topology, aff_mat, metric);

   /* Free the allocated memory*/
   tm_free_topology(topology);
   //tm_free_tree(comm_tree);
   //tm_free_solution(sol);
   tm_free_affinity_mat(aff_mat);
   //tm_finalize();
   return 0;
}

void printUsage(char **argv){
  fprintf(stderr,
     "Usage: %s -t|x <Architecture file[tgt|xml]> -c <Communication pattern file> [-m evaluation metric (1: SUM_COM (default), 2: MAX_COM, 3: HOP_BYTE)] [-n force physical numbering (usefull only for XML/hwloc)] [-h display this help]\n",
     argv[0]);
}

int main(int argc, char **argv) {
  char            c;
  char            *arch_filename = NULL;
  char            *com_filename = NULL;
  //char            *bind_filename = NULL;
  //int             bind_flag = 0;
  //int             opt_topo_flag = 1;
  //int             verbose_level = ERROR;
  tm_file_type_t  arch_file_type = TM_FILE_TYPE_UNDEF;
  tm_metric_t     metric = TM_METRIC_SUM_COM;
  //int             oversub_fact = 1;
  Net_Node[NB_CT]=0;
  Net_Node[NB_NODE]=0;
  Net_Node[NB_CORE]=0;
  Net_Node[NB_FLAG]=0;

  //tm_set_exhaustive_search_flag(0);
  //tm_set_greedy_flag(0);
  //m_set_numbering(TM_NUMBERING_LOGICAL);

   while ((c = getopt(argc, argv, "nt:x:f:C:N:T:c:m:h")) != -1) {
      switch (c) {
         case 'h':
     printUsage(argv);
     exit(-1);
     //     case 'd':
     // opt_topo_flag = 0;
     // break;
     //     case 'g':
     // tm_set_greedy_flag(1);
     // break;
         case 'n':
     //tm_set_numbering(TM_NUMBERING_PHYSICAL);
     break;
     //     case 'e':
     // tm_set_exhaustive_search_flag(1);
     // break;
         //case 'p':
     //tm_set_max_nb_threads(atoi(optarg));
     //break;
   case 't':
      arch_filename = optarg;
      arch_file_type = TM_FILE_TYPE_TGT;
      break;
   case 'x':
      arch_filename = optarg;
      arch_file_type = TM_FILE_TYPE_XML;
      break;
   case 'f':
      arch_filename = optarg;
      Net_File = optarg;
      arch_file_type = TM_FILE_TYPE_MATRIX;
      Net_Node[NB_FLAG]=1;
      break;
   case 'T':
      Net_Node[NB_CT]=atoi(optarg);
      break;
   case 'N':
      Net_Node[NB_NODE]=atoi(optarg);
      break;
   case 'C':
      Net_Node[NB_CORE]=atoi(optarg);
      break;
   case 'c':
      com_filename = optarg;
      break;
   // case 'b':
   //    bind_flag = 1;
   //    bind_filename = optarg;
      // break;
   // case 'v':
   //   verbose_level  = atoi(optarg);
   //   break;
   case 'm':
     metric  = atoi(optarg);
     break;
     //     case 'o':
     // oversub_fact = atoi(optarg);
     //       break;
   case '?':
      if (isprint(optopt))
         fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
         fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);

      printUsage(argv);
      abort();

   default:
      printUsage(argv);
      abort();
      }
   }

   if (!arch_filename || !com_filename) {
      fprintf(stderr,"Error: Missing \"Architecture file\" and/or \"Communication pattern file\"!\n");
      printUsage(argv);
      abort();
   }

   //tm_set_verbose_level(verbose_level);
   return test_mapping(arch_filename, arch_file_type, com_filename, metric);

}
