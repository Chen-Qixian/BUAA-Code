# lib_LIBRARIES=libtreematch.a

# libtreematch_a_SOURCES= \
# 	tm_mapping.c tm_mapping.h \
# 	tm_bucket.c tm_bucket.h \
#         tm_hwloc.c tm_hwloc.h \
# 	tm_timings.c tm_timings.h \
#         tm_tree.c tm_tree.h

# include_HEADERS = tm_mapping.h

lib_LTLIBRARIES=libtreematch.la

AM_CFLAGS = @HWLOC_CFLAGS@
LIBS += @HWLOC_LIBS@

#libtreematch_la_CFLAGS = $(AM_CFLAGS) -g -Wall -Wextra -ansi -pedantic -pthread -std=c99 #-Qunused-arguments
libtreematch_la_CFLAGS = $(AM_CFLAGS) -O3 -Wall -Wextra -ansi -pedantic -pthread -std=c99 -DNDEBUG -Wimplicit-fallthrough=0
#libtreematch_la_CFLAGS = $(AM_CFLAGS) -O3 -Wall -Wextra -ansi -pedantic -pthread -std=c99 -DNDEBUG

#CC = clang


libtreematch_la_SOURCES= \
	tm_mapping.c tm_mapping.h \
	tm_bucket.c tm_bucket.h \
        tm_topology.c tm_topology.h \
	tm_timings.c tm_timings.h \
	tm_tree.c tm_tree.h \
	tm_verbose.c tm_verbose.h \
	tm_malloc.c tm_malloc.h \
	tm_solution.c tm_solution.h \
	tm_thread_pool.c tm_thread_pool.h \
	tm_kpartitioning.c tm_kpartitioning.h \
	tm_scotch.c tm_scotch.h\
	tm_mt.c tm_mt.h \
	uthash.h \
	fibo.c fibo.h \
	IntConstantInitializedVector.c IntConstantInitializedVector.h \
	PriorityQueue.c PriorityQueue.h \
	k-partitioning.c k-partitioning.h 

include_HEADERS = treematch.h tm_verbose.h tm_malloc.h ../../tm_config.h
