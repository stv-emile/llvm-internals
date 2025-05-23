

Relatively straightforward to define a simple executable in CMake.
Define a target name for the executable and list the source files to be compiled:
```
add_executable(MyApp main.cpp)
```
This output a basic console executable
- CMake also allows the developer to define other types of executables (Bundle, GUI
- CMake allow developper to build and link libraries => it supports a few different kinds of libraries, including static, shared, modules and frameworks.)
- CMake also offers very powerful features for managing dependencies between targets and how libraries are linked

## 4.1 Executables
```
add_executable(targetName [WIN32] [MACOSX_BUNDLE]
    [EXCLUDE_FROM_ALL]
    source1 [source2 ...]
)
```
- WIN32 :  
  - When building the executable on a Windows platform, ingnored on other platform.
  - It instructs CMake to build the executable as a **Windows GUI** application.
  - Executable will be created with a `WinMain()` entry point instead of just `main()`
  - Exectutable will be linked with `/SUBSYSTEM:WINDOWS` option/

- MACOSX_BUNDLE:
  - When building on an Apple platform (macOS, iOS, ...), ignored on non-apple platforms.
  - It directs CMake to build an app bundle 

- EXCLUDE_FROM_ALL
When no target is specified at build time, the default ALL target is built (ALL_BUILD on Xcode).
  - If an executable is defined with the EXCLUDE_FROM_ALL option, it will not be included in that default ALL target.
  - The executable will then only be built if:
    - it is explicitly requested by the build command
    - it is a dependency for another target that is part of the default ALL build

[More on targets](Chapter 18, Target Types.)

## 4.2 Defining Libraries
The most basic form of defining Library targets:
```
add_library(targetName [STATIC | SHARED | MODULE]
    [EXCLUDE_FROM_ALL]
    source1 [source2 ...]
)
```
- The name of the built library on the file system is derived from this name by default.
- EXCLUDE_FROM_ALL prevent the library from being included in the default ALL target.
- STATIC, SHARED, MODULE specify the type of library to be built:

**STATIC**: Specifies a static library or archive.
    Windows: targetName.lib
    Unix: libtargetName.a
**SHARED**: Specifies a shared or dynamically linked library.
    Windows: targetName.dll
    Apple Platforms: libtargetName.dylib
    Unix: libtargetName.so
**MODULE**: Specifies a library like shared library, but intended to be loaded dynamically at run-time rather than being linked directly to a library or executable.

> [!Important]
> It is possible to omit the keyword defining what type of library to build.
> If the project doesn't specifically requires a particular type of library, the best practice is to not specify it and leave the choice up to the developer when building the project.
> When calling cmake: `cmake -DBUILD_SHARED_LIBS=Yes /path/to/source`
> In CMakeLists.txt: `set(BUILD_SHARED_LIBS YES)` before any `add_library()` command (less flexible).

[Another type of pseudo-library is also supported](Chapter 18, Target Types.)

## 4.3 Linking Targets
Simplistic dependency relationship: library A need Library B.
Different types of dependency relationships between libraries:
- PRIVATE
- PUBLIC
- INTERFACE

**PRIVATE**: Private dependencies specify that library A uses library B in its own internal implementation.
- Anything else that links to library A doesn't need to know about B because it is an internal implementation detail of A.

**PUBLIC**: Public dependencies specify that not only does library A use library B internally, it also uses B in its interface.
- A cannot be used without B, so anything that uses A will also have a direct dependency on B. 

**INTERFACE**: Interface dependencies specify that in order to use library A, parts of library B must also be used. 
- This differs from a public dependency in that library A doesn’t require B internally, it only uses B in its interface. 
- Example : Header-only library [](Section 18.2.4, “Interface Libraries”).
  
The general form of the command:
```
target_link_libraries(targetName
    <PRIVATE | PUBLIC | INTERFACE> item1 [item2 ...]
    <PRIVATE | PUBLIC | INTERFACE> item3 [item4 ...]
)
```
Why:
- This allows projects to precisely define how one library depends on others.
- CMake then takes care of managing the dependencies throughout the chain of libraries linked in this fashion.

