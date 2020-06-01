#include "tm_config.h"
#include "tm_scotch.h"
#include "tm_verbose.h"
#include "tm_mapping.h"
#include "tm_kpartitioning.h"
#include <stdlib.h>
#include <stdio.h>




#if HAVE_LIBSCOTCH
#include <scotch.h>
#endif


#if HAVE_LIBSCOTCH


//
int *convarray_scotchnum_to_int(SCOTCH_Num * array_in , SCOTCH_Num nsc);
void fill_k(int **k , int *sigma , int oversub , int nb_proccess);
int max_oversub(int *sigma, int size , int nb_proc);
//

int *convarray_scotchnum_to_int(SCOTCH_Num * array_in , SCOTCH_Num nsc){
  int ni=(int) nsc;
  int i;
  int *res= MALLOC (ni*sizeof(int));
  for (i=0 ; i<ni ; i++){
    res[i]=(int)(array_in[i]);
  }
  return res;
}


SCOTCH_Graph* com_mat_to_scotch_graph(com_mat_t *com_mat, int n){
  double **mat = com_mat->comm;
  SCOTCH_Num vertnbr = n;// number of vertices
  SCOTCH_Num edgenbr = vertnbr*vertnbr;// number of edges
  /* adjacency list */
  SCOTCH_Num *verttab = (SCOTCH_Num *)malloc(sizeof(SCOTCH_Num) * (vertnbr+1));
  /* loads of vertices */
  /* SCOTCH_Num *velotab = (SCOTCH_Num *)malloc(sizeof(SCOTCH_Num) * vertnbr); */
  /* id of the neighbors */
  SCOTCH_Num *edgetab = (SCOTCH_Num *)malloc(sizeof(SCOTCH_Num) * edgenbr);
  /* number of bytes exchanged */
  SCOTCH_Num *edlotab = (SCOTCH_Num *)malloc(sizeof(SCOTCH_Num) * edgenbr);
  SCOTCH_Graph *graphptr =  SCOTCH_graphAlloc();

  int edgeNum = 0;
  int i,j;

  if(n < com_mat->n){
    tm_verbose_fprintf(CRITICAL ,verbose_stderr,"Not enough slots (%d), for the number of processes (%d).\n Cannot continue.\n Use oversubscrbing to overcome this issue\n",n, com_mat->n);
    exit(-1);
  }
  /* Building with the communication matrix */
  for(i = 0; i < com_mat->n ; i++) {
    verttab[i] = edgeNum;
    for(j = 0; j < i; j++) {
      if(mat[i][j]){
	edgetab[edgeNum] = j;
	edlotab[edgeNum] = (SCOTCH_Num)mat[i][j];
	edgeNum++;
      }
    }
    /* ensure i!=j. Hence, avoid to test it...*/
    for(j = i+1 ; j < com_mat->n ; j++) {
      if(mat[i][j]){
	edgetab[edgeNum] = j;
	edlotab[edgeNum] = (SCOTCH_Num)mat[i][j];
	edgeNum++;
      }
    }
  }

  /* adding the dumb vertices: they have no neighbor*/
  for(i = com_mat->n ; i<vertnbr ; i++) {
    verttab[i] = edgeNum;
  }

  verttab[i] = edgeNum;
  tm_verbose_fprintf(INFO ,verbose_stdout,"Graph converted to Scotch format: edgeNum=%d, edgenbr = %lld, vertnbr = %lld\n",edgeNum, (long long int)edgenbr, (long long int)vertnbr);


  assert(edgeNum <= edgenbr);
  edgenbr = edgeNum;

  SCOTCH_graphInit(graphptr);
  SCOTCH_graphBuild(graphptr, 0, vertnbr, verttab, verttab+1, NULL, NULL, edgenbr, edgetab, edlotab);

  return graphptr;
}


