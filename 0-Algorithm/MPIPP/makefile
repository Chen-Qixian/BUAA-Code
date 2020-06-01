objects = map_MPIPP.o tm_TOPOLOGY.o tm_MALLOC.o tm_TIMINGS.o tm_MAPPING.o
soobjects =MPIPP_C2PY.c map_MPIPP.c map_MPIPP.h tm_MALLOC.c tm_MALLOC.h tm_MAPPING.c tm_MAPPING.h tm_TIMINGS.c tm_TIMINGS.h tm_TOPOLOGY.c tm_TOPOLOGY.h UTHash.h
all : map_MPIPP MPIPP_C2PY.so

MPIPP_C2PY.so : $(soobjects)
	gcc -o MPIPP_C2PY.so -shared -fPIC MPIPP_C2PY.c map_MPIPP.c tm_MALLOC.c tm_MAPPING.c tm_TIMINGS.c tm_TOPOLOGY.c -L/usr/local/lib -lhwloc

map_MPIPP : $(objects)
	gcc -o map_MPIPP $(objects) -L/usr/local/lib -lhwloc

tm_TOPOLOGY.o : tm_MALLOC.h tm_TOPOLOGY.h
tm_MALLOC.o : tm_MALLOC.h UTHash.h
tm_TIMINGS.o : tm_TIMINGS.h 
tm_MAPPING.o : tm_MAPPING.h tm_TOPOLOGY.h

.PHONY : clean run cleanall
clean :
	rm -rf $(objects)
cleanall :
	rm -rf $(objects)
	rm -rf *.so
	rm -rf map_MPIPP
run :
	./map_MPIPP -t ../examples/netgraph/128.tgt -c ../examples/taskgraph/128.mat
test1:
	./map_MPIPP -f ../examples/netgraph/TimeCostDataFile_128-7-20.txt -T 128 -N 7 -C 20 -c ../examples/taskgraph/128.mat