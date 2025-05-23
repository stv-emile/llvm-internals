
To apply some steps only in certain circumstances.

## 6.1. The if() Command
```sh
if(expression1)
    # commands...
elseif(expression2)
    # commands...
else()
    # commands...
endif()
```

various other conditions :
- file system tests
- version comparison
-  testing for the existence of things.

What it considers true and false:
- quoted or not: ON, YES, TRUE, Y ==> True (case-insensitive)
- quoted or not: OFF, NO, FALSE, N, IGNORE, NOTFOUND, "", *-NOTFOUND ==> False (case-insensitive)
- 2 ==> converted to bool (C rules - 0/1) 
- someletters ==> is variable name ? not set ==> false.
- "someletters" ==> false.

```
  # Does not match any of the true or false constants, so proceed
  # to testing as a variable name in the fall-through case below
  if(someLetters)

  # Quoted value that doesn't match any of the true or false constants,
  # so again fall through to testing as a variable name or string
  if("someLetters")
```

Fall through case in cmake:
```
if(<variable|string>)
```
in practice the if-expression is either:
- An unquoted name of a (possibly undefined) variable.
- A quoted string.

eg.
- if(someletter) <==> if(${someletter})
someletter not defined => ${someletter} empty string => false.
someletter defined => replace by it value (is False/True)
- if("someletter") <==> if(False)

Environment variable are different.
- if(ENV{some_var}) will always evaluate to false.

Best practice
There is a potential confusion around quoted values.
It is generally advisable to avoid using quoted arguments with the if(something) form. better comparison expressions that handle strings more robustly [](Section 6.1.3, “Comparison Tests”)

### 6.1.2. Logic Operators
AND, OR and NOT logical operators, and parentheses.
```
# Logical operators
if(NOT expression)
if(expression1 AND expression2)
if(expression1 OR expression2)

# Example with parentheses
if(NOT (expression1 AND (expression2 OR expression3)))
```

### 6.1.3. Comparison Tests

categories of comparison test:
numeric, string, version numbers and path.
`if(value1 OPERATOR value2)`
- value1 and value2, can be either variable names or (possibly quoted) values.
- value same as the name of a defined variable, it will be treated as a variable.

Numeric:
    - LESS
    - GREATER
    - EQUAL
    - LESS_EQUAL
    - GREATER_EQUAL

Strings:
    - STRLESS
    - STRGREATER
    - STREQUAL
    - STRLESS_EQUAL
    - STRGREATER_EQUAL

Version numbers:
    - VERSION_LESS
    - VERSION_GREATER
    - VERSION_EQUAL
    - VERSION_LESS_EQUAL
    - VERSION_GREATER_EQUAL

Path:
    - PATH_EQUAL

Version number comparison:
```
    if(1.2 VERSION_EQUAL 1.2.0) 
    if(1.2 VERSION_LESS 1.2.3) 
    if(1.2.3 VERSION_GREATER 1.2 ) 
    if(2.0.1 VERSION_GREATER 1.9.7) 
    if(1.8.2 VERSION_LESS 2)
```

String vs Path
```
  # comparison is TRUE
  if ("/a//b/c" PATH_EQUAL "/a/b/c")
     ...
  endif()

  # comparison is FALSE
  if ("/a//b/c" STREQUAL "/a/b/c")
     ...
  endif()
```

Using Regex for condition
```
  if("Hi from ${who}" MATCHES "Hi from (Fred|Barney).*")
      message("${CMAKE_MATCH_1} says hello")
  endif()
```

### 6.1.4. File System Tests

set of tests to query the file system:
```sh
  if(EXISTS pathToFileOrDir)
  if(IS_DIRECTORY pathToDir)
  if(IS_SYMLINK fileName)
  if(IS_ABSOLUTE path)
  if(file1 IS_NEWER_THAN file2)
```
these operators don't perform any variable/string substitution without ${}.
IS_NEWER_THAN also returns true if both files have the same timestamp.

To achiave this naive one:
```sh
  # WARNING: Very likely to be wrong
  if(${firstFile} IS_NEWER_THAN ${secondFile})
      # ... commands to recreate secondFile
endif()
```
Do this:
```sh
  set(firstFile  "/full/path/to/somewhere")
  set(secondFile "/full/path/to/another/file")
  if(NOT EXISTS ${firstFile})
      message(FATAL_ERROR "${firstFile} is missing")
  elseif(NOT EXISTS ${secondFile} OR
         NOT ${secondFile} IS_NEWER_THAN ${firstFile})
      # ... commands to recreate secondFile
   endif()
```

