#include "tm_config.h"
#include <hwloc.h>
#include <hwloc/helper.h>
#include "tm_tree.h"
#include "tm_mapping.h"
#include <ctype.h>
#include "tm_verbose.h"
#include "tm_solution.h"


//
void build_process_tab_id(tm_topology_tree_t *topology,  hwloc_obj_t *objs, char* filename);
int is_tleaf(char* filename);
double link_cost(int depth);
int tm_topology_set_binding_constraints(int *constraints, int nb_constraints, tm_topology_t *topology);
int tm_topology_set_binding_constraints_cpy(int *constraints, int nb_constraints, tm_topology_t *topology, int cpy_flag);
int topo_check_constraints(tm_topology_t *topology);
//
tm_topology_t* tm_get_local_topo_with_hwloc(void);
tm_topology_tree_t* hwloc_to_tm(char *filename);
int int_cmp_inc(const void* x1,const void* x2);
void optimize_arity(int **arity, double **cost, int *nb_levels,int n);
int symetric(hwloc_topology_t topology);
tm_topology_tree_t * tleaf_to_tm(char *filename);
void tm_display_arity(tm_topology_t *topology);
void tm_display_topology(tm_topology_t *topology);
void tm_free_topology(tm_topology_t *topology);
tm_topology_t *tm_load_topology(char *arch_filename, tm_file_type_t arch_file_type);
void tm_optimize_topology(tm_topology_t **topology);
int  tm_topology_add_binding_constraints(char *constraints_filename, tm_topology_t *topology);
int topo_nb_proc(hwloc_topology_t topology,int N);
void topology_arity_cpy(tm_topology_tree_t *topology,int **arity,int *nb_levels);
void topology_constraints_cpy(tm_topology_tree_t *topology,int **constraints,int *nb_constraints);
void topology_cost_cpy(tm_topology_tree_t *topology,double **cost);
void topology_numbering_cpy(tm_topology_tree_t *topology,int **numbering,int *nb_nodes);
double ** topology_to_arch(hwloc_topology_t topology);
void   build_synthetic_proc_id(tm_topology_tree_t *topology);
tm_topology_t  *tm_build_synthetic_topology(int *arity, double *cost, int nb_levels, int *core_numbering, int nb_core_per_nodes);
void            tm_set_numbering(tm_numbering_t new_val); /* TM_NUMBERING_LOGICAL or TM_NUMBERING_PHYSICAL */
tm_numbering_t  tm_get_numbering(); /* TM_NUMBERING_LOGICAL or TM_NUMBERING_PHYSICAL */
int nb_processing_units_tree(tm_topology_tree_t *topology);
#if HAVE_LIBSCOTCH
SCOTCH_Arch tscotch_to_tm(char *arch_filename);
#endif
 
#define LINE_SIZE (1000000)

static tm_mapping_strat_t mapping_strat = TM_STRAT_TM;


void  tm_set_mapping_strat(tm_mapping_strat_t strat){
  if(strat > TM_STRAT_SCOTCH){
    tm_verbose_fprintf(ERROR,verbose_stderr,"Unknown strategy %u for option -l!\n",strat);
    tm_verbose_fprintf(ERROR,verbose_stderr,"Falling back to TreeMatch strategy!\n");
    mapping_strat = TM_STRAT_TM;
    return;
  }
  #ifdef HAVE_LIBSCOTCH
  mapping_strat = strat;
  #else
  if(strat ==  TM_STRAT_SCOTCH){
    tm_verbose_fprintf(ERROR,verbose_stderr,"Scotch is not linked: Cannot force Scotch!\n",strat);
    tm_verbose_fprintf(ERROR,verbose_stderr,"Falling back to TreeMatch strategy!\n");
    mapping_strat = TM_STRAT_TM;
  }else{
    mapping_strat = strat;
  }
  #endif
}

tm_mapping_strat_t  tm_get_strat(){
  return mapping_strat;
}

static tm_numbering_t numbering = TM_NUMBERING_LOGICAL;

void tm_set_numbering(tm_numbering_t new_val){
    numbering = new_val;
}

/* Not Called */
tm_numbering_t  tm_get_numbering(){
  return numbering;
}





