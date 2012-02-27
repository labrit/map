#include <stdio.h>
#include <stdlib.h>

#include "map.h"

int main(void) {
  map_t map;
  char buffer[32];
  if (map_init(&map, 0x10000)) /* 0x10000 = 2^16 */
    return EXIT_FAILURE;

  map_insert(&map, "KEY", "VALUE");
  map_insert(&map, "big", "small");
  map_insert(&map, "male", "female");
  map_insert(&map, "works", "no");

  map_reassign(&map, "works", "yes");

  if (!map_exists(&map, "big"))
    printf("'big' exists!\n");

  map_lookup(&map, buffer, "works", sizeof buffer);
  printf("k: '%s' v: '%s'\n", "works", buffer);

  map_remove(&map, "male");

  if (map_exists(&map, "male"))
    printf("'male' does not exist!\n");

  map_delete(&map);

  return EXIT_SUCCESS;
}
