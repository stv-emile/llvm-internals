# Properties


Why use properties while we have variables ?
What are properties ?

Properties:
- Properties are always attached to a specific entity and provide information specific to it.
- Entity can be a directory, a target, a source file, a test case, a cache variable or even the overall build process itself.

Variables:
- Variable store standalone values.
- A property default value is wometimes provided by a variable.

## General property commands

Most generic command to manipulate properties:

**set_property() command.**
```sh
set_property(entitySpecific
    [APPEND | APPEND_STRING]
    PROPERTY propertyName values...
)
```

The `entitySpecific` defines the entity whose property is being set.
Possible values : 
```
GLOBAL              # The built itself
DIRECTORY [dir]     # dir: The directory. the current directory if not set.
TARGET target
SOURCE source 
INSTALL file
TEST test
CACHE var
VARIABLE
```
The `PROPERTY` keywords marks all remaining arguments as defining the `property name` and its `value(s)`

- `propertyName` would normally match one of the properties defined in the CMake documentation.
- `value` is property specific.

A project may also set its own custom properties.
```sh
set_property(TARGET MyApp2 MyApp
    PROPERTY MYPROJ_CUSTOM_PROP val1 val2 val3)
```

The `APPEND` and `APPEND_STRING` keywords can be used to control how the named property is updated if it already has a value.
Assume:
- Previous Value: foo
- New Value: bar
Result values will be:
- No Keyword : bar
- APPEND foo;bar
- APPEND_STRING foobar

**get_property_command()**
```sh
  get_property(resultVar entitySpecific
      PROPERTY propertyName
      [DEFINED | SET | BRIEF_DOCS | FULL_DOCS]
)
```


## Global properties

## Directory properties

## Target properties

## SOurce properties 

## Cache variable properties

## Other property types

## Recommended practices

