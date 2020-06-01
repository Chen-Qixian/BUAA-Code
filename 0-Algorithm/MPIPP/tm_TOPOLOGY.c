/*
* @Author: kingofwolf
* @Date:   2019-03-07 15:47:43
* @Last Modified by:   kingofwolf
* @Last Modified time: 2019-03-09 15:52:55
*/
#include <hwloc.h>
#include <hwloc/helper.h>
#include <ctype.h>
#include "tm_MALLOC.h"
#include "tm_TOPOLOGY.h"

#define LINE_SIZE (1000000)

void   build_synthetic_proc_id(tm_topology_t *topology)
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
	// if(tm_get_verbose_level() >= CRITICAL)
	//   fprintf(stderr,"Cannot allocate last level (of size %ld) of the topology\n", (unsigned long int)n);
	exit(-1);
      }
      
      topology->nb_constraints = n;
      topology->nb_proc_units = n;
      
      for( j = 0 ; j < n ; j++ ){
	topology->node_id[j]   = j;
	topology->node_rank[j] = j;
      }
    }

    n *= topology->arity[i];
  }

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

/* transform a tgt scotch file into a topology file*/
tm_topology_t * tgt_to_tm(char *filename)
{
  tm_topology_t *topology = NULL;
  FILE *pf = NULL;
  char line[1024];
  char *s = NULL;
  double *cost = NULL;
  int i;



  pf = fopen(filename,"r");
  if(!pf){
    //if(tm_get_verbose_level() >= CRITICAL)
      fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  // if(tm_get_verbose_level() >= INFO)
  //   printf("Reading TGT file: %s\n",filename);


  fgets(line,1024,pf);
  fclose(pf);

  s = strstr(line,"tleaf");
  if(!s){
    //if(tm_get_verbose_level() >= CRITICAL)
      fprintf(stderr,"Syntax error! %s is not a tleaf file\n",filename);
    exit(-1);
  }

  s += 5;
  while(isspace(*s))
    s++;

  topology                 = (tm_topology_t*)MALLOC(sizeof(tm_topology_t));
  topology->nb_constraints = 0;
  topology->oversub_fact   = 1;
  topology->constraints    = NULL;
  topology->nb_levels      = atoi(strtok(s," "))+1;
  topology->arity          = (int*)MALLOC(sizeof(int)*topology->nb_levels);

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

  // if(tm_get_verbose_level() >= INFO)
  //   printf("Topology built from %s!\n",filename);

  topology->cost=cost;


  return topology;
}

void build_process_tab_id(tm_topology_t *topology,  hwloc_obj_t *objs, char* filename){
  unsigned int i,j;
  unsigned int nb_nodes = topology->nb_proc_units; 
  //int vl = tm_get_verbose_level();
  
  /* Build process id tab */
//  if(numbering == TM_NUMBERING_LOGICAL){
    for (i = 0; i < nb_nodes; i++){
      topology->node_id[i] = i;
      topology->node_rank[i] = i;
    }
 //  }else if(numbering == TM_NUMBERING_PHYSICAL){
 //    for (i = 0; i < nb_nodes; i++){
 //      if(objs[i]->os_index > nb_nodes){
	// //if(vl >= CRITICAL){
	//   fprintf(stderr, "Cannot use forced physical numbering!\n\tIndex of PU %d is %d and larger than number of nodes : %d\n",
	// 	  i, objs[i]->os_index, nb_nodes);
	// //}
	// exit(-1);
 //      }
 //      for(j = 0; j < i; j++){
	// if((unsigned int)topology->node_id[j] == objs[i]->os_index){
	//   //if(vl >= CRITICAL){
	//     fprintf(stderr, "Cannot use forced physical numbering!\n\tDuplicated physical number of some PUs in %s.\n\tPU %d and PU %d have the same physical number: (os_index[%d] = %d) == (os_index[%d] = %d)\n", filename, j, i, j, objs[j]->os_index, i, objs[i]->os_index);
	//   //}
	//   exit(-1);
	// }
 //      }
 //      topology->node_id[i] = objs[i]->os_index;
 //      topology->node_rank[objs[i]->os_index] = i;
 //    }
 //  }else{
 //    //if(vl >= CRITICAL){
 //      fprintf(stderr, "Unknown numbering %d\n", (int)numbering);
 //    //}
 //    exit(-1);
 //  }
}


tm_topology_t* hwloc_to_tm(char *filename)
{
  hwloc_topology_t topology;
  tm_topology_t *res = NULL;
  hwloc_obj_t *objs = NULL;
  unsigned topodepth,depth;
  unsigned int nb_nodes;
  double *cost;
  int err, l;
  //int vl = tm_get_verbose_level();

  /* Build the topology */
  hwloc_topology_init(&topology);
  err = hwloc_topology_set_xml(topology, filename);
  if(err == -1){
    //if(vl >= CRITICAL)
      fprintf(stderr,"Error: %s is a bad xml topology file!\n",filename);
    exit(-1);
  }

#if HWLOC_API_VERSION < 0x20000
  hwloc_topology_ignore_all_keep_structure(topology);
#else
  hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_STRUCTURE);
#endif

  err = hwloc_topology_load(topology);
  if(err == -1){
    //if(vl >= CRITICAL)
      fprintf(stderr,"Error: the content of the xml topology file %s is not compatible with the version installed on this machine.\nPlease use compatible versions to generate the file and to use it!\n",filename);
    exit(-1);
  }


  /* Test if symetric */
  if(!symetric(topology)){
    //if(vl >= CRITICAL)
      fprintf(stderr,"%s not symetric!\n",filename);
    exit(-1);
  }

  /* work on depth */
  topodepth = hwloc_topology_get_depth(topology);
  
  res                   = (tm_topology_t*)MALLOC(sizeof(tm_topology_t));
  res->oversub_fact      = 1;
  res->nb_constraints   = 0;
  res->constraints      = NULL;
  res->nb_levels        = topodepth;
  res->nb_nodes         = (size_t*)MALLOC(sizeof(size_t)*res->nb_levels);
  res->arity            = (int*)MALLOC(sizeof(int)*res->nb_levels);

  // if(vl >= INFO)
  //     printf("topodepth = %d\n",topodepth);

  /* Build TreeMatch topology */
  for( depth = 0 ; depth < topodepth ; depth++ ){
    nb_nodes = hwloc_get_nbobjs_by_depth(topology, depth);
    res->nb_nodes[depth] = nb_nodes;

    objs    = (hwloc_obj_t*)MALLOC(sizeof(hwloc_obj_t)*nb_nodes);
    objs[0] = hwloc_get_next_obj_by_depth(topology, depth, NULL);
    hwloc_get_closest_objs(topology, objs[0], objs+1, nb_nodes-1);
    res->arity[depth] = objs[0]->arity;
    
    // if(vl >= DEBUG)
    //   printf("\n--%d(%d) **%d**:--\n",res->arity[depth],nb_nodes,res->arity[0]);

    
    if (depth == topodepth -1){
      res->nb_constraints = nb_nodes;
      res->nb_proc_units  = nb_nodes;
      res->node_id        = (int*)MALLOC(sizeof(int)*nb_nodes);
      res->node_rank      = (int*)MALLOC(sizeof(int)*nb_nodes);
   
      build_process_tab_id(res, objs, filename);
     
    }
    FREE(objs);


  }

  cost = (double*)CALLOC(res->nb_levels,sizeof(double));
  for(l=0; l<res->nb_levels; l++){
    cost[l] = link_cost(l);
  }
  res->cost = cost;


  /* Destroy topology object. */
  hwloc_topology_destroy(topology);
  // if(tm_get_verbose_level() >= INFO)
  //   printf("\n");



  return res;
}

void tm_free_topology(tm_topology_t *topology)
{
  FREE(topology->node_id);
  FREE(topology->node_rank);
  FREE(topology->constraints);
  FREE(topology->nb_nodes);
  FREE(topology->arity);
  FREE(topology->cost);
  FREE(topology);
}

tm_topology_t *tm_load_topology(char *arch_filename, tm_file_type_t arch_file_type){
  switch(arch_file_type){
  case   TM_FILE_TYPE_TGT:
    return  tgt_to_tm(arch_filename);
  case TM_FILE_TYPE_XML:
    return hwloc_to_tm(arch_filename);
  default:
    //if(tm_get_verbose_level() >= ERROR){
      fprintf(stderr,"Error loading topology. Filetype %d unknown\n", arch_file_type);
    //}
    exit(-1);
  }
}