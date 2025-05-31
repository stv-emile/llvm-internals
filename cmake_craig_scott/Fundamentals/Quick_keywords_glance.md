CMake is a powerful, open-source build system generator that uses a variety of keywords to define and control the build process. Below is a concise overview of the most commonly used CMake keywords, grouped by their purpose. These keywords are used in `CMakeLists.txt` files or CMake scripts to configure projects, set variables, manage dependencies, and generate build files.

### 1. **Core Project Keywords**
- `cmake_minimum_required(VERSION <version>)`: Specifies the minimum CMake version required.
- `project(<name> [LANGUAGES <lang>...])`: Defines the project name and supported languages (e.g., C, CXX).
- `set(<variable> <value>)`: Sets a variable to a specified value.
- `unset(<variable>)`: Removes a variable.
- `option(<name> <description> <default>)`: Defines a user-configurable option with a default value.
- `include(<file>)`: Includes another CMake file or module.
- `add_subdirectory(<dir>)`: Adds a subdirectory containing a `CMakeLists.txt` to the build.

### 2. **Target Definition Keywords**
- `add_executable(<name> <sources>...)`: Defines an executable target with source files.
- `add_library(<name> [STATIC | SHARED | MODULE] <sources>...)`: Defines a library target (static, shared, or module).
- `target_include_directories(<target> [PUBLIC | PRIVATE | INTERFACE] <dirs>...)`: Specifies include directories for a target.
- `target_link_libraries(<target> [PUBLIC | PRIVATE | INTERFACE] <libs>...)`: Links libraries to a target.
- `target_compile_definitions(<target> [PUBLIC | PRIVATE | INTERFACE] <defs>...)`: Adds preprocessor definitions to a target.
- `target_compile_options(<target> [PUBLIC | PRIVATE | INTERFACE] <options>...)`: Specifies compiler options for a target.
- `target_sources(<target> [PUBLIC | PRIVATE | INTERFACE] <sources>...)`: Adds source files to a target.

### 3. **File and Directory Keywords**
- `file(GLOB [RECURSIVE] <var> <pattern>...)`: Collects files matching a pattern into a variable.
- `file(COPY <files>... DESTINATION <dir>)`: Copies files to a specified directory.
- `file(MAKE_DIRECTORY <dirs>...)`: Creates directories.
- `configure_file(<input> <output>)`: Copies and configures a file, substituting variables.

### 4. **Control Flow Keywords**
- `if(<condition>) ... elseif(<condition>) ... else() ... endif()`: Conditional execution.
- `foreach(<var> <items>...) ... endforeach()`: Iterates over a list of items.
- `while(<condition>) ... endwhile()`: Executes a loop while a condition is true.
- `function(<name> [<args>...]) ... endfunction()`: Defines a reusable function.
- `macro(<name> [<args>...]) ... endmacro()`: Defines a macro (text substitution).

### 5. **Build Configuration Keywords**
- `add_definitions(<definitions>...)`: Adds global preprocessor definitions.
- `add_compile_options(<options>...)`: Adds global compiler options.
- `set(CMAKE_BUILD_TYPE <type>)`: Sets the build type (e.g., Debug, Release).
- `enable_testing()`: Enables testing support for the project.
- `add_test(NAME <name> COMMAND <command>...)`: Defines a test.

### 6. **Finding and Including Dependencies**
- `find_package(<package> [COMPONENTS <components>...])`: Locates and loads settings for an external package.
- `find_library(<var> <names>...)`: Finds a library and stores its path in a variable.
- `find_path(<var> <names>...)`: Finds a directory containing a specified file.
- `find_file(<var> <names>...)`: Finds a file and stores its path.

### 7. **Installation and Packaging Keywords**
- `install(TARGETS <targets>... DESTINATION <dir>)`: Specifies installation rules for targets.
- `install(FILES <files>... DESTINATION <dir>)`: Installs specified files.
- `install(DIRECTORY <dirs>... DESTINATION <dir>)`: Installs directories.
- `include(CPack)`: Enables CPack for generating installers or packages.

### 8. **Property Management Keywords**
- `set_property(<scope> <target> PROPERTY <name> <value>)`: Sets a property on a target, directory, or global scope.
- `get_property(<var> <scope> <target> PROPERTY <name>)`: Retrieves a property’s value.
- `define_property(<scope> PROPERTY <name> [BRIEF_DOCS <doc>])`: Defines a custom property.

### 9. **Message and Debugging Keywords**
- `message([STATUS | WARNING | FATAL_ERROR] <text>...)`: Outputs messages to the console.
- `string(<operation> <output> <input>...)`: Performs string operations (e.g., CONCAT, REPLACE).
- `list(<operation> <output> <input>...)`: Manipulates lists (e.g., APPEND, REMOVE_ITEM).

### 10. **Generator Expressions**
- `$<...>`: Used for conditional logic in build commands (e.g., `$<CONFIG:Debug>`).
- `$<TARGET_FILE:<target>>`: Resolves to the target’s output file path.
- `$<IF:cond,true_val,false_val>`: Conditional evaluation in generator expressions.

### Notes
- **Case Sensitivity**: CMake keywords are case-insensitive (e.g., `SET` and `set` are equivalent).
- **Documentation**: For detailed information, refer to the official CMake documentation (e.g., `cmake --help-command <keyword>` or online at cmake.org).
- **Custom Commands**: You can define custom commands with `add_custom_command` and `add_custom_target` for specialized build steps.

If you need detailed explanations or examples for specific keywords, or if you want me to generate a sample `CMakeLists.txt` for a particular use case, let me know!