/* transform a tgt scotch file into a topology file*/
tm_topology_tree_t * tleaf_to_tm(char *filename)
{
  tm_topology_tree_t   *topology = NULL;
  tm_common_topology_t *common_topology = NULL;
  FILE *pf = NULL;
  char line[1024];
  char *s = NULL;
  double *cost = NULL;
  int i;



  pf = fopen(filename,"r");
  if(!pf){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  tm_verbose_fprintf(INFO,verbose_stdout, "Reading TGT file: %s\n",filename);


  fgets(line,1024,pf);
  fclose(pf);

  s = strstr(line,"tleaf");
  if(!s){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Syntax error! %s is not a tleaf file\n",filename);
    
    exit(-1);
  }

  s += 5;
  while(isspace(*s))
    s++;

  topology                                  = (tm_topology_tree_t*)MALLOC(sizeof(tm_topology_tree_t));
  common_topology                           = (tm_common_topology_t*)MALLOC(sizeof(tm_common_topology_t));
  common_topology->nb_constraints           = 0;
  common_topology->oversub_fact             = 1;
  common_topology->constraints              = NULL;
  topology->common 		            = common_topology;
  topology->nb_levels                       = atoi(strtok(s," "))+1;
  topology->arity                           = (int*)MALLOC(sizeof(int)*topology->nb_levels);

  cost = (double*)CALLOC(topology->nb_levels,sizeof(double));

  for( i = 0 ; i < topology->nb_levels-1 ; i++ ){
    topology->arity[i] = atoi(strtok(NULL," "));
    cost[i] = atoi(strtok(NULL," "));
  }

  topology->arity[topology->nb_levels-1] = 0;
  /* cost[topology->nb_levels-1]=0; */

  /*aggregate costs*/
  for( i = topology->nb_levels-2 ; i >= 0 ; i-- )
    cost[i] += cost[i+1];

  build_synthetic_proc_id(topology);

  tm_verbose_fprintf(INFO,verbose_stdout,"Topology built from %s!\n",filename);
  topology->common->cost=cost;


  return topology;
}

int nb_processing_units_tree(tm_topology_tree_t *topology){
  return topology->common->nb_proc_units;
}

/* Not Called */
int nb_processing_units(tm_topology_t *topology)
{
  if (topology->topology_type == TM_TOPOLOGY_TREE){
    return nb_processing_units_tree(topology->topology.tree);
  }else if (topology->topology_type == TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
    return SCOTCH_archSize(&(topology->topology.scotch->arch));
#else
    return -1;
#endif
  }else{
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Unknown toplogy type to compute nb_processing unit line %d in file %s\n",__LINE__, __FILE__);
    exit(-1);
  }
}

/* Not Called */
int topo_nb_proc(hwloc_topology_t topology,int N)
{
  hwloc_obj_t *objs = NULL;
  int nb_proc;

  objs = (hwloc_obj_t*)MALLOC(sizeof(hwloc_obj_t)*N);
  objs[0] = hwloc_get_next_obj_by_type(topology,HWLOC_OBJ_PU,NULL);
  nb_proc = 1 + hwloc_get_closest_objs(topology,objs[0],objs+1,N-1);
  FREE(objs);
  return nb_proc;
}



double link_cost(int depth)
{
  /*
    Bertha values
    double tab[5]={21,9,4.5,2.5,0.001};
    double tab[5]={1,1,1,1,1};
    double tab[6]={100000,10000,1000,500,100,10};
  */
  double tab[11] = {1024,512,256,128,64,32,16,8,4,2,1};

  return tab[depth];
  /*
    return 10*log(depth+2);
    return (depth+1);
    return (long int)pow(100,depth);
  */
}

/* Not Called */
double ** topology_to_arch(hwloc_topology_t topology)
{
  unsigned nb_proc ,i ,j;
  hwloc_obj_t obj_proc1,obj_proc2,obj_res;
  double **arch = NULL;

  nb_proc = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU);
  arch = (double**)MALLOC(sizeof(double*)*nb_proc);
  for( i = 0 ; i < nb_proc ; i++ ){
    obj_proc1 = hwloc_get_obj_by_type(topology,HWLOC_OBJ_PU,i);
    arch[obj_proc1->os_index] = (double*)MALLOC(sizeof(double)*nb_proc);
    for( j = 0 ; j < nb_proc ; j++ ){
      obj_proc2 = hwloc_get_obj_by_type(topology,HWLOC_OBJ_PU,j);
      obj_res = hwloc_get_common_ancestor_obj(topology,obj_proc1,obj_proc2);
      /* printf("arch[%d][%d] <- %ld\n",obj_proc1->os_index,obj_proc2->os_index,*((long int*)(obj_res->userdatab))); */
      arch[obj_proc1->os_index][obj_proc2->os_index]=link_cost(obj_res->depth+1);
    }
  }
  return arch;
}

int symetric(hwloc_topology_t topology)
{
  int depth,i,topodepth = hwloc_topology_get_depth(topology);
  unsigned int arity;
  hwloc_obj_t obj;
  for ( depth = 0; depth < topodepth-1 ; depth++ ) {
    int N = hwloc_get_nbobjs_by_depth(topology, depth);
    obj = hwloc_get_next_obj_by_depth (topology,depth,NULL);
    arity = obj->arity;

    /* printf("Depth=%d, N=%d, Arity:%d\n",depth,N,arity); */
    for (i = 1; i < N; i++ ){
      obj = hwloc_get_next_obj_by_depth (topology,depth,obj);
      if( obj->arity != arity){
	/* printf("[%d]: obj->arity=%d, arity=%d\n",i,obj->arity,arity); */
	return 0;
      }
    }
  }
  return 1;
}

void build_process_tab_id(tm_topology_tree_t *topology,  hwloc_obj_t *objs, char* filename){
  unsigned int i,j;
  unsigned int nb_nodes = topology->common->nb_proc_units; 
  
  /* Build process id tab */
  if(numbering == TM_NUMBERING_LOGICAL){
    for (i = 0; i < nb_nodes; i++){
      topology->node_id[i] = i;
      topology->node_rank[i] = i;
    }
  }else if(numbering == TM_NUMBERING_PHYSICAL){
    for (i = 0; i < nb_nodes; i++){
      if(objs[i]->os_index > nb_nodes){
	tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot use forced physical numbering!\n\tIndex of PU %d is %d and larger than number of nodes : %d\n", i, objs[i]->os_index, nb_nodes);
	exit(-1);
      }
      for(j = 0; j < i; j++){
	if((unsigned int)topology->node_id[j] == objs[i]->os_index){
	  tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot use forced physical numbering!\n\tDuplicated physical number of some PUs in %s.\n\tPU %d and PU %d have the same physical number: (os_index[%d] = %d) == (os_index[%d] = %d)\n", filename, j, i, j, objs[j]->os_index, i, objs[i]->os_index);
	  exit(-1);
	}
      }
      topology->node_id[i] = objs[i]->os_index;
      topology->node_rank[objs[i]->os_index] = i;
    }
  }else{
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Unknown numbering %d\n", (int)numbering);
    exit(-1);
  }
}


