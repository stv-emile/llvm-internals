
## 4.1 Variable basics
Syntax:
```
set(varName value... [PARENT_SCOPE])
```
- varName is case sensitive.
- a variable has a particular scope
- a variable cannot read/modified outside its own scope
- [define local scope & pass information](Section 5.4, “Scope Blocks”) 
- [use of local scope](Chapter 7, Using Subdirectories and Chapter 8, Functions And Macros)

All variable is string.
- Values must be quoted if they have spaces.
- Multiple values semicolon(;) separated make the variable a **list**.
- Not quoted, Values space separated are converted to semicolon separated.

```
  set(myVar a b c)    # myVar = "a;b;c"
  set(myVar a;b;c)    # myVar = "a;b;c"
  set(myVar "a b c")  # myVar = "a b c"
  set(myVar a b;c)    # myVar = "a;b;c"
  set(myVar a "b c")  # myVar = "a;b c"
```

Value of a variable is obtained with ${myVar}.
- It can be used anywhere a string or variable is expected.
- Use of an undefined variable simply results in an empty string being substituted.
- no warning is issued for use of an undefined variable
- `--warn-uninitialized` option to the cmake command to enable undefined variable warnings.
  (rarely used because undefined variables are very common)

Use of vars:
```
    set(foo ab)             # foo   =   "ab"
    set(bar ${foo}cd)       # bar   =   "abcd"
    set(baz ${foo} cd)      # baz   =   "ab;cd"
    set(myVar ba)           # myVar =   "ba"
    set(big ${${myVar}r}ef) # big   =   "${bar}ef"  =   "abcdef"
    set(${foo} xyz)         # ab    =   "xyz"
    set(bar ${notSetVar})   # bar   =   ""   
```