int find_oversub(int *sigma, int size , int nb_proc){
  int i;
  int max;
  int *tab_nb_affect = (int*)CALLOC(nb_proc, sizeof(int));

  for(i=0 ; i<size ; i++){
    tab_nb_affect[sigma[i]]++;
    if(sigma[i] > nb_proc){
      tm_verbose_fprintf(CRITICAL, verbose_stderr, "Error: sigma[%d]=%d is larger than number of processing unit  (%d)!\nExiting!\n",i, sigma[i], nb_proc);
      exit(-1);
    }
  }

  max = tab_nb_affect[0];
  for(i=1 ; i<nb_proc ; i++){
    if (tab_nb_affect[i]>max){
      max=tab_nb_affect[i];
    }
  }

  FREE(tab_nb_affect);
  return max;
}


void fill_k(int **k , int *sigma , int oversub , int nb_proccess){
  int i;
  for(i=0 ; i<nb_proccess ; i++){
    set_val(k[sigma[i]], i, oversub);
  }
}


SCOTCH_Num *scotch_partitionning(SCOTCH_Arch archdat, tm_affinity_mat_t *aff_mat, SCOTCH_Num *parttab_len, int oversub_fact){
  com_mat_t *           com_mat=(com_mat_t *)MALLOC(sizeof(com_mat_t));
  SCOTCH_Graph          *graphptr;
  SCOTCH_Strat          strat;
  SCOTCH_Mapping        mappdat;
  SCOTCH_Num            straval;
  SCOTCH_Num * restrict parttab;
  SCOTCH_Num            vertnbr;
  size_t                nb_processes;
  size_t                arch_size;
  
  /* verifier le nombre de contraintes et appeler subarch pour les prendre en compte*/
  arch_size = SCOTCH_archSize(&archdat);
  nb_processes  = aff_mat->order;
  com_mat->comm = aff_mat->mat;
  com_mat->n    = nb_processes;
  graphptr = com_mat_to_scotch_graph(com_mat, arch_size * oversub_fact);
  
  SCOTCH_graphSize (graphptr, &vertnbr, NULL);
  tm_verbose_fprintf(INFO, verbose_stdout, "Comm mat converted to Scotch graph (vertnb=%ld)\n",vertnbr);

  SCOTCH_stratInit (&strat);
  straval = SCOTCH_STRATBALANCE;
  SCOTCH_stratGraphMapBuild (&strat, straval, vertnbr, 0);
  
  tm_verbose_fprintf(INFO, verbose_stdout, "Scotch strategy done\n");

  parttab = MALLOC (vertnbr * sizeof (SCOTCH_Num));
  SCOTCH_graphMapInit (graphptr, &mappdat, &archdat, parttab);
  tm_verbose_fprintf(INFO, verbose_stdout,"Scotch mapping init done\n");
  SCOTCH_graphMapCompute (graphptr, &mappdat, &strat);
  tm_verbose_fprintf(INFO, verbose_stdout,"Scotch mapping computed\n");

  *parttab_len = vertnbr;
  FREE(com_mat);
  SCOTCH_graphMapExit(graphptr,&mappdat);
  SCOTCH_graphExit(graphptr);
  SCOTCH_stratExit(&strat);
  
  return parttab;
  
}


/* int int_cmp_inc(const void* x1,const void* x2) */
/* { */
/*   return *((int *)x1) < *((int *)x2) ? -1 : 1; */
/* } */


