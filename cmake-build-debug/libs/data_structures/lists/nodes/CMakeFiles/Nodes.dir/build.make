# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/danis/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.6693.114/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/danis/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.6693.114/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/danis/Desktop/AnnouncementServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/danis/Desktop/AnnouncementServer/cmake-build-debug

# Include any dependencies generated for this target.
include libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/depend.make

# Include the progress variables for this target.
include libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/progress.make

# Include the compile flags for this target's objects.
include libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/flags.make

libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.o: libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/flags.make
libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.o: ../libs/data_structures/lists/nodes/node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danis/Desktop/AnnouncementServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.o"
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Nodes.dir/node.c.o -c /home/danis/Desktop/AnnouncementServer/libs/data_structures/lists/nodes/node.c

libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Nodes.dir/node.c.i"
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/danis/Desktop/AnnouncementServer/libs/data_structures/lists/nodes/node.c > CMakeFiles/Nodes.dir/node.c.i

libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Nodes.dir/node.c.s"
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/danis/Desktop/AnnouncementServer/libs/data_structures/lists/nodes/node.c -o CMakeFiles/Nodes.dir/node.c.s

# Object files for target Nodes
Nodes_OBJECTS = \
"CMakeFiles/Nodes.dir/node.c.o"

# External object files for target Nodes
Nodes_EXTERNAL_OBJECTS =

libs/data_structures/lists/nodes/libNodes.a: libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/node.c.o
libs/data_structures/lists/nodes/libNodes.a: libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/build.make
libs/data_structures/lists/nodes/libNodes.a: libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/danis/Desktop/AnnouncementServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libNodes.a"
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && $(CMAKE_COMMAND) -P CMakeFiles/Nodes.dir/cmake_clean_target.cmake
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Nodes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/build: libs/data_structures/lists/nodes/libNodes.a

.PHONY : libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/build

libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/clean:
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes && $(CMAKE_COMMAND) -P CMakeFiles/Nodes.dir/cmake_clean.cmake
.PHONY : libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/clean

libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/depend:
	cd /home/danis/Desktop/AnnouncementServer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/danis/Desktop/AnnouncementServer /home/danis/Desktop/AnnouncementServer/libs/data_structures/lists/nodes /home/danis/Desktop/AnnouncementServer/cmake-build-debug /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes /home/danis/Desktop/AnnouncementServer/cmake-build-debug/libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/data_structures/lists/nodes/CMakeFiles/Nodes.dir/depend
