Functions and macros are the primary mechanism to extend CMake’s functionality and to encapsulate repetitive tasks in a natural way.

## 8.1. The Basics
Functions and macros in CMake have very similar characteristics.
- Functions introduce a new scope and the function arguments become variables accessible inside the function body. 
- Macros, effectively paste their body into the point of the call and the macro arguments are substituted as simple string replacements.

```
function(name [arg1 [arg2 [...]]])
    # Function body (ie. commands) ...
endfunction()

macro(name [arg1 [arg2 [...]]])
    # Macro body (ie. commands) ...
endmacro()

# called
name()
```
Names are case-insensitively.

## 8.2. Argument Handling Essentials
The argument handling of functions and macros is the same.
Except:
- functions: each argument is a CMake variable with all the usual behaviors of a CMake variable (if-Defined works)
- macro: macro arguments are string replacements (if-Defined doesn't work)

Important: even vars in macro are string substitution, they can be used with ${vars} 
```
  function(func myArg)
      message("myArg = ${myArg}")
  endfunction()
  macro(macr myArg)
      message("myArg = ${myArg}")
  endmacro()
```

functions and macros come with a set of automatically defined variables:
- ARGC : set to the total number of arguments passed to the function (named and unnamed)
- ARGV : list containing each of the arguments passed to the function (named and unnamed)
- ARGN : only contains arguments beyond the named ones
- ARGVx: such as ARGV0, ARGV1 ... are individual arguments referenced by their position num.

Use case:
```
  # Use a named argument for the target and treat all other
  # (unnamed) arguments as the source files for the test case
  function(add_mytest targetName)
      add_executable(${targetName} ${ARGN})
      target_link_libraries(${targetName} PRIVATE foobar)
      add_test(NAME    ${targetName}
               COMMAND ${targetName})
  endfunction()
  # Define some test cases using the above function
  add_mytest(smallTest small.cpp)
  add_mytest(bigTest   big.cpp algo.cpp net.cpp)
```
be cautious: macros treat their arguments as string substitutions rather than as variables, if ARGN is used in a place where a variable name is expected, the variable it will refer to will be in the scope of the the macro caller, not the ARGN from the macro’s own arguments.


## 8.3. Keyword Arguments

flexibility of user-defined functions and macros to contain any number of argument secetions in any order.
like:
```sh
target_link_libraries(targetName
    <PRIVATE|PUBLIC|INTERFACE> item1 [item2 ...]
    <PRIVATE|PUBLIC|INTERFACE> item3 [item4 ...]
)
```
It is achieved by `cmake_parse_arguments()` command.
- first form for functions and macros (supported by all CMake versions).
```sh
include(CMakeParseArguments)
cmake_parse_arguments(
    prefix
    valuelessKeywords singleValueKeywords multivalueKeywords
    argsToParse...
)
```

- second form for functions only (supported by cmake >3.7)
```sh
cmake_parse_arguments(
    PARSE_ARGV startIndex
    prefix
    valuelessKeywords singleValueKeywords multivalueKeywords
)
```

Differing only in the way they take in the set of arguments to parse:
- first form: argsToParse will is given as ${ARGN} without quotes. All arguments given to the enclosing function or macro.
  
- PARSE_ARGV option tells cmake_parse_arguments() to read the arguments directly from the set of ${ARGVx} variables, with x ranging from startIndex to (ARGC - 1).
  (it robustly handles the corner cases that the first form does not.)
  If there are no named arguments for the enclosing function, then passing ${ARGV} or ${ARGN} to the first form is equivalent to giving PARSE_ARGV 0 to the second form when none of the corner cases apply.


### 8.4.1. Returning Values From Functions

return values by specifying variables to propagate to the caller. 
- For each variable name listed after PROPAGATE, that variable will be updated in the calling scope to have the same value as in the function at the point of the return() call. 
- The CMP0140 policy must be set to NEW