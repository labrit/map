#ifndef MAP_H
#define MAP_H
typedef unsigned hashfunc(const char*, unsigned);

typedef struct mapelem_t {
    struct mapelem_t *next;
    char *key;
    char *value;
} mapelem_t;

typedef struct map_t {
    struct mapelem_t *htable;
    hashfunc *hash;
    unsigned keys;
} map_t;

int map_init(map_t *const, const unsigned);
int map_init2(map_t *const, hashfunc*, unsigned);
int map_insert(map_t *const, const char *const, const char *const);
int map_reassign(map_t *const, const char *const, const char *const);
int map_exists(const map_t *const, const char *const);
int map_lookup(const map_t *const, char *const, const char *const, const size_t);
void map_remove(map_t *const, const char *const);
void map_delete(map_t *const);
#endif