tm_solution_t *build_mapping_with_scotch(tm_topology_scotch_t *topology, tm_affinity_mat_t *aff_mat) {
  unsigned      nb_constraints;
  unsigned      nb_processes, npu;
  int           result;
  unsigned      i;
  SCOTCH_Num *  constraints;
  SCOTCH_Num    *parttab;
  SCOTCH_Num    parttab_len;
  SCOTCH_Arch   sub_arch;
  int           j, oversub_real = -1, oversub_max;
  tm_solution_t *solution =  (tm_solution_t *)MALLOC(sizeof(tm_solution_t));
  int           *sigma, **k;
  size_t        sigma_length;
  
  nb_constraints = topology->common->nb_constraints;
  nb_processes   = aff_mat->order;
  npu = SCOTCH_archSize(&topology->arch);

  if(verbose_level >= INFO){
    tm_verbose_fprintf(INFO, verbose_stdout, "Com matrix size      : %d\n", nb_processes);
    tm_verbose_fprintf(INFO, verbose_stdout, "nb_constraints       : %d\n", nb_constraints);
    if(topology->common->constraints)
      print_1D_tab(topology->common->constraints, nb_constraints);
    tm_verbose_fprintf(INFO, verbose_stdout, "nb_processing_units : %d\n", npu);
  }
  oversub_max = topology->common->oversub_fact;

  if(nb_constraints && nb_processes > nb_constraints * oversub_max){
    tm_verbose_fprintf(CRITICAL,verbose_stderr, "Error : Not enough constraints (%d) for the communication matrix order (%d)!\n",nb_constraints, nb_processes);
    exit(-1);
  }

  if((topology->common->constraints == NULL) && (nb_constraints != 0)){
    tm_verbose_fprintf(ERROR,verbose_stderr,"size of constraint table not zero while constraint tab is NULL\n");
    exit(-1);;
  }

  if((topology->common->constraints != NULL) && (nb_constraints > npu)){
    tm_verbose_fprintf(ERROR,verbose_stderr,"size of constraint table (%d) is greater than the number of cores (%d)\n", nb_constraints, npu);
    exit(-1);;
  }

  
  if(nb_constraints){
    tm_verbose_fprintf(INFO,verbose_stdout, "Scotch Partitionning with constraints\n");  
    
    constraints = (SCOTCH_Num*)MALLOC(sizeof(SCOTCH_Num)*nb_constraints);
    for (i = 0; i < nb_constraints; i++){
      constraints[i] = (SCOTCH_Num)topology->common->constraints[i];    
    }
    
    /* qsort(constraints, nb_constraints, sizeof(SCOTCH_Num),int_cmp_inc); */
    SCOTCH_archInit (&sub_arch);
    result = SCOTCH_archSub(&sub_arch, &topology->arch, (SCOTCH_Num)nb_constraints, constraints);
    if (result){
      tm_verbose_fprintf(CRITICAL, verbose_stderr, "The sub-architecture wasn't built\n");
      exit(-1);
    }else{
      tm_verbose_fprintf(INFO,verbose_stdout, "Subarch built\n");
      parttab = scotch_partitionning(sub_arch, aff_mat, &parttab_len, oversub_max);
      /* re number parttab according to the constraints*/
      for(i=0; i < parttab_len ; i++)
	parttab[i] = constraints[parttab[i]];
      SCOTCH_archExit(&sub_arch);
      FREE(constraints);
    }
  }else{
    tm_verbose_fprintf(INFO,verbose_stdout, "Scotch Partitionning without constraints\n");
    parttab = scotch_partitionning(topology->arch, aff_mat, &parttab_len,  topology->common->oversub_fact);
  }
  
  nb_processes = aff_mat -> order;
  sigma_length = (size_t) nb_processes;
  /* We want only the nb_processes first elements of partab*/
  sigma        = convarray_scotchnum_to_int(parttab, nb_processes); 
  k            = (int**) MALLOC(sizeof(int*) * npu);
  oversub_real  = find_oversub(sigma, nb_processes, npu);

  if(oversub_real > oversub_max){
    tm_verbose_fprintf(ERROR, verbose_stderr, "Scotch oversubsribed the architecture by a factor of %d while the maximum factor is set to %d.\nYou can fix it by increasing the oversubscribing factor to %d (e.g. through -o option, tm_enable_oversubscribing function)\nExiting.\n",oversub_real, oversub_max, oversub_real);
    exit(-1);
  }
  

  for (i=0 ; i < npu ; i++){
    k[i] =  (int*) MALLOC(sizeof(int) * oversub_real);
    for (j=0 ; j < oversub_real ; j++){
      k[i][j]=-1;
    }
  }
  fill_k(k, sigma, oversub_real, sigma_length);

  tm_verbose_fprintf(INFO, verbose_stdout, "Solution using Scotch computed\n");


  solution->sigma         = sigma;
  solution->sigma_length  = sigma_length;
  solution->k             = k;
  solution->k_length      = npu;
  solution->oversub_fact  = oversub_real;
  FREE(parttab);
  return solution;
  
}

#endif
