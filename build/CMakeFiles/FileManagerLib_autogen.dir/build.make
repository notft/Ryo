# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luca/Documents/Code/Ryo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luca/Documents/Code/Ryo/build

# Utility rule file for FileManagerLib_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/FileManagerLib_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/FileManagerLib_autogen.dir/progress.make

CMakeFiles/FileManagerLib_autogen: FileManagerLib_autogen/timestamp

FileManagerLib_autogen/timestamp: /usr/lib/qt6/moc
FileManagerLib_autogen/timestamp: /usr/lib/qt6/uic
FileManagerLib_autogen/timestamp: CMakeFiles/FileManagerLib_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/luca/Documents/Code/Ryo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target FileManagerLib"
	/usr/bin/cmake -E cmake_autogen /home/luca/Documents/Code/Ryo/build/CMakeFiles/FileManagerLib_autogen.dir/AutogenInfo.json Debug
	/usr/bin/cmake -E touch /home/luca/Documents/Code/Ryo/build/FileManagerLib_autogen/timestamp

CMakeFiles/FileManagerLib_autogen.dir/codegen:
.PHONY : CMakeFiles/FileManagerLib_autogen.dir/codegen

FileManagerLib_autogen: CMakeFiles/FileManagerLib_autogen
FileManagerLib_autogen: FileManagerLib_autogen/timestamp
FileManagerLib_autogen: CMakeFiles/FileManagerLib_autogen.dir/build.make
.PHONY : FileManagerLib_autogen

# Rule to build all files generated by this target.
CMakeFiles/FileManagerLib_autogen.dir/build: FileManagerLib_autogen
.PHONY : CMakeFiles/FileManagerLib_autogen.dir/build

CMakeFiles/FileManagerLib_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FileManagerLib_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FileManagerLib_autogen.dir/clean

CMakeFiles/FileManagerLib_autogen.dir/depend:
	cd /home/luca/Documents/Code/Ryo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luca/Documents/Code/Ryo /home/luca/Documents/Code/Ryo /home/luca/Documents/Code/Ryo/build /home/luca/Documents/Code/Ryo/build /home/luca/Documents/Code/Ryo/build/CMakeFiles/FileManagerLib_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/FileManagerLib_autogen.dir/depend

