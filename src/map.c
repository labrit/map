#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

/* Internal function prototypes */
static unsigned rshash(const char*, const unsigned);
static mapelem_t* map_get_elem(const map_t *const, const char *const);

static unsigned rshash(const char *str, const unsigned mod) {
  const unsigned b = 378551;
  unsigned a = 63689;
  unsigned hash = (!str[1]) ? (unsigned)*str++ : 0;

  while (*str) {
    hash = hash * a + (unsigned)(*str++);
    a *= b;
  }

  return hash & (mod-1);
}

static mapelem_t* map_get_elem(const map_t *const map, const char *const key) {
  static mapelem_t *last_mapelem_ptr;
  if (!map) return last_mapelem_ptr;

  mapelem_t *mapelem_ptr = &map->htable[map->hash(key, map->keys)];
  last_mapelem_ptr = mapelem_ptr;

  if (mapelem_ptr->key) {
    if (!mapelem_ptr->next) {
      if (strcmp(mapelem_ptr->key, key)) {
        return NULL;
      }
    } else {
      while (strcmp(mapelem_ptr->key, key)) {
        last_mapelem_ptr = mapelem_ptr;
        mapelem_ptr = mapelem_ptr->next;
        if (!mapelem_ptr) {
          return NULL;
        }
      }
    }
  } else {
    return NULL;
  }

  return mapelem_ptr;
}

int map_init(map_t *const map, const unsigned keys) {
  return map_init2(map, &rshash, keys);
}

int map_init2(map_t *const map, hashfunc *hash, unsigned keys) {
  map->htable = malloc(sizeof(mapelem_t) * keys);
  if (map->htable) {
    map->hash = hash;
    map->keys = keys;

    while (keys--) {
      map->htable[keys].next = NULL;
      map->htable[keys].key = NULL;
      map->htable[keys].value = NULL;
    }
    return 0;
  }
  return -1;
}

int map_insert(map_t *const map, const char *const key, const char *const value) {
  mapelem_t *last_mapelem_ptr = NULL;
  mapelem_t *mapelem_ptr = map_get_elem(map, key);

  if (!mapelem_ptr) {
    mapelem_ptr = map_get_elem(NULL, NULL);
    if (mapelem_ptr->key != NULL) {
      mapelem_ptr->next = malloc(sizeof(mapelem_t));
      if (mapelem_ptr->next) {
        last_mapelem_ptr = mapelem_ptr;
        mapelem_ptr = mapelem_ptr->next;
        mapelem_ptr->next = NULL;
      } else {
        return -1;
      }
    }

    mapelem_ptr->key = malloc(strlen(key)+1);
    mapelem_ptr->value = malloc(strlen(value)+1);

    if (mapelem_ptr->key && mapelem_ptr->value) {
      strcpy(mapelem_ptr->key, key);
      strcpy(mapelem_ptr->value, value);
      return 0;
    }

    if (mapelem_ptr->key) {
      free(mapelem_ptr->key);
      mapelem_ptr->key = NULL;
    }
    if (mapelem_ptr->value) free(mapelem_ptr->value);
    if (last_mapelem_ptr) {
      free(last_mapelem_ptr->next);
      last_mapelem_ptr->next = NULL;
    }
    return -1;
  }

  return 1;
}

int map_reassign(map_t *const map, const char *const key, const char *const value) {
  int ret = map_insert(map, key, value);

  if (ret != 1)
    return ret;

  char *tmp = malloc(strlen(value)+1);
  if (tmp) {
    mapelem_t *mapelem_ptr = map_get_elem(NULL, NULL);
    free(mapelem_ptr->value);
    mapelem_ptr->value = tmp;
    strcpy(mapelem_ptr->value, value);
    return 0;
  }

  return -1;
}

int map_exists(const map_t *const map, const char *const key) {
  return (!map_get_elem(map, key)) ? 1 : 0;
}

int map_lookup(const map_t *const map, char *const dest, const char *const key, const size_t size) {
  mapelem_t *mapelem_ptr = map_get_elem(map, key);

  if (mapelem_ptr) {
    dest[0] = '\0';
    strncat(dest, mapelem_ptr->value, size-1);
    return 0;
  }

  return 1;
}


void map_remove(map_t *const map, const char *const key) {
  mapelem_t *tmp, *mapelem_ptr = map_get_elem(map, key);

  if (mapelem_ptr) {
    free(mapelem_ptr->key);
    free(mapelem_ptr->value);
    mapelem_ptr->key = NULL;

    mapelem_ptr = map_get_elem(NULL, NULL);
    if (mapelem_ptr->next) {
      tmp = mapelem_ptr->next->next;
      free(mapelem_ptr->next);
      mapelem_ptr->next = tmp;
    }
  }
}

void map_delete(map_t *const map) {
  if (map->htable) {
    mapelem_t *mapelem_ptr;
    unsigned i = map->keys;

    while (i--) {
      if (map->htable[i].key) {
        while (map->htable[i].next) {
          mapelem_ptr = &map->htable[i];
          while (mapelem_ptr->next->next) {
            mapelem_ptr = mapelem_ptr->next;
          }
          free(mapelem_ptr->next->key);
          free(mapelem_ptr->next->value);
          free(mapelem_ptr->next);
          mapelem_ptr->next = NULL;
        }

        free(map->htable[i].key);
        free(map->htable[i].value);
      }
    }
    free(map->htable);
    map->htable = NULL;
  }
}
