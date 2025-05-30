# Properties

- Properties are sometimes confused with variables

The difference:
- A variable:
    - is not attached to any particular entity
    - holds a standalone value
- Properties:
    - are typically well defined and documented by CMake
    - are always apply to a specific entity
    - provides information specific to the entity it is attached to

    
Example of entities: directory, target, source file, test case, cache variable, overall build.

## 9.1. General Property Commands

The most generic commands for manipulating properties.   
`set_property()` and `get_property()`, allow setting and getting any property on any type of entity.

```sh
set_property(entitySpecific
    [APPEND | APPEND_STRING]
    PROPERTY propertyName values...
)
```

`entitySpecific` defines the entity whose property is being set.   
The possible values:
```sh
GLOBAL
DIRECTORY   [dir]
TARGET      targets...
SOURCE      sources... # Additional options with CMake 3.18
INSTALL     files...
TEST        tests
CACHE       vars
```
The first word defines the type of entity:
- GLOBAL: means the build itself, so there is no specific entity name required.
- DIRECTORY, if no dir is named, the current source directory is used.


**The PROPERTY keyword marks all remaining arguments as defining the property name and its value(s).**

The `propertyName` would normally match one of the properties defined in the CMake documentation.   
But a project may also set its own custom properties.

```sh
set_property(TARGET MyApp1 MyApp2
    PROPERTY MYPROJ_CUSTOM_PROP val1 val2 val3
)
```

The APPEND and APPEND_STRING keywords can be used to control how the named property is updated if
 it already has a value.