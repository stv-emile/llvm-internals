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
This can be achieved by `cmake_parse_arguments()` command.
- first form for *functions and macros* (supported by all CMake versions).
```sh
include(CMakeParseArguments)
cmake_parse_arguments(
    prefix
    valuelessKeywords singleValueKeywords multivalueKeywords
    argsToParse...
)
```

- second form for *functions only* (supported by cmake >3.7)
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




 ## 8.4. Returning Values
 A fundamental difference between functions and macros is that functions introduce a new variable
 scope, whereas macros do not.    


### 8.4.1. Returning Values From Functions

return values by specifying variables to propagate to the caller. 
- For each variable name listed after PROPAGATE, that variable will be updated in the calling scope to have the same value as in the function at the point of the return() call. 
- The CMP0140 policy must be set to NEW.

```sh
# This ensures that we have a version of CMake that supports
 # PROPAGATE and that the CMP0140 policy is set to NE
cmake_minimum_required(VERSION 3.25)

function(doSomething outVar)
    set(${outVar} 42)
    return(PROPAGATE ${outVar})
endfunction()

doSomething(result)
 # Here, a variable named result now holds the value 42
```sh
To give the caller full controll of what the function does and to avoid the caller variables unxepectedly overwriten,
function arguments should be used to tell the function the names of variables to be set in the calling
 scope.
```


- The return() statement propagates variables to the calling scope. No block statement prevent propagation to the function's caller.
```
cmake_minimum_required(VERSION 3.25)

function(doSomething outVar)
    set(${outVar} 42)
    block()
        set(${outVar} 27)
        return(PROPAGATE ${outvar})
    endblock()    
endfunction()

dosomething(result)
# Here, a variable named result now holds the value 27
```` 



- With CMake 3.24 and earlier, use alternative

```sh
function(func resultVar1 resultVar2)
    set(${resultVar1} "First result" PARENT_SCOPE)
    set(${resultVar2} "Second result" ¨PARENT_SCOPE)
endfunction()
func(myVar otherVar)
```


###  8.4.2. Returning Values From Macros

How ?
- specifying the names of variables to be set by passing them in as arguments. 
- t the PARENT_SCOPE keyword should not be used within the macro when calling set()

Important:
In fact, about the only reason one would use a macro instead of a function is if many variables need to be set/unset in the calling scope.

example:
```sh
macro(func resultVar1 resultVar2)
    set(${resultVar1} "First result")
    set(${resultVar2} "Second result")
endmacro()
```

cautious:
Recall that a macro effectively pastes its commands at the call site. Any return() statement from a macro will actually be returning from the scope of whatever called the macro, not from the macro itself. 

```sh
macro(inner)
    message("From inner")
    return() # Usually dangerous within a macro
    message("Never printed")
endmacro()

function(outer)
    message("From outer before calling inner")
    inner()
    message("Also never printed")
endfunction()

outer()
```

## Overriding commands:

```sh
function(printme)
   message("Hello from first")
endfunction()

function(printme)
    message("Hello from second")
   # use _printme() to call the older implementation
endfunction()
```
Undocumented behaviour of cmake:
- The first implementation of ``printme` is created and made available as a command of that name.

- The second implementation of `printme` is encountered. 
    - CMake finds an existing command by that name
    - so CMake defines the name `_printme` to point to the old command 
    - And it sets `printme` to point to the new definition.

Cautious.
- Any third defintion will do the same and the most recent older definition will be renamed.
- Don't use redefinition if you function will call the older function.


## Special variables for functions

Variables to assist with debugging and implementing functions.
They are available during execution of a function:

- CMAKE_CURRENT_FUNCTION:
    Holds the name of the function currently being executed.

- CMAKE_CURRENT_FUNCTION_LIST:
    Contains the full path to the file that defined the function currently being executed.

- CMAKE_CURRENT_FUNCTION_LIST_DIR:
    Holds the absolute directory containing the file that defined the function currently being executed.
    Different to  CMAKE_CURRENT_LIST_DIR - directory of the file where the function is called.

- CMAKE_CURRENT_FUNCTION_LIST_LINE:
    Hold the line number at which the currently executing function was defined within the file that defined it.

Example:
using configure_file() command to copy a file from the same directory as the file defining the function.
```sh
function(writeSomeFile toWHere)
    configure_file(${CMAKE_FUNCTION_LIST_DIR}/template.cpp.in ${toWhere} @ONLY)
