#Description
This is a stand-alone map/dictionary implementation I've made for the heck of 
it. All the code is licensed under the public domain and you can use it however 
you wish. It seems fairly fast, but I havn't ran any comprehensive benchmarks 
so I couldn't say for certain. Since the code really doesn't have any useful 
comments, I've tried to make it pretty straight-forward and I've included some
documentation below.

###MISC. IMPLEMENTATION INFO
NONE of the functions should be assumed to be re-entrant OR thread-safe,
because they are not. The functions have collision detection and handle such
cases accordingly.

---

* ___NOTE__: I've discarded the const qualifiers for readibility_
* ___NOTE__: All return values are at the end of the README_

---

#Types
* `hashfunc`
 * DECLARATION: `typedef unsigned hashfunc(const char*, unsigned);`

* `map_t`
 * The map object.

* `mapelem_t`
 * The map element object used by internal functions, it should not be accessed
from the outside.

---

#Functions
* `int map_init(map_t *map, unsigned keys)`
 * Calls `map_init2()` with the built in hash function. The built-in hash function is fairly fast, but 
requires that the amount of keys be in the form 2^n.

* `int map_init2(map_t *map, hashfunc *hash, unsigned keys)`
 * Initializes map with the specified hash function and keys.

* `int map_insert(map_t *map, char *key, char *value)`
 * Inserts the key/value pair into the specified map. This function is
non-destructive. If it encounters that the same key already exists it returns
without doing anything.

* `int map_reassign(map_t *map, char *key, char *value)`
 * Like map_insert, it will insert the key/value pair into the map if it doesn't
already exist. If it encounters that a key already exists, the current
value will be replaced with the value passed to the function.

* `int map_exists(map_t *map, char *key)`
 * Simply returns whether or not the key exists.

* `int map_lookup(map_t *map, char *dest, char *key, size_t size)`
 * If the key exists the value will be written into the pointer specified by
dest up to 'size' bytes.

* `void map_remove(map_t *map, char *key)`
 * Removes the specified key from the map if it exists.

* `void map_delete(map_t *map)`
 * Completely deinitializes the map object.

---

#Function Return Values
* __-1__ :: There was a memory error
 * `map_init`, `map_init2`, `map_insert`, `map_reassign`

*  __0__ :: The operation was successful
 * all functions will return this value on success

* __1__ :: Differs per function that uses this return value
 * `map_insert`: The key already exists
 * `map_exists`, `map_lookup`: The key doesn't exist

___NOTE__: `map_remove` and `map_delete` will not return a value, this is because the only reason
for them to 'fail' is if there is nothing for them todo(i.e. the element has either already been
removedor the map already been deleted)._