tm_topology_tree_t* hwloc_to_tm(char *filename)
{
  hwloc_topology_t topology;
  tm_topology_tree_t *tree_topology = NULL;
  tm_common_topology_t *common_topology = NULL;
  hwloc_obj_t *objs = NULL;
  unsigned topodepth,depth;
  unsigned int nb_nodes;
  double *cost;
  int err, l;

  /* Build the topology */
  hwloc_topology_init(&topology);
  err = hwloc_topology_set_xml(topology, filename);
  if(err == -1){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error: %s is a bad xml topology file!\n",filename);
    exit(-1);
  }

#if HWLOC_API_VERSION < 0x20000
  hwloc_topology_ignore_all_keep_structure(topology);
#else
  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
#endif

  err = hwloc_topology_load(topology);
  if(err == -1){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error: the content of the xml topology file %s is not compatible with the version installed on this machine.\nPlease use compatible versions to generate the file and to use it!\n",filename);
    exit(-1);
  }


  /* Test if symetric */
  if(!symetric(topology)){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"%s not symetric!\n",filename);
    exit(-1);
  }

  /* work on depth */
  topodepth = hwloc_topology_get_depth(topology);
  
  tree_topology                                = (tm_topology_tree_t*)MALLOC(sizeof(tm_topology_tree_t));
  common_topology                              = (tm_common_topology_t*)MALLOC(sizeof(tm_common_topology_t));
  common_topology->oversub_fact                = 1;
  common_topology->nb_constraints              = 0;
  common_topology->constraints                 = NULL;
  tree_topology->common                        = common_topology;
  tree_topology->nb_levels                     = topodepth;
  tree_topology->nb_nodes                      = (size_t*)MALLOC(sizeof(size_t)*tree_topology->nb_levels);
  tree_topology->arity                         = (int*)MALLOC(sizeof(int)*tree_topology->nb_levels);

  tm_verbose_fprintf(INFO,verbose_stdout,"topodepth = %d\n",topodepth);

  /* Build TreeMatch topology */
  for( depth = 0 ; depth < topodepth ; depth++ ){
    nb_nodes = hwloc_get_nbobjs_by_depth(topology, depth);
    tree_topology->nb_nodes[depth] = nb_nodes;

    objs    = (hwloc_obj_t*)MALLOC(sizeof(hwloc_obj_t)*nb_nodes);
    objs[0] = hwloc_get_next_obj_by_depth(topology, depth, NULL);
    hwloc_get_closest_objs(topology, objs[0], objs+1, nb_nodes-1);
    tree_topology->arity[depth] = objs[0]->arity;
 
    tm_verbose_fprintf(DEBUG,verbose_stdout,"\n--%d(%d) **%d**:--\n",tree_topology->arity[depth],nb_nodes,tree_topology->arity[0]);

    
    if (depth == topodepth -1){
      tree_topology->common->nb_constraints = nb_nodes;
      tree_topology->common->nb_proc_units  = nb_nodes;
      tree_topology->node_id        = (int*)MALLOC(sizeof(int)*nb_nodes);
      tree_topology->node_rank      = (int*)MALLOC(sizeof(int)*nb_nodes);
   
      build_process_tab_id(tree_topology, objs, filename);
     
    }
    FREE(objs);


  }

  cost = (double*)CALLOC(tree_topology->nb_levels,sizeof(double));
  for(l=0; l<tree_topology->nb_levels; l++){
    cost[l] = link_cost(l);
  }
  tree_topology->common->cost = cost;


  /* Destroy topology object. */
  hwloc_topology_destroy(topology);
  tm_verbose_fprintf(INFO,verbose_stdout,"\n");


  return tree_topology;
}