endFunction()
```

The CMAKE_CURRENT_FUNCTION… variables are only updated for functions.

##  8.7. Other Ways Of Invoking CMake Code

cmake_language() command which can be used to invoke arbitrary CMake code directly without having to define a function or macro.

it isn’t designed to replace functions or macro.

The two sub-commands provided by CMake 3.18 are :
- CALL : `cmake_language(CALL command [args...])`
- EVAL CODE: `cmake_language(EVAL CODE code...)`

CALL sub-command invokes a single CMake command, with arguments if required.
Certain built-in commands cannot be invoked this way:  commands that start or end a block (e.g. `if()`/`endif()`, `foreach()`/`endforeach()`).
```sh
function(qt_generate_moc)
    set(cmd qt${QT_DEFAULT_MAJOR_VERSION}_generate_moc) # assume this qt variable is defined.

    cmake_language(CALL ${cmd} ${ARGV})
endfunction()

```

EVAL CODE sub-command supports executing any valid CMake script.
it does not interfere with variables that get updated inside a function invocation.
The variables are evaluated only when cmake_language() is called.
So EVAL CODE can be used for repeated code in functions.
only use it from inside a function.

example
```sh
set(myProjTraceCall [=[
    message("Called ${CMAKE_CURRENT_FUNCTION})
    set(__x 0)
    while(__x LESS ${ARGC})
        message(" ARGV${__x} = ${ARGV${__x}}")
        math(EXPR __x "${__x} + 1")
    endwhile()
    unset(__x)
]])

function(func)
    cmake_language(EVAL CODE "${myProjTraceCall}")
    # ...
endfunction()

func(one two three)
```

CMake 3.19 added the DEFER set of sub-commands.
These allow a command to be queued for execution at a later time and to manage the set of currently queued commands. 

```sh
cmake_language(DEFER
    [DIRECTORY dir]
    [ID id | ID_VAR outVar]
    CALL command [args...]
)
```
- command and its arguments will be queued for execution at the end of the current directory scope.
-  The DIRECTORY option can be given to specify a different directory scope instead. (dir directory must already be known to CMake and it must not already have finished being processed.)

Example:
```sh
cmake_language(DEFER
    CALL message "End of current scope processing"
)

cmake_language(DEFFER
    DIRECTORY ${CMAKE_SOURCE_DIR}
    CALL message "End of top level processing"
)
```

Useful:
Each queued command has an identifier associated with it. 
Multiple commands can be associated with the same identifier to allow them to be manipulated as a group.
- The ID_VAR option can be used to capture the assigned identifier.
- which can then be reused in later calls with the ID option ti add more command to the same identifier.

```sh
cmake_language(DEFER
    ID_VAR deferredID
    CALL message "First deffered command"
)

cmake_language(DEFER
    ID ${deferredID})
    CALL message "Second deferred command
```

Other DEFER subcommands:
- To get a list of the identifiers for all commands currently queued for the specified or current directory scope.
`cmake_language(DEFER [DIRECTORY dir]) GET_CALL_IDS outVar)`
- To get the first command and its arguments associated with the specified id.(Only the first command)
`cmake_language(DEFER [DIRECTORY dir] GET_CALL id outVar)`
- To discard all deferred commands associated with any of the specified identifiers.
`cmake_language(DEFER [DIRECTORY dir] CANCEL_CALL ids...)`


What to consider before using DEFER:
- Special rules apply to variable expansion within deferred commands and their arguments.(see section 8.8.3)
- Deferred commands make it harder for developers to follow the flow of execution.
- In deferring commands, the project may be making assumptions about what can happen between the deferral and the execution of the commands.
- Deferred commands can be a sign of a project’s CMake API trying to do too much in one function or macro


##  8.8. Problems With Argument Handling

Arguments are separated by spaces and consecutive spaces are treated as a single argument separator.

```sh
someCommand(a b c)
someCommand(a    b   c)
```
Equivalent to
```sh
 someCommand(a b;c)
 someCommand(a;;;;b;c)

