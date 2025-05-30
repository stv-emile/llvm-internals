
# Minimal Project

All CMake projects start with a file called *CMakeLists.txt* and it is expected to be placed *at the top of the source tree*. 

CMake defines its own language, and CMakeLists.txt and other cmake list files are written into this language.

A minimal well-formed CMakeLists.txt producing a basic executable.
```cmake
cmake_minimum_required(VERSION 3.2)
project(MyApp)
add_executable(MyApp)
```
- Command names are also case insensitive.
- command arguments are space separated.

## 3.1 Managing CMakeVersion
How cmake deal with project which used an older cmake verison ?

- CMake doesn't require all projects to immediately adapt to new fixes and features.

- CMake provides policy mechanisms which allow the project to say “Behave like CMake version X.Y.Z” To maintain the expected behavior of any particular past release.

Details about the expected CMake version behavior is specified by:
```bash
# Version details must have at least the major.minor part
cmake_minimum_required(VERSION major.minor[.patch[.tweak]])
```

This should be the first line of the CMakeLists.txt.   
It must be called before any CMake commands.

It does 2 things:
- **Specifies the minimum version of CMake the project needs.**   
  (**An older CMake processing such CMakeLists.txt ==> Halted with an error)
- **Enforces policy settings to match CMake behavior to the specified version.**


> Choose the most recent CMake version that won’t present significant problems for those building the project. 

## 3.2 The project command.

Every CMake project should contain a project() command.
it should be appeared after cmake_minimum_required() has been called.
Most used parameters.
```sh
# Must be in  the top directory CMakeLists.txt
project(projectName
    [VERSION major[.minor[.patch[.tweak]]]]
    [LANGUAGE languageName ...]
)
```

ProjectName is required.   
Name: [a-Z], [0-9], "_", "-" / no quotes required / no space accepted.   
Best practice: letter only.

- Name is the only mandatory argument.
- Optional VERSION details are only supported in CMake >3.0. ()
- Optional LANGUAGES argument defines the programming languages that should be enabled for the project.   
  Supported values : C, CXX, Fortran, ASM, CUDA and others.
  multiple languages - separate each with a space. 
- LANGUAGES NONE - indicates that no Languages are used. default is C CXX (in CMake >3.0)


>[!important]   
> The project() command does much more than just populate a few variables. One of its important responsibilities is to 
> - Check the compilers for each enabled language.
> - Ensure they are able to compile and link successfully. 

Problems with the compiler and linker setup are then caught very early. If success -  details Cached in build/CMakeCache.txt


[How to setup version](Chapter 21, Specifying Version Details)   
[Operations after checking build tools](Chapter 23, Toolchains And Cross Compiling)  
[](Chapter 7, Using Subdirectories)


## 3.3 Building A Basic Executable
Tell CMake to create an executable from a set of source files:
```sh
# create an executable.
add_executable(targetName source1 [source2 ...])
```
- Executable will be created in the build directory with a platform-dependent name based by default on the target name.
- The executable name can be customized with target properties. [Properties CMake feature](Chapter 9, Properties).
- Multiple executable with different names :  call `add_executable()` multiple times with different target names in the CMakeLists.txt
- **Same target name multiple time => fail.**

## 3.4 commenting
```
# this is a comment
```

## 3.5 Recommended practices

- Every CMake project has `cmake_minimum_required()` as first in the top level CMakeLists.txt file.
- If creating a project intended to be built and distributed as part of an operating system itself (common for Linux), the minimum CMake version is likely to be dictated by the version of CMake provided by that same distribution.
- Force thinking about project version numbers early and start incorporating version numbering into the project() command as soon as possible. 
- Consider popular practices such as Semantic Versioning when deciding on a versioning strategy.

[]( Section 18.2, “Libraries”)
target_...() commands enhance the dependency information carried between targets.

## 4.4. Linking Non-targets
Things that can also be specified as items in a `target_link_libraries()` in addition to CMake targets:

- Full Path to a library file:
  CMake will add the library file to the linker command. If the library file changes, CMake will detect that change and re-link the target.
  `target_link_libraries(targetName PRIVATE /usr/lib/libname.so)`

- Plain library name:
  If just the name of the library is given with no path, the linker command will search for that library. eg: foo -> -lfoo or foo.lib
  `target_link_libraries(targetName PRIVATE llvm.a)`

- Link flag:
  As a special case, items starting with a hyphen other than -l or -framework will be treated as flags to be added to the linker command. (Only for Private items)
  `target_link_libraries(targetName PRIVATE -lmylib)`


## 4.5. Old-style CMake

debug, optimized and general keywords:
- debug: item will only be added if the build is a debug build. 
- optimized: it me will only be added if the build is not a debug build.
- general: item should be added for all build configurations, default behavior.
These keywords should be avoid in New Projects.


Other deprecated syntax:

- No PRIVATE|PUBLIC|INTERFACE keyword. PUBLIC is by default
  If mixed with new cmake - PRIVATE by default.
`target_link_libraries(targetName item [item...])`

- Pre-cursor of INTERFACE Keyword
```
   target_link_libraries(targetName
      LINK_INTERFACE_LIBRARIES item [item...]
)
```

- Pre-cursor of PUBLIC/PRIVATE keyword
```
  target_link_libraries(targetName
      <LINK_PRIVATE|LINK_PUBLIC> lib [lib...]
     [<LINK_PRIVATE|LINK_PUBLIC> lib [lib...]]
  )

```

## 4.6. Recommended Practices

- Consider the project name and executable name as being separate
  eg. `project(projectName)` and `add_executable(targetName)`
- Set the project name directly rather than via a variable.
- choose a target name according to what the target does rather than the project it is part of.
- **When naming targets for libraries, resist the temptation to start or end the name with lib.**
- Unless there are strong reasons to do so, try to **avoid specifying the STATIC or SHARED keyword for a library** until it is known to be needed.
- Aim to always specify `PRIVATE`, `PUBLIC` and/or `INTERFACE` keywords when calling the target_link_libraries() command rather than following the old-style CMake syntax which assumed everything was PUBLIC.