/* Not Called */
tm_topology_t* tm_get_local_topology_with_hwloc(void)
{
  hwloc_topology_t topology;
  tm_topology_t *res = NULL;
  tm_topology_tree_t *tree_topology = NULL;
  tm_common_topology_t *common_topology = NULL;
  hwloc_obj_t *objs = NULL;
  unsigned topodepth,depth;
  int nb_nodes;

  /* Build the topology */
  hwloc_topology_init(&topology);

#if HWLOC_API_VERSION < 0x20000
  hwloc_topology_ignore_all_keep_structure(topology);
#else
  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
#endif

  hwloc_topology_load(topology);

  /* Test if symetric */
  if(!symetric(topology)){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Local toplogy not symetric!\n");
    exit(-1);
  }

  /* work on depth */
  topodepth = hwloc_topology_get_depth(topology);

  res                              = (tm_topology_t*)MALLOC(sizeof(tm_topology_t));
  tree_topology                    = (tm_topology_tree_t*)MALLOC(sizeof(tm_topology_tree_t));
  common_topology                  = (tm_common_topology_t*)MALLOC(sizeof(tm_common_topology_t));
  common_topology->nb_constraints  = 0;
  common_topology->constraints     = NULL;
  common_topology->oversub_fact    = 1; //defaut
  common_topology->cost            = NULL; 
  tree_topology->common            = common_topology;
  tree_topology->nb_levels         = topodepth;
  tree_topology->nb_nodes          = (size_t*)MALLOC(sizeof(size_t)*tree_topology->nb_levels);
  tree_topology->arity             = (int*)MALLOC(sizeof(int)*tree_topology->nb_levels);
  res->topology_type               = TM_TOPOLOGY_TREE;
  res->topology.tree               = tree_topology;
  

  /* Build TreeMatch topology */
  for( depth = 0 ; depth < topodepth ; depth++ ){
    nb_nodes = hwloc_get_nbobjs_by_depth(topology, depth);
    res->topology.tree->nb_nodes[depth] = nb_nodes;

    objs = (hwloc_obj_t*)MALLOC(sizeof(hwloc_obj_t)*nb_nodes);
    objs[0] = hwloc_get_next_obj_by_depth(topology,depth,NULL);
    hwloc_get_closest_objs(topology,objs[0],objs+1,nb_nodes-1);
    res->topology.tree->arity[depth] = objs[0]->arity;

    if (depth == topodepth -1){
      res->topology.tree->common->nb_constraints = nb_nodes;
      res->topology.tree->common->nb_proc_units  = nb_nodes;
      res->topology.tree->node_id        = (int*)MALLOC(sizeof(int)*nb_nodes);
      res->topology.tree->node_rank      = (int*)MALLOC(sizeof(int)*nb_nodes);
      /* printf("%d:",res->arity[depth]); */

      /* Build process id tab */

      build_process_tab_id(res->topology.tree, objs, "Local node topology");
    }
    FREE(objs);
  }



  /* Destroy HWLOC topology object. */
  hwloc_topology_destroy(topology);

  /* printf("\n"); */
  return res;
}


void tm_free_topology(tm_topology_t *topology)
{

  if (topology->topology_type == TM_TOPOLOGY_TREE){
    FREE(topology->topology.tree->node_id);
    FREE(topology->topology.tree->node_rank);
    FREE(topology->topology.tree->common->constraints);
    FREE(topology->topology.tree->nb_nodes);
    FREE(topology->topology.tree->arity);
    FREE(topology->topology.tree->common->cost);
    FREE(topology->topology.tree->common);
    FREE(topology->topology.tree);
    FREE(topology);
  }else if(topology->topology_type == TM_TOPOLOGY_SCOTCH){
#ifdef HAVE_LIBSCOTCH
    FREE(topology->topology.scotch->common->constraints);
    FREE(topology->topology.scotch->common);
    SCOTCH_archExit(&(topology->topology.scotch->arch));
#endif
    FREE(topology->topology.scotch);
    FREE(topology);
  }
}


/*return true if file is a tleaf*/
int is_tleaf(char* filename){
  FILE  *pf;
  char string[1024];
  char* istleaf;

  pf = fopen(filename, "r");
  if(!pf){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error: cannot open %s\n", filename);
    exit(-1);
  }
  fgets(string , 1024 , pf);
  istleaf = strstr(string , "tleaf");
  fclose (pf);

  
  
  if (istleaf==NULL){
    tm_verbose_fprintf(DEBUG,verbose_stdout,"%s is not a tleaf\n", filename);
    
    return 0;
  } else{
    tm_verbose_fprintf(DEBUG,verbose_stdout,"%s is a tleaf\n", filename);
    return 1;
  }
}

tm_topology_t *tm_load_topology(char *arch_filename, tm_file_type_t arch_file_type){
  tm_topology_t *topology;
  tm_topology_tree_t * topology_tree;
 #if HAVE_LIBSCOTCH
  tm_topology_scotch_t * topology_scotch;
#endif

  switch(arch_file_type){
  case   TM_FILE_TYPE_TGT:
    tm_verbose_fprintf(DEBUG,verbose_stdout,"Reading %s as a TGT file\n", arch_filename);
    topology =(tm_topology_t*)MALLOC(sizeof(tm_topology_t));
    if (tm_get_strat()==TM_STRAT_SCOTCH ||!is_tleaf(arch_filename)){

#if HAVE_LIBSCOTCH
      SCOTCH_Arch arch;
      tm_common_topology_t *common_topology = NULL; //
      tm_verbose_fprintf(DEBUG,verbose_stdout,"Storing topology in Scotch format\n");
      arch                                      = tscotch_to_tm(arch_filename);
      topology_scotch                           = (tm_topology_scotch_t*)MALLOC(sizeof(tm_topology_scotch_t));
      topology_scotch->arch                     = arch;
      common_topology                           = (tm_common_topology_t*)MALLOC(sizeof(tm_common_topology_t)); //
      common_topology->nb_constraints           = 0; //
      common_topology->oversub_fact             = 1; //
      common_topology->constraints              = NULL; //
      topology_scotch->common                   = common_topology; //
      topology->topology.scotch                 = topology_scotch;
      topology->topology_type                   = TM_TOPOLOGY_SCOTCH;
#else
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error:\tTrying to use a Scotch feature (e.g. a non-tleaf file) while this library is not linked with Scotch.\n\tWithout Scotch, this library is only able to read tleaf files.\n\tExiting!\n");
      exit(-1);
#endif
    }
    else{
      tm_verbose_fprintf(DEBUG,verbose_stdout,"Storing topology in TreeMatch format\n");
      topology_tree = tleaf_to_tm(arch_filename);    
      topology -> topology_type = TM_TOPOLOGY_TREE;
      topology -> topology.tree = topology_tree;
    }
    return topology;
  case TM_FILE_TYPE_XML:
    tm_verbose_fprintf(DEBUG,verbose_stdout,"Reading %s as an XML file\n",arch_filename);
    topology=(tm_topology_t*)MALLOC(sizeof(tm_topology_t));
    topology_tree = hwloc_to_tm(arch_filename);    
    topology -> topology_type = TM_TOPOLOGY_TREE;
    topology -> topology.tree = topology_tree;
    return topology;
  default:
    tm_verbose_fprintf(ERROR,verbose_stderr,"Error loading topology. Filetype %d unknown\n", arch_file_type);
    exit(-1);
  }
}

