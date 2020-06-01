#include <unistd.h>
#include <ctype.h>
#include <treematch.h>
#include <tm_timings.h>
#include "tm_config.h"

#define Deb() print_line(__LINE__)/*TODO a suprimer*/

void print_line(int line_num){
  fprintf(stderr,"---> DEBUG line %d\n",line_num);
}

int test_mapping(char *arch_filename, tm_file_type_t arch_file_type, char *com_filename, int bind_flag,
		 char *bind_filename, int opt_topo_flag, tm_metric_t metric, int oversub_fact, int display_solution_flag) {
   /*tm_tree_t      *comm_tree = NULL;*/
   tm_topology_t  *topology;
   int            verbose_level = tm_get_verbose_level();
   tm_affinity_mat_t *aff_mat;
   tm_solution_t  *sol;

   
   TIC;
   TIC;
   /* Parse the arch file according to its type XML or TGT*/
   topology = tm_load_topology(arch_filename, arch_file_type);

   /* build affinity matrices and get the number of processes*/
   aff_mat = tm_load_aff_mat(com_filename);
   /* If we have binding constraints store them in the topology */
   if (bind_flag) {
     if(!tm_topology_add_binding_constraints(bind_filename, topology))
       exit(-1);
   }
   double io_dur = TOC;

   if(verbose_level == INFO)
     tm_display_arity(topology);
   else if(verbose_level >= DEBUG)
     tm_display_topology(topology);


   /* Manage the case where oversubscribing is requested */
   if(oversub_fact>1){
     tm_enable_oversubscribing(topology, oversub_fact);
   }

   /* optimize the toplogy in order to decompose arity of the tree into subtrees.
      Usualy it speeds up tm_build_tree_from_topology
    */
   TIC;
   if(opt_topo_flag)
     tm_optimize_topology(&topology);
   double    optimize_dur = TOC;

   if(verbose_level == INFO)
     tm_display_arity(topology);
   else if(verbose_level >= DEBUG)
     tm_display_topology(topology);

   TIC;
   /* Compute the mapping and build the solution*/
   sol = tm_compute_mapping(topology, aff_mat , NULL , NULL);
   double    map_dur = TOC;

   double    duration = TOC;

   tm_verbose_fprintf(TIMING, verbose_stdout, "I/O:            %10.4f ms\n", io_dur * 1000);
   tm_verbose_fprintf(TIMING, verbose_stdout, "Optimize:       %10.4f ms\n", optimize_dur * 1000);
   tm_verbose_fprintf(TIMING, verbose_stdout, "Mapping:        %10.4f ms\n", map_dur * 1000);
   tm_verbose_fprintf(TIMING, verbose_stdout, "Total duration: %10.4f ms\n", duration * 1000);
   

   if(display_solution_flag){
     /* display the solution. Warnig: use the oiginal topology not the optimized one*/
     printf("TreeMatch: ");
     tm_display_solution(topology, aff_mat, sol, metric);
     /* display the solution of other heuristics*/
     tm_display_other_heuristics(topology, aff_mat, metric);
   }
   
   /* Free the allocated memory*/
   tm_free_topology(topology);
   tm_free_solution(sol);
   tm_free_affinity_mat(aff_mat);
   tm_finalize();

   return 0;
}

