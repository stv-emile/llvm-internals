
### What is CMake

- **Cmake is cross platform build system generator.**
- **It is used to generate build system for different platform.**
### What is a Build System
A build system decribes how to build a project's executables and libraries from its source using a build tool to automate a process.

eg. of build system: 
- A Makefile for use with Make
- A project file for use with IDE.

### Why use CMake
To avoid maintaining multiple such build systems for a project:
**CMake is used to specify abstractly its build system through a set a files written in CMake language.**

### How CMake achieve that
A set of files written in CMake language, Specify abstractly the build system of the project.
CMake use a backend (a generator) to generate a preferred (concerte) build system locally for each user.

### What is required
To generate a build system with CMake, the following must be selected.
- Source tree
- Build tree
- Generator

Source tree: A top level directory containing source files porvided by the project. (Having CmakeLists.txt == build target and dependencies)

Build tree: A top level directory in which build system files and build output artifacts (executables and libraries) are to be stored. (Having CMakeCache.txt == persistent info and build system config options)

Generator: this chooses the kind of build system to generate.(-G Ninja, ...)