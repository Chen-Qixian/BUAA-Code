#LDADD = ../libtreematch/libtreematch.a

AM_CFLAGS = @HWLOC_LIBS@
LIBS += @HWLOC_LIBS@


 mapping_CFLAGS = $(AM_CFLAGS) -O3  -Wall -Wextra -pthread
# mapping_CFLAGS = $(AM_CFLAGS) -g  -Wall -Wextra -Qunused-arguments



AM_CPPFLAGS = -I$(srcdir)/../libtreematch 

bin_PROGRAMS=mapping #kpart #test_ompi

mapping_SOURCES= mapping.c
mapping_LDADD = ../libtreematch/libtreematch.la
#kpart_SOURCES= kpart.c
#kpart_LDADD = ../libtreematch/libtreematch.la
# test_ompi_SOURCES= test_ompi.c
# test_ompi_LDADD = ../libtreematch/libtreematch.la