void printUsage(char **argv) {
  fprintf(stderr,"This is the mapping tool for %s (http://treematch.gforge.inria.fr) version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
#ifdef HAVE_LIBSCOTCH
  if(SCOTCH_NUM_SIZE == 64){
    fprintf(stderr,"This version is compiled with Scotch (64 bits).\n");
  }else{
    fprintf(stderr,"This version is compiled with Scotch (32 bits).\n");
    fprintf(stderr,"WARNING: Scotch library compiled with 32 bits integer. This might not work for large values.\n");
    fprintf(stderr,"         We advise you to compile Scotch with -DINTSIZE64 in the CFLAGS variable of Makfefile.inc\n");
  }
#else
  fprintf(stderr,"This version is not compiled with Scotch.\n");
#endif  
  fprintf(stderr,
	   "Usage: %s\n\t-t|x <filename> Architecture file[tgt|xml]\n\t-c <filename> Communication pattern file\n",
	   argv[0]);
  fprintf(stderr,
	  "Options:\n\t-b <filename> binding constraint file\n\t-f <filename> filename where to output the verbose messages\n\t-m <metric> evaluation metric (1: SUM_COM (default), 2: MAX_COM, 3: HOP_BYTE)\n\t-o <factor> oversubscribing factor (default 1)\n\t-p <nb thread> maximum number of threads used in parallel sections (default: number of cores)\n\t-v <level> verbose level. Default: 2. From 0 (disabled) to 7 (full debug verbose).\n");

    fprintf(stderr,
	  "Flags:\n\t-d disable topology optimization\n\t-e force exhaustive search\n\t-g force greedy k-partionniong\n\t-n force physical numbering (usefull only for XML/hwloc)\n\t-s disable display of solution (useful for timings -- with -v 4)\n\t-h display this help\n");
}



int main(int argc, char **argv) {
  char            c;
  char            *arch_filename    = NULL;
  char            *com_filename     = NULL;
  char            *bind_filename    = NULL;
  int             bind_flag = 0;
  int             opt_topo_flag = 1;
  int             display_solution_flag = 1;
  int             verbose_level = ERROR;
  tm_file_type_t  arch_file_type = TM_FILE_TYPE_UNDEF;
  tm_metric_t     metric = TM_METRIC_SUM_COM;
  int             oversub_fact = 1;

  tm_set_exhaustive_search_flag(0);
  tm_set_greedy_flag(0);
  tm_set_numbering(TM_NUMBERING_LOGICAL);
  /*single charcater <=> flag ; character + ':' <=> option*/
   while ((c = getopt(argc, argv, "hsngedt:x:c:b:v:m:o:p:f:l:")) != -1) {
     switch (c) {
         case 'l':
	   tm_set_mapping_strat(atoi(optarg));
	   break; 
         case 's':
	   display_solution_flag = 0;
	   break;
         case 'h':
	   printUsage(argv);
	   exit(-1);
         case 'd':
	   opt_topo_flag = 0;
	   break;
         case 'g':
	   tm_set_greedy_flag(1);
	   break;
         case 'n':
	   tm_set_numbering(TM_NUMBERING_PHYSICAL);
	   break;
         case 'e':
	   tm_set_exhaustive_search_flag(1);
	   break;
         case 'p':
	   tm_set_max_nb_threads(atoi(optarg));
	   break;
	 case 'f':
	    tm_open_verbose_file(optarg);	    
	    break;
	 case 't':
	    arch_filename = optarg;
	    arch_file_type = TM_FILE_TYPE_TGT;
	    break;
	 case 'x':
	    arch_filename = optarg;
	    arch_file_type = TM_FILE_TYPE_XML;
	    break;
	 case 'c':
	    com_filename = optarg;
	    break;
	 case 'b':
	    bind_flag = 1;
	    bind_filename = optarg;
	    break;
	 case 'v':
	   verbose_level  = atoi(optarg);
	   break;
	 case 'm':
	   metric  = atoi(optarg);
	   break;
         case 'o':
	   oversub_fact = atoi(optarg);
           break;
	 case '?':
	    if (isprint(optopt))
	       fprintf(stderr, "Unknown option `-%c'.\n", optopt);
	    else
	       fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);

	    printUsage(argv);
	    exit(-1);

	 default:
	    printUsage(argv);
	    exit(-1);
      }
   }

   if (!arch_filename || !com_filename) {
      fprintf(stderr,"Error: Missing \"Architecture file\" and/or \"Communication pattern file\"!\n");
      printUsage(argv);
      abort();
   }

   tm_set_verbose_level(verbose_level);
   
   return test_mapping(arch_filename, arch_file_type, com_filename, bind_flag, bind_filename,
		       opt_topo_flag, metric, oversub_fact, display_solution_flag);

}
