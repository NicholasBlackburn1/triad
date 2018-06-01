/**
  * @file vector.c
  * Vector array helper functions.
  */

#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

/**
  * Create a new vector.
  */
void vector_init(vector *v)
{
  v->capacity = VECTOR_INIT_CAPACITY;
  v->total    = 0;
  v->items    = malloc(sizeof(void *) * v->capacity);
}

/**
  * Returns the vector's total item count.
  */
int vector_total(vector *v) {
  return v->total;
}

static void vector_resize(vector *v, int cap) {
  #if DEBUG
  printf("vector resize: %d to %d\n", v->capacity, cap);
  #endif

  void **items = realloc(v->items, sizeof(void *) * cap);
  if (items != NULL) {
    v->items    = items;
    v->capacity = cap;
  }
}

/**
  * Add an item to a vector.
  * This method automatically resizes a vector if
  * needed.
  */
void vector_add(vector *v, void *item) {
  if (v->capacity == v->total) {
    vector_resize(v, v->capacity * 2);
  }
  v->items[v->total++] = item;
}

void vector_set(vector *v, int index, void *data) {
  if (index >= 0 && index <= v->total) {

  }
}
