# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/mtm/cmake-3.17.0-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/mtm/cmake-3.17.0-Linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build

# Include any dependencies generated for this target.
include CMakeFiles/priority_queue.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/priority_queue.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/priority_queue.dir/flags.make

CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o: CMakeFiles/priority_queue.dir/flags.make
CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o: ../tests/pq_example_tests.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o   -c /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/tests/pq_example_tests.c

CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.i"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/tests/pq_example_tests.c > CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.i

CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.s"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/tests/pq_example_tests.c -o CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.s

CMakeFiles/priority_queue.dir/priority_queue.c.o: CMakeFiles/priority_queue.dir/flags.make
CMakeFiles/priority_queue.dir/priority_queue.c.o: ../priority_queue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/priority_queue.dir/priority_queue.c.o"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/priority_queue.dir/priority_queue.c.o   -c /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/priority_queue.c

CMakeFiles/priority_queue.dir/priority_queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/priority_queue.dir/priority_queue.c.i"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/priority_queue.c > CMakeFiles/priority_queue.dir/priority_queue.c.i

CMakeFiles/priority_queue.dir/priority_queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/priority_queue.dir/priority_queue.c.s"
	/usr/bin/x86_64-redhat-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/priority_queue.c -o CMakeFiles/priority_queue.dir/priority_queue.c.s

# Object files for target priority_queue
priority_queue_OBJECTS = \
"CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o" \
"CMakeFiles/priority_queue.dir/priority_queue.c.o"

# External object files for target priority_queue
priority_queue_EXTERNAL_OBJECTS =

priority_queue: CMakeFiles/priority_queue.dir/tests/pq_example_tests.c.o
priority_queue: CMakeFiles/priority_queue.dir/priority_queue.c.o
priority_queue: CMakeFiles/priority_queue.dir/build.make
priority_queue: CMakeFiles/priority_queue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable priority_queue"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/priority_queue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/priority_queue.dir/build: priority_queue

.PHONY : CMakeFiles/priority_queue.dir/build

CMakeFiles/priority_queue.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/priority_queue.dir/cmake_clean.cmake
.PHONY : CMakeFiles/priority_queue.dir/clean

CMakeFiles/priority_queue.dir/depend:
	cd /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build /home/yant/mtm/mtm_hw1/submission_folder/mtm_pq/build/CMakeFiles/priority_queue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/priority_queue.dir/depend