/* Not Called */
void tm_display_topology(tm_topology_t *topology){
  int i;
  unsigned long  id;

  if(topology->topology_type==TM_TOPOLOGY_TREE){
    for( i = 0 ; i < topology->topology.tree->nb_levels ; i++ ){
          tm_verbose_fprintf(DEBUG, verbose_stdout, "Level %d with arity %d\n", i, topology->topology.tree->arity[i]);
    }
    
    tm_verbose_fprintf(DEBUG, verbose_stdout, "Last level: ");
    for(id = 0; id < topology->topology.tree->nb_nodes[topology->topology.tree->nb_levels-1]/topology->topology.tree->common->oversub_fact; id++)
      fprintf(*verbose_stdout, "%d ",topology->topology.tree->node_rank[id]);
    fprintf(*verbose_stdout, "\n");

    
    if(topology->topology.tree->common->constraints){
      tm_verbose_fprintf(DEBUG, verbose_stdout, "Constraints: ");
      for(i = 0; i < topology->topology.tree->common->nb_constraints; i++)
	fprintf(*verbose_stdout, "%d ",topology->topology.tree->common->constraints[i]);
      fprintf(*verbose_stdout, "\n");
    }
    
    tm_verbose_fprintf(DEBUG, verbose_stdout, "nb_levels=%d\n",       topology->topology.tree->nb_levels);
    tm_verbose_fprintf(DEBUG, verbose_stdout, "nb_constraints=%d\n",  topology->topology.tree->common->nb_constraints);
    tm_verbose_fprintf(DEBUG, verbose_stdout, "oversub_fact=%d\n",    topology->topology.tree->common->oversub_fact);
    tm_verbose_fprintf(DEBUG, verbose_stdout, "nb proc units=%d\n\n", topology->topology.tree->common->nb_proc_units);
	  
  }
}


void tm_display_arity(tm_topology_t *topology){
  int depth;
  int level;
  level = verbose_level>DEBUG? DEBUG : verbose_level;
  
  if(topology->topology_type==TM_TOPOLOGY_TREE){
    tm_verbose_fprintf(level, verbose_stdout, "");
    for(depth=0; depth < topology->topology.tree->nb_levels; depth++){
      fprintf(*verbose_stdout, "%d",topology->topology.tree->arity[depth]);
      if(topology->topology.tree->common->cost)
	fprintf(*verbose_stdout, "(%lf)",topology->topology.tree->common->cost[depth]);
      else
	fprintf(*verbose_stdout, ":");
    }
    fprintf(*verbose_stdout, "\n");
  }
}

int int_cmp_inc(const void* x1,const void* x2)
{
  return *((int *)x1) < *((int *)x2) ? -1 : 1;
}


int topo_check_constraints(tm_topology_t *topology){
  int i, size, depth, n;
  if (topology->topology_type == TM_TOPOLOGY_TREE){
    depth = topology->topology.tree->nb_levels-1;
    n = topology->topology.tree->common->nb_constraints;
    for (i=0;i<n;i++){
      if(!in_tab(topology->topology.tree->node_id, topology->topology.tree->nb_nodes[depth], topology->topology.tree->common->constraints[i])){
	tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error! Incompatible constraint with the topology: rank %d in the constraints is not a valid id of any nodes of the topology.\n",topology->topology.tree->common->constraints[i]);
	return 0;
      }
    }
    return 1;
  }

  else if (topology->topology_type == TM_TOPOLOGY_SCOTCH){
#if HAVE_LIBSCOTCH
    size = SCOTCH_archSize(&topology->topology.scotch->arch);
    n = topology->topology.scotch->common->nb_constraints;
    if (topology->topology.scotch->common->constraints[n-1] >= size) {
      tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error! Incompatible constraint with the topology: rank %d in the constraints is greater than the topology's size(%d).\n",topology->topology.scotch->common->constraints[n-1], size);
      return 0;
    }
    return 1;
#else
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error:\tUsing a Scotch topology without the Scotch library\nYou need to link with TreeMatch with the Scotch library\tExiting!\n");
      exit(-1);
#endif
  }

  else{
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Unknow topology type line %d of file %s\n",__LINE__, __FILE__);
    exit(-1);
    
  }
}




/* cpy flag tells if we need to copy the array.
   Set to 1 when called from the application level and 0 when called from inside the library*/
int tm_topology_set_binding_constraints_cpy(int *constraints, int nb_constraints, tm_topology_t *topology, int cpy_flag){
  if (topology->topology_type == TM_TOPOLOGY_TREE){
    topology->topology.tree ->common-> nb_constraints = nb_constraints;
    if(cpy_flag){
      topology->topology.tree->common -> constraints    =  (int*)MALLOC(nb_constraints*sizeof(int));
      memcpy(topology->topology.tree->common -> constraints, constraints, nb_constraints*sizeof(int));
    }else{
      topology->topology.tree->common -> constraints    = constraints;
    }
  }

  else if (topology->topology_type == TM_TOPOLOGY_SCOTCH){
    topology->topology.scotch ->common-> nb_constraints = nb_constraints;
    if(cpy_flag){
      topology->topology.scotch->common -> constraints    =  (int*)MALLOC(nb_constraints*sizeof(int));
      memcpy(topology->topology.scotch->common -> constraints, constraints, nb_constraints*sizeof(int));
    }else{
      topology->topology.scotch->common -> constraints    = constraints;
    }
  }

  else{
    tm_verbose_fprintf(CRITICAL,verbose_stderr, "Unknown toplogy type to compute nb_processing unit line %d in file %s\n",__LINE__, __FILE__);
    exit(-1);
  }

  return topo_check_constraints(topology);
}

