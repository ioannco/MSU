# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Ivan\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5080.54\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Ivan\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5080.54\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = \\wsl$$\Ubuntu\home\master\msu\c_homework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = \\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release

# Include any dependencies generated for this target.
include CMakeFiles/homework_6.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/homework_6.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/homework_6.dir/flags.make

CMakeFiles/homework_6.dir/homework_6.c.obj: CMakeFiles/homework_6.dir/flags.make
CMakeFiles/homework_6.dir/homework_6.c.obj: ../homework_6.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=\\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/homework_6.dir/homework_6.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\homework_6.dir\homework_6.c.obj -c \\wsl$$\Ubuntu\home\master\msu\c_homework\homework_6.c

CMakeFiles/homework_6.dir/homework_6.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/homework_6.dir/homework_6.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E \\wsl$$\Ubuntu\home\master\msu\c_homework\homework_6.c > CMakeFiles\homework_6.dir\homework_6.c.i

CMakeFiles/homework_6.dir/homework_6.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/homework_6.dir/homework_6.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S \\wsl$$\Ubuntu\home\master\msu\c_homework\homework_6.c -o CMakeFiles\homework_6.dir\homework_6.c.s

# Object files for target homework_6
homework_6_OBJECTS = \
"CMakeFiles/homework_6.dir/homework_6.c.obj"

# External object files for target homework_6
homework_6_EXTERNAL_OBJECTS =

homework_6.exe: CMakeFiles/homework_6.dir/homework_6.c.obj
homework_6.exe: CMakeFiles/homework_6.dir/build.make
homework_6.exe: CMakeFiles/homework_6.dir/linklibs.rsp
homework_6.exe: CMakeFiles/homework_6.dir/objects1.rsp
homework_6.exe: CMakeFiles/homework_6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=\\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable homework_6.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\homework_6.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/homework_6.dir/build: homework_6.exe
.PHONY : CMakeFiles/homework_6.dir/build

CMakeFiles/homework_6.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\homework_6.dir\cmake_clean.cmake
.PHONY : CMakeFiles/homework_6.dir/clean

CMakeFiles/homework_6.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" \\wsl$$\Ubuntu\home\master\msu\c_homework \\wsl$$\Ubuntu\home\master\msu\c_homework \\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release \\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release \\wsl$$\Ubuntu\home\master\msu\c_homework\clion_release\CMakeFiles\homework_6.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/homework_6.dir/depend

