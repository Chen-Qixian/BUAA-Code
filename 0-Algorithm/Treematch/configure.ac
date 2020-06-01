#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(TreeMatch, 1.4, emmanuel.jeannot@inria.fr)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Werror -Wall])
AC_CONFIG_SRCDIR([src/libtreematch/tm_mapping.h])
AC_CONFIG_HEADERS([tm_config.h])
PKG_PROG_PKG_CONFIG


# adding standard path where HWLOC is normally installed
CFLAGS="$CFLAGS -I/usr/local/include"
CPPFLAGS="$CPPFLAGS -I/usr/local/include"
LDFLAGS="$LDFLAGS -L/usr/local/lib"


m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
# Libtool
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])


# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_MAKE_SET

# Checks for libraries.
AC_SEARCH_LIBS([log2], [m], [], [exit])
PKG_CHECK_MODULES(HWLOC, hwloc, dummy=yes, 
	AC_SEARCH_LIBS([hwloc_topology_get_depth], [hwloc], [libhwloc=yes],
	[
	  AC_MSG_ERROR([Could not find the HWLOC library: http://www.open-mpi.org/projects/hwloc/])	
	])
)
	
if test $libhwloc = "yes"; then
  AC_CHECK_HEADERS([hwloc.h],,[exit])
fi

# Checks for header files.
AC_CHECK_HEADERS([math.h sys/timeb.h errno.h unistd.h stdint.h sys/time.h pthread.h stdlib.h limits.h float.h string.h ctype.h inttypes.h assert.h strings.h stdio.h  memory.h  stddef.h pthread.h],,[exit])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atof atoi bcmp calloc ceil exit fclose fflush fgets floor fopen fprintf free gettimeofday isspace log2  memcpy memset pow printf qsort  rewind strerror strlen strstr strtok])

AC_CHECK_LIB(pthread, pthread_create, [PTHREAD_LIBS+=-lpthread])
AC_CHECK_LIB(pthread, pthread_create)

#Check for SCOTCH relative stuff
AC_CHECK_LIB(z, gzdopen)
AC_ARG_VAR(SCOTCH_DIR, path to Scotch install directory (under /lib and /include e.g. /usr/local))
if test $SCOTCH_DIR != ""; then  
   # adding the path to SCOTCH lib and include as specified by SCOTCH_DIR variable
   CFLAGS="-I$SCOTCH_DIR/include $CFLAGS"
   CPPFLAGS="-I$SCOTCH_DIR/include $CPPFLAGS"
   LDFLAGS="-L$SCOTCH_DIR/lib $LDFLAGS"
fi

# Need to do this first.
AC_CHECK_LIB(scotcherr, SCOTCH_errorPrint)
# the next check will fail if libscotcherr is not detected
AC_CHECK_LIB(scotch, SCOTCH_graphPartFixed,[libscotch=yes],[libscotch=no])
# do it agin for setting compile flags and HAVE_LIBSCOTCH variable
AC_CHECK_LIB(scotch, SCOTCH_graphPartFixed)

#We have detected libscotch. Do additional check. 
if test $libscotch = "yes"; then
   AC_CHECK_HEADERS([scotch.h],[scotch_h=yes],[scotch_h=no])
   if test $scotch_h = "no"; then
       AC_MSG_ERROR(Cannot find scotch.h (while libscotch is detected). Plese check installation and/or SCOTCH_DIR variable...)
   fi

#check is Scotch is compiled in 64 or 32 integer bits
   AC_MSG_CHECKING(Scotch integer size)
   AC_RUN_IFELSE([AC_LANG_SOURCE(
	 [#include <stdlib.h> 
	 #include <stdio.h> 
	 #include "scotch.h" 
	 int main(){if(sizeof(SCOTCH_Num)>=8) return 0; else return -1;}])], [scotch_int64=yes],[scotch_int64=no])
   if test $scotch_int64 = "yes"; then
      AC_MSG_RESULT(64 bits)
      AC_DEFINE(SCOTCH_NUM_SIZE, 64, [sizeof SCOTCH_Num])
   else
      AC_MSG_RESULT(32 bits)
      AC_DEFINE(SCOTCH_NUM_SIZE, 32, [sizeof SCOTCH_Num])
   fi
   
   # check if we have the fonction to compute distance in Scotch_arch
   AC_CHECK_LIB(scotch, SCOTCH_archDomDist,[newlibscotch=yes],[newlibscotch=no])
   if test $newlibscotch = "yes"; then
      AC_DEFINE(HAVE_NEWLIBSCOTCH, 1, [Scotch library with distance API])
   fi
fi





# Add DRESTRICT
CFLAGS="$CFLAGS -Drestrict=__restrict"

# Display all warnings
#if test "x$GCC" = "xyes"; then
#  CFLAGS="$CFLAGS -Wall"
#fi

#src/k-partitioning/Makefile
#	src/slurm_sim/Makefile

AC_CONFIG_FILES([
        Makefile
	examples/Makefile
	src/Makefile
	src/treematch/Makefile
	src/libtreematch/Makefile
        ])
AC_OUTPUT


echo""
echo "Configure done!"
if test $libscotch = "yes"; then
  echo "Scotch Library detected"
  echo "TreeMatch will be compiled with the Scotch Library"
  if test $scotch_int64 = "no"; then
     echo "WARNING: Scotch library compiled with 32 bits integer. This might not work for large values."
     echo "         We advise you to compile Scotch with -DINTSIZE64 in the CFLAGS variable of Makfefile.inc"
  fi
  if test $newlibscotch = "no"; then
     echo "WARNING: You have an old version of Scotch. Please upgrade to version 7 or higher to get metric evaluation features."
  fi
fi

if test $libscotch = "no"; then
  echo "WARNING : Scotch Library ***not*** detected!"
  echo "          TreeMatch will ***not*** be compiled with the Scotch Library (ver. 6 or higher)."
  echo "          This is fine as Scotch only provides optional features."
  echo "	  If Scotch is available on your system please consider setting SCOTCH_DIR when calling this script:" 
  echo "          ./configure SCOTCH_DIR=/path/to/scotch/install"
  echo "	  Be aware that Scotch requires the lz, lpthread and lm library to work..."
fi
echo""
