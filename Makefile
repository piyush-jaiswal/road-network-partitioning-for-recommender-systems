compiler = g++
warning_flags = -Wall -Werror
compilation_flag = -c
object_flag = -o
source_files_object = partitions_and_POIs_source.o partitioned_grids_source.o

guide:
	@echo "Available commands:\nmake compile: Compile the source files.\nmake run: Execute the program.\nmake clean: Remove the objects and binary executables."

compile: $(source_files_object)
	$(compiler) $(warning_flags) $(object_flag) main main.cpp $(source_files_object)

run: compile main
	./main

partitions_and_POIs_source.o:
	$(compiler) $(compilation_flag) partitions_and_POIs_source.cpp

partitioned_grids_source.o:
	$(compiler) $(compilation_flag) partitioned_grids_source.cpp

clean:
	rm -rf *.o main
