#include "proto.h"

void * create_memory_region (void) {
  struct memory_region ** region = malloc(sizeof(struct memory_region *));
  if (region) *region = NULL;
  return region;
}

void destroy_memory_region (void * region) {
  if (!region) return;
  struct memory_region * current = *((struct memory_region **) region);
  struct memory_region * next;
  while (current) {
    next = current -> next;
    free(current);
    current = next;
  }
  free(region);
}

void * mr_malloc (void * region, unsigned size) {
  return mr_create_node(region, malloc(size + sizeof(struct memory_region)));
}

void * mr_calloc (void * region, unsigned size) {
  return mr_create_node(region, calloc(size ? 2 + (size - 1) / sizeof(struct memory_region) : 1, sizeof(struct memory_region)));
}

void * mr_create_node (struct memory_region ** region, struct memory_region * node) {
  if (!node) return NULL;
  node -> prev = NULL;
  node -> next = *region;
  if (*region) (**region).prev = node;
  *region = node;
  return node + 1;
}

void * mr_realloc (void * region, void * buffer, unsigned new_size) {
  if (!new_size) {
    mr_free(region, buffer);
    return NULL;
  }
  if (!buffer) return mr_malloc(region, new_size);
  struct memory_region * old = ((struct memory_region *) buffer) - 1;
  struct memory_region * new = realloc(old, sizeof(struct memory_region) + new_size);
  if (old == new) return buffer;
  if (old -> next) old -> next -> prev = new;
  if (old -> prev)
    old -> prev -> next = new;
  else
    *((struct memory_region **) region) = new;
  return new + 1;
}

void mr_free (void * region, void * buffer) {
  if (!buffer) return;
  struct memory_region * node = ((struct memory_region *) buffer) - 1;
  if (node -> next) node -> next -> prev = node -> prev;
  if (node -> prev)
    node -> prev -> next = node -> next;
  else
    *((struct memory_region **) region) = node -> next;
  free(node);
}