- String can contain multiple lines 
- String can contain quotes (\")
```
    set(multiLine "First line
      Second line with a \"quoted\" word")
```

- Alternative to quotes is to use the lua-inspired bracket syntax (>CMake 3.0):
  start: [=[ and end : ]=]
-  bracket syntax is particularly well suited to defining content like Unix shell scripts.
-  [situations where bracket syntax are better](Chapter 20, Working With Files)

```
  # Simple multi-line content with bracket syntax,
  # no = needed between the square bracket markers
  set(multiLine [[
  First line
  Second line
  ]])


  # Bracket syntax prevents unwanted substitution
  set(shellScript [=[
  #!/bin/bash

  [[ -n "${USER}" ]] && echo "Have USER"
  ]=])

  # Equivalent code without bracket syntax
  set(shellScript
  "#!/bin/bash

  [[ -n \"\${USER}\" ]] && echo \"Have USER\"
  ")

```

A variable can be unset by:
```
# set() with no value
set(myVar)

# alling unset() 
unset(myVar)
```

There are two sets of variables:
- variables defined by the project for its own use
- variable set to to influence behavior of CMake’s commands

## 4.2 Environnment variable

Retrieve and set environment variables values.
- Obtained: `$ENV{varName}`
- This can be used anywhere a regular ${varName} form can be used.
setting env variable.
```
  set(ENV{PATH} "$ENV{PATH}:/opt/myDir")
```

>[!Warning]
> Setting environment variables within the CMakeLists.txt file like this is rarely useful.
> As soon as the CMake run is finished, the change to the environment variable is lost.


## 5.3. Cache Variables
- Normal variables which have a lifetime limited to the processing of the CMakeLists.txt file.
- Cache variables are stored in the special file called CMakeCache.txt in the build directory and they persist between CMake runs. 

```sh
set(varName value... CACHE type "docstring" [FORCE])
```
- the docstring can be empty -  its a documentation string used only by GUI.
- The type is used mostly to improve the user experience in GUI tools.

The type must be :
  - BOOL: a boolean on/off value == checkbox in GUI.
          [ON/OFF, TRUE/FALSE, 1/0 etc.](Section 6.1.1, “Basic Expressions”)
    
  - FILEPATH: a path to a file on disk ==  a file dialog to the user in GUI.
    
  - PATH:  a path to a file on disk == dialog that selects a directory in GUI.
  
  - STRING: an arbitrary string == a single-line text edit widget in GUI.
  
  - INTERNAL: not intended to be made available to the user - Not shown by GUI.

>[!Important]
> Setting a boolean cache variable is a common need.
> CMake provides a separate command for it. equivalent commands
> `option(optVar helpString [initialValue])` by default initialValue is OFF
> `set(optVar initialValue CACHE BOOL helpString)`
> [Little difference](Section 5.5, “Potentially Surprising Behavior Of Variables”)

Difference between normal and cache variable.
- cahe: set() command will only overwrite a cache variable if the FORCE keyword is present.
- normal: set() command will always overwrite a pre-existing value.

## 5.4. Scope Blocks
- Cache variables have global scope, so they are always accessible.
- non-cache variable’s scope is the CMakeLists.txt file in which the variable is defined. often called the directory scope.
- Subdirectories and functions inherit variables from their parent scope.

(>CMake 3.25)
`block()` and `endblock()`   commands can be used to define a local variable scope. 
- Upon entering the block, all the variables defined in the surrounding scope are copied.
- The surrounding scope’s variables unchanged. 
- Upon leaving the block, all variables that were created in the block are discarded. 

```
set(x 1)
  block()
      set(x 2)   # Shadows outer "x"
      set(y 3)   # Local, not visible outside the block
  endblock()
  # Here, x still equals 1, y is not defined
```

Selectively modify some variables in the surrounding scope.
PARENT_SCOPE of the set() and unset() commands can be used.
```
  set(x 1)
  set(y 3)
  block()
      set(x 2 PARENT_SCOPE)
      unset(y PARENT_SCOPE)
      # x still has the value 1 here
      # y still exists and has the value 3
  endblock()
  # x has the value 2 here and y is no longer defined
```

- When PARENT_SCOPE is used, the variable being set or unset is the one in the parent scope, not the one in the current scope.

To set or unset the variable in both the parent and the current scope use `PROPAGATE`.
```
  set(x 1)
  set(z 5)
  block(PROPAGATE x z)
    set(x 2)            # Gets propagated back out to the outer "x"
    set(y 3)            # Local, not visible outside the block
    unset(z)            # Unsets the outer "z" too
  endblock()
  # Here, x equals 2, y and z are undefined
```

full signature of block() command:
```
  block([SCOPE_FOR [VARIABLES] [POLICIES]] [PROPAGATE var...])
```
- SCOPE_FOR ommitted: creates a new local scope for both variables and policies

```
  set(x 1)
  set(z 5)
  block(SCOPE_FOR VARIABLES PROPAGATE x z)
    set(x 2)        # Gets propagated back out to the outer "x"
    set(y 3)        # Local variable, not visible outside the block
    unset(z)        # Unsets the outer "z" too
  endblock()
  # Here, x equals 2, y and z are undefined
```

## 5.5. Potentially Surprising Behavior Of Variables

VERY IMPORTANT
- Normal and cache variables are two separate things.
- It is possible to have a normal variable and a cache variable with the same name but holding different values.
- Behaviour: CMake will retrieve the normal variable’s value rather than the cache variable when using ${myVar}.

when setting a cache variable’s value,
any normal variable of the same name is removed from the current scope 
in the following situations:
- The cache variable did not exist before the call to set() or option().
- opposie of 1 but it did not have a defined type.
- The FORCE or INTERNAL option was used in the call to set().

TAKE YOUR TIME...

## 5.6. Manipulating Cache Variables

Ways to manipulate variable of compler/linker flags, external packages paths, build parts:

### 5.6.1. Setting Cache Values On The Command Line
via command line options passed to cmake.
```
cmake -D myVar:type=someValue ...
```
- someValue will replace any previous value of the myVar cache variable.
- same behaviour as the set() command with the CACHE and FORCE options.
- no corresponding set()/option() command is required in CMakeLists.txt
- docstring is empty.
- the type can be omitted = variable will have an undefined type = INTERNAL

```
cmake -D foo:BOOL=ON ...
cmake -D "bar:STRING=this contains space" ...  <<< Important to note quote>>>
cmake -D hideme=misteryValue ...
cmake -D helpers:FILEPATH=subdir/helpers.txt ...
cmake -D helpdir:PATH=/opt/helpthings ...
```
- It is a good habit to always specify the type of the variable on the command line.

It is also possible to remove variables from the cache with the -U option:
Avoid using wildcard (* and ?)
```
cmake -U 'help*' -U foo ...
```

### 5.6.2. CMake GUI Tools
cmake-gui and ccmake

## 5.7. Printing Variable Values

It can be useful to print out diagnostic messages and variable values during a CMake run.
[More details](Chapter 13, Debugging And Diagnostics.)
```
set(myVar HiThere)
message("The value of myVar = ${myVar}\nAnd this "
    "appears on the next line")
```

## 5.8. String Handling
project complexity grows.
string handling functionality enables projects to perform :
- find and replace operations, 
- regular expression matching, 
- upper/lower case transformations, 
- strip whitespace and other common tasks.
Refere to CMake reference documentation to use them.
Return values are stored in variables.

Most common:
- FIND searches for subString in inputString and stores the index of the found subString in outVar. REVERSE to take last occurrence.
```
  string(FIND inputString subString outVar [REVERSE])
```

- The REPLACE operation will replace every occurrence of matchString in the input strings with replaceWith and store the result in outVar.
```
  string(REPLACE matchString replaceWith outVar input...)
```

- Regular expression with variant
```

  string(REGEX MATCH    regex outVar input...)
  string(REGEX MATCHALL regex outVar input...)
  string(REGEX REPLACE  regex replaceWith outVar input...)
```
MATCH operation finds just the first match and stores it in outVar.
MATCHALL finds all matches and stores them in outVar as a list.
REPLACE will return the entire input string with each match replaced by replaceWith.
    Matches can be referred to in replaceWith using the usual notation \\1, \\2, etc

```
  string(REGEX MATCH    "[ace]"           matchOne abcdefabcdef)
  string(REGEX MATCHALL "[ace]"           matchAll abcdefabcdef)
  string(REGEX REPLACE  "([de])" "X\\1Y"  replVar1 abc def abcdef)
  string(REGEX REPLACE  "([de])" [[X\1Y]] replVar2 abcdefabcdef)
```
output:

```
  matchOne = a
  matchAll = a;c;e;a;c;e
  replVar1 = abcXdYXeYfabcXdYXeYf
  replVar2 = abcXdYXeYfabcXdYXeYf
```

- Extracting a substring is also possible:
  ```
    string(SUBSTRING input index length outVar)
  ```
  The index is an integer defining the start of the substring to extract from input. 

- String length can be trivially obtained.
  it counts bytes rather than characters
  ```
    string(LENGTH  input outVar)
  ```

- Strings can easily be converted to upper or lower case:
  ```
  string(TOLOWER input outVar)
  string(TOUPPER input outVar)
  ```

- strip whitespace from the start and end of a string
  ```
    string(STRIP   input outVar)
  ```

## 5.9 Lists
Lists are used heavily in CMake.
Lists are just a single string with list items separated by semicolons.
CMake provides the list() command to facilitate tasks on list.

- The basic list operations:
```
  list(LENGTH listVar outVar)
  list(GET    listVar index [index...] outVar)
```
eg.
```
set(myList a b c)       # create the list "a;b;c"

# count the length of myList and store on len
list(LENGTH myList len)

# get the elements of indexes 1 and 3 and store them in ltters
list(GET mylist 1 2 letters)
```

- Inserting, appending and prepending - modify the list in-place:
```
  list(INSERT  listVar index item [item...])
  list(APPEND  listVar item [item...])
  list(PREPEND listVar item [item...])  # Requires CMake 3.15 or later
```

- Finding a particular item in the list follows the expected pattern:
  ```
  list(FIND myList value outVar)
  ```

- Removing items
  ```
  list(REMOVE_ITEM       myList value [value...])
  list(REMOVE_AT         myList index [index...])
  list(REMOVE_DUPLICATES myList)
  ```
  - REMOVE_ITEM remove all instances of one or more items from a list
  - REMOVE_AT specifies one or more indices to remove
  - REMOVE_DUPLICATES ensure the list contains only unique items.

- popping items from the front or back of a list:
```
  list(POP_FRONT myList [outVar1 [outVar2...]])
  list(POP_BACK myList [outVar1 [outVar2...]])
```

-  reorder items with REVERSE or SORT operations:
```
list(REVERSE myList)
list(SORT myList [COMPARE method] [CASE case] [ORDER order])
```
method:
   - STRING: Sort alphabetically (default when COMPARE not specified)
   - FILE_BASENAME: ordered according to the basename part of the path only.
   - NATURAL: Similar to STRING, except contiguous digits within an item are sorted numerically.

case: 
   - SENSITIVE
   - INSENSITIVE 
  
ordrer:
   - ASCENDING
   - DESCENDING

## 5.9.1. Problems With Unbalanced Square Brackets

- For historical reasons, if a list item contains an opening square bracket [, it must also have a matching closing square bracket ]. 
-  CMake will consider any semicolon between these square brackets to be part of the list item instead of as a list separator. 
```
    set(withBrackets "a[a" "b]b")
    list(LENGTH withBrackets lenWith)
    message("With brackets: Length=${lenWith} --> First_element=${firstWith}")

    # output
    With brackets: Length=1 --> First_element=a[a;b]b
```

## 5.10. Math

common form of variable manipulation is math computation. 
```
  math(EXPR outVar mathExpr [OUTPUT_FORMAT format])
```
-  first argument must be the keyword EXPR
-  mathExpr defines the expression to be evaluated
-  the result will be stored in outVar
-  format is either DECIMAL (by default) or HEXADECIMAL.

```
  set(x 3)
  set(y 7)
  math(EXPR zDec "(${x}+${y}) * 2")
  message("decimal = ${zDec}")
```

## 5.11. Recommended Practices

- The CMake GUI tool is a useful way to quickly and easily understand the build options for a project.
Don't underestimate it. use it.

- Prefer to provide cache variables for controlling whether to enable optional parts of the build instead of encoding the logic in build scripts outside of CMake.

- Try to avoid relying on environment variables being defined to make build predictable, reliable and easy to set up.

- prefer to pass information directly to CMake through cache variables instead wherever possible.

- Try to establish a variable naming convention early. 

- For cache variables, consider grouping related variables under a common prefix followed by an underscore  to take advantage of how CMake GUI groups variables.

- A name beginning with the project name or something closely associated with the project may be desirable for scalability.

- Avoid defining non-cache variables in the project which have the same name as cache variables.

- make a quick scan through the CMake documentation page listing the pre-defined variable  to become familiar with already existing variables wich influence CMake’s behavior.