```

Important:
- Where an argument needs to contain embedded spaces or semicolons, quoting must be used:
```sh
    someCommand(a "b b" c)
    someCommand(a "b;b" c)
    someCommand(a;"b;b";c)
```

- When **variable evaluation** is involved and **arguments are not quoted**, space and semicolons differ.
```sh
set(containsSpace     "b b")
set(containsSemiColon "b;b")

someCommand(a ${containsSpace} c)  # three arguments being passed
someCommand(a ${constainsSemiColon} c) # four argument being passed
```
Spaces only act as argument separators before any variable evaluation is performed.

- OBSERVATION 1
When they are the result of variable evaluations, spaces are never discarded and never act as argument separators.

- OBSERVATION 2
One or more semicolons at the start or the end of an unquoted argument are discarded.

- OBSERVATION 3
Consecutive semicolons not at the start or end of an unquoted argument are merged and act as a single argument separator.


### 8.8.1. Parsing Arguments Robustly

Quoting in cmake_parse_arguments():
- Always quote keyword lists ("${noValues}", "${singleValues}", "${multiValues}") to treat them as single arguments.
- Leave ${ARGV} or ${ARGN} unquoted in the first form to allow argument separation.

Issues with First Form:
- Using ${ARGV} in the first form can drop empty arguments (e.g., a;;c becomes a;c) or flatten lists (e.g., "a;b;c" becomes a;b;c).


Use PARSE_ARGV for Robustness (The second form):
- Use PARSE_ARGV <startIndex> (CMake ≥ 3.7) in functions to preserve empty arguments and list structures.

###  8.8.2. Forwarding Command Arguments

- Naive forwarding issue:
Using ${ARGN} to forward arguments flattens lists (e.g., "a;b;c" becomes a;b;c), losing the original structure.

- Use PARSE_ARGV for List Preservation:
Use cmake_parse_arguments(PARSE_ARGV 0 FWD "" "" "") to forward arguments while preserving list structures (e.g., "a;b;c" stays as one argument).

- Handling Empty Arguments:
To preserve empty arguments, use cmake_language(EVAL CODE) with quoted arguments([===[${arg}]===])
```sh
# e.g.
function(outer)
    cmake_parse_arguments(PARSE_ARGV 0 FWD "" "" "")
    set(quotedArgs "")
    foreach(arg IN LISTS FWD_UNPARSED_ARGUMENTS)
        string(APPEND quotedArgs " [===[${arg}]===]")
    endforeach()
    cmake_language(EVAL CODE "inner(${quotedArgs})")
endfunction()
```

- Macro limitation:
PARSE_ARGV is not available for macros. Use string(REPLACE) with cmake_language(EVAL CODE) to preserve empty strings, but list structures will still be flattened.


###  8.8.3. Special Cases For Argument Expansion


## Recomended practices

-  In general, prefer to use functions rather than macros - not to alter variables in caller.
-  Used Macro only where the contents of the macro body really do need to be executed within the scope of the caller.
-  Avoid calling return() from inside a macro.
- For ease of maintenance, prefer pass all values a function/macro need as command arguments rather than relying on variable being set in the calling scope.
- use the keyword-based argument handling provided by cmake_parse_arguments(). 
- Beware of dropping empty arguments and list flattening when parsing or forwarding command arguments. 
- Prefer to use the PARSE_ARGV form of cmake_parse_arguments() inside functions.
- When forwarding arguments, use cmake_language(EVAL CODE) to quote each argument individually if preserving empty arguments and lists is required.

- For ease of maintenance, avoid cmake_language(DEFER).

- Don't distribute functions and macros throughout the source tree.
- nominate a particular directory (below the top level of the project) where various XXX.cmake files (defining the functions/macros) can be collected.
- Do not define or call a function or macro with a name that starts with a single underscore.
- prefer to use CMAKE_CURRENT_FUNCTION_LIST_DIR to refer to any file or directory expected to exist at a location relative to the file in which a function is defined.