/* Not Called */
int tm_topology_set_binding_constraints(int *constraints, int nb_constraints, tm_topology_t *topology){
  
    return tm_topology_set_binding_constraints_cpy(constraints, nb_constraints, topology, 1);
}

int  tm_topology_add_binding_constraints(char *constraints_filename, tm_topology_t *topology)
{
  int *tab = NULL;
  FILE *pf = NULL;
  char  line[LINE_SIZE],*l = NULL;
  char *ptr = NULL;
  int i,n;


  if (!(pf = fopen(constraints_filename,"r"))) {
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot open %s\n",constraints_filename);
    exit(-1);
  }

  /* compute the size of the array to store the constraints*/
  n = 0;
  fgets(line, LINE_SIZE, pf);
  l = line;
  while((ptr=strtok(l," \t"))){
    l = NULL;
    if((ptr[0] != '\n') && ( !isspace(ptr[0])) && (*ptr) && (ptr))
      n++;
  }

  tab = (int*)MALLOC(n*sizeof(int));

  rewind(pf);
  fgets(line, LINE_SIZE, pf);
  fclose(pf);
  l = line;
  i = 0;
  while((ptr=strtok(l," \t"))){
    l = NULL;
    if((ptr[0] != '\n') && ( !isspace(ptr[0])) && (*ptr) && (ptr)){
      if(i < n)
	tab[i] = atoi(ptr);
      else{
	tm_verbose_fprintf(CRITICAL,verbose_stderr,"More than %d entries in %s\n", n, constraints_filename);
	exit(-1);
      }
      i++;
    }
  }

  if( i != n ){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Read %d entries while expecting %d ones\n", i, n);
    exit(-1);
  }

  qsort(tab,n,sizeof(int),int_cmp_inc);

  return tm_topology_set_binding_constraints_cpy(tab, n, topology, 0);
}


void topology_numbering_cpy(tm_topology_tree_t *topology,int **numbering,int *nb_nodes)
{
  int nb_levels;
  nb_levels = topology->nb_levels;
  *nb_nodes = topology->nb_nodes[nb_levels-1];
  tm_verbose_fprintf(INFO,verbose_stdout,"nb_nodes=%d\n",*nb_nodes);
  *numbering = (int*)MALLOC(sizeof(int)*(*nb_nodes));
  memcpy(*numbering,topology->node_id,sizeof(int)*(*nb_nodes));
}

void topology_arity_cpy(tm_topology_tree_t *topology,int **arity,int *nb_levels)
{
  *nb_levels = topology->nb_levels;
  *arity = (int*)MALLOC(sizeof(int)*(*nb_levels));
  memcpy(*arity,topology->arity,sizeof(int)*(*nb_levels));
}

void topology_constraints_cpy(tm_topology_tree_t *topology,int **constraints,int *nb_constraints)
{
  *nb_constraints = topology->common->nb_constraints;
  if(topology->common->constraints){
    *constraints = (int*)MALLOC(sizeof(int)*(*nb_constraints));
    memcpy(*constraints,topology->common->constraints,sizeof(int)*(*nb_constraints));
  }else{
    *constraints = NULL;
  }
}

void topology_cost_cpy(tm_topology_tree_t *topology,double **cost)
{
  *cost = (double*)MALLOC(sizeof(double)*(topology->nb_levels));
  memcpy(*cost,topology->common->cost,sizeof(double)*(topology->nb_levels));
}

