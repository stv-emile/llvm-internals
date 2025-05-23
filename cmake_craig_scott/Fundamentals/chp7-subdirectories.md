Most real world projects split their files across multiple directories.The way the project is structured also impacts the build system.

Two fundamental CMake commands in any multi-directory:
- add_subdirectories()
- include()

They bring content from another file or directory into the build.

Advantages:
- Build logic is localized (build characteritics stay in relevant dir)
- Builds can be composed of subcomponents (if use git submodule, ...)
- directories can be self-contained (turn off/on a part of a build)

KEEP-IN-MIND: 
strengths and weaknesses of include() and add_subdirectory()


## 7.1. add_subdirectory()

- Allows a project to bring another directory into the build.
- The directory must have its own CMakeLists.txt.
- CMakeLists.txt processed when add_subdirectory is called.
- A corresponding directory will be created in the build tree.

```js
add_subdirectory(sourceDir [binaryDir]
      [EXCLUDE_FROM_ALL]
      [SYSTEM]   # Requires CMake 3.25 or later
)
```
- Not mandatory to have sourceDir a subdirectory within the source tree.
- sourceDir can be absolute path or relative path to source tree.

- binaryDir not specified => by default build_Tree/sourceDir 
- binaryDir can be absolute path or relative path to binaryDir
- binaryDir mandatory if SourceDir is outside the source tree.

- EXCLUDE_FROM_ALL keyword not in project’s ALL target by default

## 7.1.1. Source And Binary Directory Variables
Provided by CMake
Variables which keep track of the source and binary directories for the CMakeLists.txt file currently being processed.
Read-only variables -  updated automatically as each file is processed by CMake.
Always contain absolute paths:

- `CMAKE_SOURCE_DIR`: 
  The top-most directory of the source tree - never changed.

- `CMAKE_BINARY_DIR`:
  The top-most directory of the build tree - never changed.

- `CMAKE_CURRENT_SOURCE_DIR`:
  The directory of the CMakeLists.txt file currently being processed.
  updated at each add_subdirectory.

- `CMAKE_CURRENT_BINARY_DIR`
  The build directory corresponding to the CMakeLists.txt file currently being processed.
  updated at each add_subdirectory.


### 7.1.2. Scope
add_subdirectory() called ==> a new scope is create for processing subdirectory’s CMakeLists.txt file.
The new scope acts like a child of the calling scope.
- calling scope variable copied to child scope 
- child scope new variable is not be visible to the calling scope.
- change in child scope variable is local to child scope.
- unset child scope variable (the copied also) - not unset in the calling scope.

Scope is an important characteristic of add_subdirectory().
It allows the added directory to change whatever variables it wants without affecting variables in the calling scope. 

### 7.1.3. When To Call project()
- Should we call project() in the CMakeLists.txt files of subdirectories.
- In most cases, it is not necessary or desirable to do so, but it is permitted but extra file are generated.
- The only place where project() must be called is the top-most CMakeLists.txt file.

## 7.2. include()
pulling in content from other directories
```js
  include(fileName [OPTIONAL] [RESULT_VARIABLE myVar] [NO_POLICY_SCOPE])
  include(module   [OPTIONAL] [RESULT_VARIABLE myVar] [NO_POLICY_SCOPE])
```

The first form is somewhat analogous to add_subdirectory().
`include(fileName [OPTIONAL] [RESULT_VARIABLE myVar] [NO_POLICY_SCOPE])`
Important differences to add_subdirectory():
- include() expects the name of a file to read in / add_directro:  a folder
- expect file of .cmake as extension / add_directory: a CMakeList.txt in the folder.
- policy scope can be diable in include / a new policy scope always by default on add_subdirectory
- CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR don not change when processing the file named by include().

The second form serves an entirely different purpose - It is used to load the named module (Chp11)
`include(module   [OPTIONAL] [RESULT_VARIABLE myVar] [NO_POLICY_SCOPE])`


CMAKE_CURRENT_SOURCE_DIR does not change when include() is called
=> CMAKE_CURRENT_SOURCE_DIR contain the location of the caller file -  not the directory containing the included file.

Additional set of variables provided by CMake to get the included files directory:
- CMAKE_CURRENT_LIST_DIR: updated when processing the included file, absolute path
- CMAKE_CURRENT_LIST_FILE: the name of the file currently being processed, absolute path
- CMAKE_CURRENT_LIST_LINE: the line number of the file currently being processed (for debugging)

IMPORTANT
They work for any file being processed by CMake, not just those pulled in by an include() command.


## 7.3. Project-relative Variables
Problematic:
Scalability in directory name - when project imported as subproject or sumodule with git.
project() command sets some variables that provide a much more robust way of defining paths relative to locations in the directory hierarchy.
Variables available after project() has been called at least once:

`PROJECT_SOURCE_DIR`: 
source directory of the most recent call to project() in the current scope or any parent scope. 

`PROJECT_BINARY_DIR`:
build directory corresponding to the source directory defined by PROJECT_SOURCE_DIR.

`projectName_SOURCE_DIR`:
The source directory of the most recent call to `project(projectName)` in the current scope or any parent scope

`projectName_BINARY_DIR`:
The build directory corresponding to the source directory defined by projectName_SOURCE_DIR.


- Some parts of the project might only make sense if it is the top of the build, such as setting up packaging support. 
- Some hierarchical build arrangements allow a project to be built either stand-alone or as part of a larger parent project (see Chapter 30, FetchContent). 

A project can detect whether it is the top level:
```sh
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
      add_subdirectory(packaging)
endif()
```
or 
```sh
# Requires CMake 3.21 or later
  if(PROJECT_IS_TOP_LEVEL)
      add_subdirectory(packaging)
endif()
```
A similar variable, `<projectName>_IS_TOP_LEVEL`, is also defined by CMake 3.21 or later for every call to project().

## 7.4. Ending Processing Early
stop processing the remainder of the current file and return control back to the caller.
don't use to return values to caller scope. use it in function.
```sh
  return(PROPAGATE x y)
```

## 7.5. Recommended Practices
- most simple projects are probably better off preferring to use add_subdirectory() over include().
- establish the habit of using CMAKE_CURRENT_LIST_DIR early
- avoid using the CMAKE_SOURCE_DIR and CMAKE_BINARY_DIR variables
- prefer using PROJECT_SOURCE_DIR and PROJECT_BINARY_DIR, or projectName_SOURCE_DIR and projectName_BINARY_DIR
- Avoid using the PROPAGATE keyword with return() statements that end processing of the current file.
- where multiple inclusion of a file must be prevented, > CMake 3.10, prefer to use the include_guard() command without arguments instead of an explicit if-endif block.
- Avoid the practice of arbitrarily calling project() in the CMakeLists.txt of every subdirectory.

