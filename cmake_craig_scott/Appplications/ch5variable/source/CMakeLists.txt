cmake_minimum_required(VERSION 3.24)
project(variable VERSION 1.0.0 LANGUAGES CXX)
add_executable(vars main.cxx)



message("Hello, i build nothing. Just to play with variables.")

# SIMPLE VARIABLE
set(author Steevenson)
set(fullname "Steevenson EMILE")
message("The author is ${author}")
message(WARNING "The full Name with space and quoted is ${fullname}" )

# Effecetive use of variable
set(prefix SYCL)
set(${prefix}_VERSION 1.4)
message("The version is ${${prefix}_VERSION}")

# LIST VARIABLE
set(authors Steevenson "Jean Ronel" Richardeau)
set(books "Guide d'amour" "SMS d'amour" "books")
set(metier Medecin;Ingenieur;Ingenieur)
message("The authors are ${authors}")
message("The books are ${books}")
message("Their metier are ${metier}")

# MULTILINE VARIABLE
set(shellscript [=[
#!/bin/bash
echo "You can even use expresions with brackets here"
]=])

# ENVIRONMENT VARIABLE
set(ENV{WORK_HOME} "/mnt/c/Personnel/git/llvm-internals")
message("The working home is $ENV{WORK_HOME}")

# CACHE VARIABLE
set(VARCACHE "VAL-CACHE1020" CACHE STRING "Description of cache")
message("cache value is ${VARCACHE}")
# boolean cache
option(BOOLCACHE1 "This a boolean cache with OFF as not set")
option(BOOLCACHE2 "This a boolean cache with ON explicitly set" ON)

message("set to ON; BOOLCACHE1 = ${BOOLCACHE1}")
message("set to OFF by default; BOOLCACHE2 = ${BOOLCACHE2}")

# CACHE VARIABLE - overwrite existing value between cmake run
set(VARCACHE "VAL-CACHE-NEW-0" CACHE STRING "Description of new cache" FORCE)
message("New cache value is ${VARCACHE}")

# SCOPE of variables

set(parent_var1 "parentValue1")
set(parent_var2 "parentValue2")
set(parent_var3 "parentValue3")
message("parent_var1 in parent before bock is ${parent_var1}")
message("parent_var2 in parent before bock is ${parent_var2}")
message("parent_var3 in parent before bock is ${parent_var3}")
block(PROPAGATE parent_var3)
    # modify in the child scope
    set(parent_var1 "childValue1")
    message("parent_var1 in child is: ${parent_var1}")

    # modfify in child scope - with parent scope
    # Only the parent.parent_var2 is changed. child.parent_var2 keep its original value.
    set(parent_var2 "childValue2" PARENT_SCOPE)
    message("parent_var2 in child is ${parent_var2}")

    # modify in child scope - but have been propagated. child.parent_var3 modified => parent.parent_var3 modified.
    set(parent_var3 "childValue3")
    message("parent_var3 in child is ${parent_var3}")
endblock()
message("parent_var1 in parent after bock is ${parent_var1}")
message("parent_var2 in parent after bock is ${parent_var2}")
message("parent_var3 in parent after bock is ${parent_var3}")

# CACHE VALUES ON THE COMMAND LINE

# -D varname:type=value ==== set(varname value CACHE type "" FORCE)

# String operation on variable
string(LENGTH ${parent_var1} len_parent_var1)
message(WARNING ${len_parent_var1})

# List operation on list variable
set(mylist moi toi lui)
message(NOTICE "mylist inital is : ${mylist}")
list(APPEND mylist "nous")
message(NOTICE "myNext list is : ${mylist}")

# Math operation on variables
list(LENGTH mylist lenlist)
math(EXPR lenlist "${lenlist} + 1" OUTPUT_FORMAT DECIMAL )
message("size of mylist + 1 is : ${lenlist}")