void optimize_arity(int **arity, double **cost, int *nb_levels,int n)
{
  int a,i;
  int *new_arity = NULL;
  double *new_cost = NULL;

  if( n < 0 )
    return;
  /*   printf("n=%d\tnb_levels=%d\n",n,*nb_levels); */
  /*   for(i=0;i<*nb_levels;i++) */
  /*     printf("%d:",(*arity)[i]); */
  /*   printf("\n");   */
  /* if(n==(*nb_levels)-3) */
  /*  exit(-1); */
  a = (*arity)[n];
  if( (a%3 == 0) && (a > 3) ){
    /*
      check if the arity of level n devides 3
      If this is the case:
      Add a level
    */
    (*nb_levels)++;
    /* Build a new arity and cost arrays  */
    new_arity = (int*)MALLOC(sizeof(int)*(*nb_levels));
    new_cost  = (double*)MALLOC(sizeof(double)*(*nb_levels));
    /*  Copy the begining if the old arrays */
    for( i = 0 ; i < n ; i++){
      new_arity[i] = (*arity)[i];
      new_cost[i] = (*cost)[i];
    }
    /* set the nth level to arity 3  */
    new_arity[n] = 3;
    /* copy the cost to this level*/
    new_cost[n] = (*cost)[n];;
    /* printf("a=%d\n",a); */
    /* Set the (n+1) level to arity a/3 */
    new_arity[n+1] = a/3;
    /*Dupliacte the cost as it is the same level originally*/
    new_cost[n+1] = (*cost)[n];
    /* Copy the end of the arrays */
    for( i = n+2 ; i < *nb_levels ; i++){
      new_arity[i] = (*arity)[i-1];
      new_cost[i] = (*cost)[i-1];
    }
    FREE(*arity);
    FREE(*cost);
    /* if a/3 =3 then go to the next level */
    if(new_arity[n+1] == 3)
      optimize_arity(&new_arity,&new_cost,nb_levels,n);
    else /* continue to this level (remember we just add a new level */
      optimize_arity(&new_arity,&new_cost,nb_levels,n+1);
    *arity=new_arity;
    *cost=new_cost;
  }else if( (a%2==0) && (a>2) ){/* same as above but for arity == 2 instead of 3 */
    (*nb_levels)++;
    new_arity = (int*)MALLOC(sizeof(int)*(*nb_levels));
    new_cost  = (double*)MALLOC(sizeof(double)*(*nb_levels));
    for( i = 0 ; i < n ; i++ ){
      new_arity[i] = (*arity)[i];
      new_cost[i] = (*cost)[i];
    }
    new_arity[n] = 2;
    new_cost[n] = (*cost)[n];;
    /* printf("a=%d\n",a); */
    new_arity[n+1] = a/2;
    new_cost[n+1] = (*cost)[n];
    for( i = n+2 ; i < *nb_levels ; i++ ){
      new_arity[i] = (*arity)[i-1];
      new_cost[i] = (*cost)[i-1];
    }
    FREE(*arity);
    FREE(*cost);
    if(new_arity[n+1] == 2)
      optimize_arity(&new_arity, &new_cost, nb_levels, n);
    else
      optimize_arity(&new_arity, &new_cost, nb_levels, n+1);
    *arity = new_arity;
    *cost= new_cost;
  }else /* if nothing works go to next level.  */
    optimize_arity(arity, cost, nb_levels,n-1);
}




void tm_optimize_topology(tm_topology_t **topology){
  int *arity = NULL,nb_levels;
  int *numbering = NULL,nb_nodes;
  tm_topology_t *new_topo;
  double *cost;
  int *constraints = NULL, nb_constraints;
  int i;
  tm_topology_tree_t **tree_topology = &((*topology)->topology.tree);

  if((*topology)->topology_type != TM_TOPOLOGY_TREE){
    tm_verbose_fprintf(WARNING, verbose_stdout, "This topology is Scotch topology: we do not optimize it\n");
    return;
  }
    
  
  if(verbose_level >= DEBUG)
    tm_display_arity(*topology);

  topology_arity_cpy(*tree_topology, &arity, &nb_levels);
  topology_numbering_cpy(*tree_topology, &numbering, &nb_nodes);
  topology_constraints_cpy(*tree_topology, &constraints, &nb_constraints);
  topology_cost_cpy(*tree_topology, &cost);


  optimize_arity(&arity,&cost,&nb_levels,nb_levels-2);
  new_topo = tm_build_synthetic_topology(arity, NULL, nb_levels,numbering,nb_nodes);
  new_topo->topology.tree->common->cost = cost;
  new_topo->topology.tree->common->constraints    = constraints;
  new_topo->topology.tree->common->nb_constraints = nb_constraints;
  new_topo->topology.tree->common->nb_proc_units  = (*topology)->topology.tree->common->nb_proc_units;
  new_topo->topology.tree->common->oversub_fact   = (*topology)->topology.tree->common->oversub_fact;



  if(verbose_level >= DEBUG){
    if(constraints){
      tm_verbose_fprintf(DEBUG, verbose_stdout, "Constraints: ");
      for(i=0;i<nb_constraints;i++)
	fprintf(*verbose_stdout, "%d - ",constraints[i]);
      fprintf(*verbose_stdout, "\n");
    }

    tm_display_arity(new_topo);
  }
  FREE(arity);
  FREE(numbering);
  tm_free_topology(*topology);

  *topology = new_topo;
  /*  exit(-1); */


}



/*
  Build a synthetic balanced topology

  arity : array of arity of the first nb_level (of size nb_levels)
  cost : array of costs between the levels (of size nb_levels)
  core_numbering: numbering of the core by the system. Array of size nb_core_per_node

  nb_core_per_nodes: number of cores of a given node size of the array core_numbering

  The numbering of the cores is done in round robin fashion after a width traversal of the topology.
  for example:
  {0,1,2,3} becomes 0,1,2,3,4,5,6,7...
  and
  {0,2,1,3} becomes 0,2,1,3,4,6,5,7,...
*/



