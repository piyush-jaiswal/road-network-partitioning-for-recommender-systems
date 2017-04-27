compiler = g++ -std=c++11
warning_flags = -Wall -Werror
object_flag = -c
executable_flag = -o
source_files_object = partitions_and_POIs_source.o partitioned_grids_source.o haversine_distance_source.o queryProcessing.o

guide:
	@echo "\nAvailable commands:\nmake compile: Compile the source files.\nmake run: Execute the program.\nmake clean: Remove the objects and binary executables.\n"

compile: $(source_files_object)
	$(compiler) $(warning_flags) $(executable_flag) main main.cpp $(source_files_object)

run: compile main
	./main < inp.txt

partitions_and_POIs_source.o:
	$(compiler) $(object_flag) partitions_and_POIs_source.cpp

partitioned_grids_source.o:
	$(compiler) $(object_flag) partitioned_grids_source.cpp

haversine_distance_source.o:
	$(compiler) $(object_flag) haversine_distance_source.cpp

queryProcessing.o:
	$(compiler) $(object_flag) queryProcessing.cpp

clean:
	rm -rf *.o main