### 6.1.5. Existence Tests
testing whether various CMake entities exist. 
useful in larger, more complex projects
some parts might or might not be present or be enabled.
```sh
if(DEFINED name)
if(COMMAND name)
if(POLICY name)
if(TARGET name)
if(TEST name)                   # CMake 3.4
if(value IN_LIST listvar)       # CMake 3.3
```
- `DEFINED`: Returns true if any variable of the specified name exists.
  for specific type of variable:
    ```sh
    if(DEFINED somevar)
    if(DEFINED CACHE{somevar})
    if(DEFINED ENV{somevar})
    ```
- `COMMAND`: Tests whether a CMake command, function or macro 
  with the specified name exists.
  Useful for checking whether something is defined before trying to use it.

- `POLICY`: Tests whether a particular policy is known to CMake
  Policy names: CMPxxxx, (xxxx is a four-digit number)

- `TARGET`: Returns true if a CMake target of the specified name has been defined by one of the commands : add_executable(), add_library() or add_custom_target(). 

- `TEST`: Returns true if a CMake test with the specified name has been previously defined by the add_test() command.

- `IN_LIST`: returns true if listVar contains the specified value.
```sh
# Correct
  set(things A B C)
  if("B" IN_LIST things)
... endif()
  # WRONG: Right hand side must be the name of a variable
  if("B" IN_LIST "A;B;C")
      ...
endif()
```

## 6.1.6. Common Examples

When building with different compilers, eg. Visual Studio and other.
Express the right constraint.
```sh
if(WIN32)  # non
if(MSVC) # oui

if(APPLE) # non
if(CMAKE_GENERATOR STREQUAL "Xcode")) # oui
```

Common practice - configure variable on call:
```sh
  option(BUILD_MYLIB "Enable building the MyLib target")
  if(BUILD_MYLIB)
      add_library(MyLib src1.cpp src2.cpp)
  endif()
```

## 6.2 Looping
How to perform some action on a list of items or for a range of values. 

### 6.2.1. foreach()

To iterate over a set of items or values.

- The most basic form
  ```js
  foreach(loopVar arg1 arg2 ...)
      # ...
  endforeach()
  ```
  for each argN value, loopVar is set to that argument and the loop body is executed.
  No variable/string test is performed, the arguments are used exactly as the values are specified

- to iterate over a set of items or lists variable, More general form
  ```js
  foreach(loopVar IN [LIST list1 list2 ... ] IN [ITEMS item1 itme2 ...])
    # ...
  endforeach()
  ```

- a more specialized form for looping over multiple lists at once
  like the zip in python
  ```js
  foreach(loopVar... IN ZIP_LISTS listVar...)
    #...
  endforeach()
  ```
  give only loopVar -> read with ${loopVar_N}
  or put as much as loopVar as List in fist argument.
  eg. `foreach(var1 var2 IN ZIP_LISTS list1 list2)`
  if number of arg not equal => UB

- C-like iteration over a range of numerical values - stop (inclusive)
  ```js
  foreach(loopVar RANGE start stop [step])
  ```
  shortcut
  ```js
  # start from zero.
  foreach(loopVar RANGE maxvalue)
  ```

### 6.2.2. while()

```js
while(condition)
      # ...
endwhile()
```

### 6.2.3. Interrupting Loops
break() and continue() like in C.


### 6.3. Recommended Practices

- Minimize opportunities for strings to be unintentionally interpreted as variables in if(), foreach() and while() commands.
  
- Avoid unary expressions with quotes, prefer to use a string comparison operation instead. 
  `if("${MY_VAR}")` ==> `if(DEFINED MY_VAR)` or `if(MY_VAR STREQUAL "")`
  
- Strongly prefer to set a minimum CMake version of at least 3.1 

- store the CMAKE_MATCH_<n> match results in ordinary variables as soon as possible, When regular expression matching in if(xxx MATCHES regex) commands

- Prefer to use looping commands which avoid ambiguous or misleading code. 

- If using the RANGE form of foreach(), always specify both the start and end values.

- prefer IN LISTS or IN ITEMS forms  over  foreach(loopVar item1 item2 ...) form.