/*sert à faire des arbres?*/
tm_topology_t  *tm_build_synthetic_topology(int *arity, double *cost, int nb_levels, int *core_numbering, int nb_core_per_nodes)
{
  tm_topology_t *topology = NULL;
  tm_topology_tree_t *tree_topology = NULL;
  tm_common_topology_t *common_topology = NULL;
  int i,j,n;

  topology                              = (tm_topology_t*)MALLOC(sizeof(tm_topology_t));
  tree_topology                         = (tm_topology_tree_t*)MALLOC(sizeof(tm_topology_tree_t));
  common_topology                       = (tm_common_topology_t*)MALLOC(sizeof(tm_common_topology_t));
  common_topology->nb_constraints       = 0;
  common_topology->oversub_fact         = 1;
  common_topology->constraints          = NULL;
  tree_topology->common                 = common_topology;
  tree_topology->nb_levels              = nb_levels;
  tree_topology->arity                  = (int*)MALLOC(sizeof(int)*tree_topology->nb_levels);
  tree_topology->nb_nodes               = (size_t *)MALLOC(sizeof(size_t)*tree_topology->nb_levels);
  topology->topology_type               = TM_TOPOLOGY_TREE;
  topology->topology.tree               = tree_topology;
  if(cost)
    topology->topology.tree->common->cost         = (double*)CALLOC(topology->topology.tree->nb_levels,sizeof(double));
  else
    topology->topology.tree->common->cost         = NULL;
  

  memcpy(topology->topology.tree->arity, arity, sizeof(int)*nb_levels);
  if(cost)
    memcpy(topology->topology.tree->common->cost, cost, sizeof(double)*nb_levels);

  n = 1;
  for( i = 0 ; i < topology->topology.tree->nb_levels ; i++ ){
    topology->topology.tree->nb_nodes[i] = n;
    if (i == topology->topology.tree->nb_levels-1){
      topology->topology.tree->node_id        = (int*)MALLOC(sizeof(int)*n);
      topology->topology.tree->node_rank      = (int*)MALLOC(sizeof(int)*n);
      topology->topology.tree->common->nb_constraints = n;
      topology->topology.tree->common->nb_proc_units  = n;
      for( j = 0 ; j < n ; j++ ){
	int id = core_numbering[j%nb_core_per_nodes] + (nb_core_per_nodes)*(j/nb_core_per_nodes);
	topology->topology.tree->node_id[j]    = id;
	topology->topology.tree->node_rank[id] = j;
      }
    }
    n *= topology->topology.tree->arity[i];
  }
  if(cost){
    /*aggregate costs*/
    for( i = topology->topology.tree->nb_levels-2 ; i >= 0 ; i-- )
      topology->topology.tree->common->cost[i] += topology->topology.tree->common->cost[i+1];
  }

  return topology;
}


void   build_synthetic_proc_id(tm_topology_tree_t *topology)
{
  int i;
  size_t j,n = 1;

  topology->nb_nodes  = (size_t*) MALLOC(sizeof(size_t)*topology->nb_levels);

  for( i = 0 ; i < topology->nb_levels ; i++ ){
    /* printf("n= %lld, arity := %d\n",n, topology->arity[i]); */
    topology->nb_nodes[i] = n;
   
    if (i == topology->nb_levels-1){
      topology->node_rank      = (int*)MALLOC(sizeof(int)*n);
      topology->node_id        = (int*)MALLOC(sizeof(int)*n);
      if ( !topology->node_id ){
	tm_verbose_fprintf(CRITICAL,verbose_stderr,"Cannot allocate last level (of size %ld) of the topology\n", (unsigned long int)n);
	exit(-1);
      }
      
      topology->common->nb_constraints = n;
      topology->common->nb_proc_units = n;
      
      for( j = 0 ; j < n ; j++ ){
	topology->node_id[j]   = j;
	topology->node_rank[j] = j;
      }
    }

    n *= topology->arity[i];
  }

}



void tm_enable_oversubscribing(tm_topology_t *topology, unsigned int oversub_fact)
{
  int i,j,n;
  int *node_id, *node_rank;

 
  if(oversub_fact <=1)
    return;
  
  if (topology->topology_type == TM_TOPOLOGY_TREE){
    topology ->topology.tree -> nb_levels ++;
    topology ->topology.tree -> arity        = (int*)    REALLOC(topology->topology.tree->arity, sizeof(int)*topology->topology.tree->nb_levels);
    topology ->topology.tree -> common -> cost         = (double*) REALLOC(topology->topology.tree->common->cost, sizeof(double)*topology->topology.tree->nb_levels);
    topology ->topology.tree -> nb_nodes     = (size_t *)REALLOC(topology->topology.tree->nb_nodes, sizeof(size_t)*topology->topology.tree->nb_levels);
    topology ->topology.tree -> common -> oversub_fact = oversub_fact;

    i = topology->topology.tree->nb_levels - 1;
    n = topology->topology.tree->nb_nodes[i-1] * oversub_fact;
    topology->topology.tree->arity[i-1] = oversub_fact;
    topology->topology.tree->common->cost[i-1] = 0;
    node_id = (int*)MALLOC(sizeof(int)*n);
    node_rank = (int*)MALLOC(sizeof(int)*n);
    topology->topology.tree->nb_nodes[i] = n;

    for( j = 0 ; j < n ; j++ ){
      int id = topology->topology.tree->node_id[j/oversub_fact];
      node_id[j]    = id;
      node_rank[id] = j;
    }
    FREE(topology->topology.tree->node_id);
    FREE(topology->topology.tree->node_rank);
    topology->topology.tree->node_id   = node_id;
    topology->topology.tree->node_rank = node_rank;
  } else   if (topology->topology_type == TM_TOPOLOGY_SCOTCH){
    topology->topology.scotch->common->oversub_fact = oversub_fact;
  }
}


#if HAVE_LIBSCOTCH
SCOTCH_Arch tscotch_to_tm(char *arch_filename){
  FILE          *archFileptr;
  SCOTCH_Arch   archdat;

  if(!(archFileptr = fopen (arch_filename, "r"))){
    tm_verbose_fprintf(CRITICAL,verbose_stderr,"Error: Cannot open %s\n", arch_filename);
    exit(-1);
  }

  SCOTCH_archInit (&archdat);               /* Create architecture structure    */
  SCOTCH_archLoad (&archdat, archFileptr);  /* Read target architecture         */

  return archdat;
}
#endif
