
## Two syntax to configure LLVM
### Basic in-source build
OBJ_ROOT=$(pwd)/build_llvm
SRC_ROOT=$(pwd)/llvm-project/llvm
LLVM_INSTALL_DIR=/usr/local/llvm

```bash
cmake -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local/llvm \
  $SRC_ROOT
```
$SRC_ROOT is the directory with your CMakeLists.txt
The project is configured in the current directory - build system is generated.

### Out-of-source Build (Recommended for LLVM)
```bash
cmake -G "Unix Makefiles" \
  -S $SRC_ROOT \
  -B $OBJ_ROOT \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$LLVM_INSTALL_DIR
```
$SRC_ROOT is the directory with your CMakeLists.txt
$OBJ_ROOT is where the project is configured - build system is generated.


>[!Warning]
>Ambiguiy fixed:
>cmake [<options>] -B <path-to-build> [-S <path-to-source>]
>cmake [<options>] <path-to-source | path-to-existing-build>
>path-to-source: cmake will configure the source into the current directory.
>path-to-existing-build: cmake will reconfigure the existing build into the current directory.
>in the latter case, cmake resolve the source thanks to the metadata in the existing build directory.

## How to build LLVM configured

### Cross platform
```bash
cmake --build $OBJ_ROOT
```

### Usage of the build tool
```bash
ninja -C $OBJ_ROOT
```

## Building a Standalone LLVM (A component with and existing installation)
We will build an LLVM first and build clang as standalone
### Variable initialization
```bash
build_llvm=`pwd`/build-llvm
build_clang=`pwd`/build-clang
installprefix=`pwd`/install

# Where llvm repository is cloned
llvm=`pwd`/llvm-project
```

### Build and Install LLVM
```bash
cmake -G ninja \
  -S $llvm \
  -B $build_llvm \
  -DLLVM_INSTALL_UTILS=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$installprefix
cmake --build $build_llvm --target install
# or
ninja -C $build_llvm install
```

### Configure and  Clang
```bash
# subproject is $llvm/clang
cmake -G ninja \
  -S $llvm\clang
  -B $build_clang
  -DLLVM_EXTERNAL_LIT=$build_llvm/utils/lit \
  -DLLVM_ROOT=$installprefix

cmake --build $installprefix --target install
```
LLVM_ROOT point to the previously installed LLVM

*Note: --target install run the script camke_install.cmake from the build directory.*


## Options in configuration commonly used.

- -DLLVM_ENABLE_PROJECTS="...": Semicolon Separated list of the LLVM subprojects (clang, lld, etc)
- -DCMAKE_INSTALL_PREFIX=directory: Full pathname of where you want the LLVM tools and libraries to be installed
- -DCMAKE_BUILD_TYPE=type: Controls optimization level and debug information of the build. (Debug, Release, RelWithDebInfo, MinSizeRel)
- -DLLVM_ENABLE_ASSERTION=ON : Compile with assertion checks enabled
- -DLLVM_USE_LINKER=ldd : Link with the lld linker
- -DLLVM_PARALLEL_{COMPILE,LINK,TABLEGEN}_JOBS=N : Limit the number of compile/link/tablegen jobs running in parallel at the same time. This is especially important for linking since linking can use lots of memory.


