Developped by Kitware.


# Variable and Cache debugging
To inspect cache and variables:
    Read Cache variables
        -L  : list cache only
        -LA : list advanced cache
        -LAH: list advanced cache with comments

# Interactive Variable and Cache debuggin
Inspect with other interface:
    ccmake -B build -S source : TUI to inspect
    cmake-gui : GUI to inspect

# Logging
--log-level=<message-mode> : message can be <ERROR|WARNING|NOTICE|STATUS|VERBOSE|DEBUG|TRACE>.  
                            Sets the verbosity level for messages emitted by CMake - message(<message-mode> MESSAGE)
--log-context : Prefix each message with where it came from.

# Debugging - to see what's happen (Useful fo CMake Developper only):

    --debug-output : Enable debugging output for the entire cmake configuration process while processing the CMakeLists.txt files (for all commands.)   
    --debug-find : --debug-output, but only for find_* commands and find_program.    
    --debug-find-vars=<var_name> : --debug-output, for the variable var_name only that are used in find_* context.
    --debug-find-pkg=<pkg-name> : --debug-output, for package pkg_name only  that are used in find_* context.
    
    --debug-trycompile : To keep temporary directories and file created during the try-compile checks.- by default in `CMakeFiles/CMakeTmp/`

    Why it is useful to detect painful problems ?
        - You have problems from files that doesnt exist in the directories (source code and compiler output)
        - You have trouble during the CMake configuration step.
        - You see message like `check for checking C compiler failed`, Performing Test XYZ failed 
        - You want to inspect the source code, CMakeLists.txt, and build log that CMake uses to check system capabilities, compiler flags, or feature availability.
       
# Tracing - for reverse engineering
    --trace : Logs every command executed in every CMake file (To understand cmake execution flow.)
    --trace-expand : --trace + expanding all variable values (To debug issue with variable value, e.g which values in SRC_FILES)
    
    --trace-source=<file> : To specify with file trace  (For focus debugging)
    --trace-format=<fmt> : To specify the output format json-v1/human (For advanced debugging)
    --trace-redirect=<file> : redirect trace to a log/trace file (For later view) 



# Helpers to avoid go reading the documentation when debugging:
    Documentation of cmake defined **variables**:
        --help-variable <var> [<file>] : Documentation of a single variable.
        --help-variable-list [<file>] : Get a quick list of all variable names.
        --help-variables [<file>] : Comprehensive variable Documentation.
        hint: --help-variable-list | grep <VAR_PART> and --help-variable var for details.
                Use | less, to have TUI viewer and make esc + / to search on it.

    Documentation of cmake defined **properties**:
        --help-property <prop> [<file>] : Documentation of single prpoperty.
        --help-property-list [<file>] : Get a quick list of all Property names.
        --help-properties [<file>] : Comprehensive property Documentation.

    Documentation of cmake **policies**, **commands**, **modules** follows the same pattern

    There is a manual for each CMake's tool (e.g: cmake, ctest, cpack, ...) and maual for others components such as commands, packagers, gui, etc.
        --help-manual <manual_name> [<file>] : The manual 'manual_name'
        --help-manual-list [<file>] : Get Quick list of all manuals.
        --help-full [<file>] : Comprehensive Manual.

    For keywords (e.g add_executable, include, ...) used in CMakeLists.txt or in other list files:
        --help keyword [<file>]


# Strategy to navigate into documentation to be more effective:

Use 'grep' 'awk' 'less'

1 - To find specific keywords or options

- Keywords like 'EXCLUDE_FROM_ALL' with context.   
    ```sh
    # -i : case-insensitive, -C NUM : NUM line before and after for context.
    # -A , -B can be used for After and Before
    cmake --help-command add_library | grep -i "exclude_from_all" -C 3
    ``` 
- Finding options, you can look for brackets
    ```sh
    cmake --help-command add_library | grep '\['
    ```
- Extracting a limited part
    ```sh
    cmake --help-command add_library | head -n 12
    ```
- Extracting sections:
    ```sh
    cmake --help-command add_library | awk '/Object Libraries/{p=1} p; /Interface Libraries/{p=0}'
    ```
- Interactive search
    ```sh
    # pipe less, use ESC and /SEARCHING_WORD to search word, n/N to navigate next/next-reverse.
    cmake --help-command add_library | less
    ```
- Use the file arguement for later seach in vim or emacs.

# Profiling cmake configuration scripts
This can help understanding CMakeInternals, and optimize script for faster configuration in CI.
--profiling-output=<file>
 --profiling-format=<fmt>
```sh
# Getting profiling information (only google-trace is available)
cmake -B build --profiling-output=filename.json --profiling-format=google-trace
# Use perfetto to visualize it with : https://ui.perfetto.dev/
```