/**
 * vector.c
 * @file Dynamic vector arrays.
 *
 * Copyright © 2018, Aaron Sutton <aaronjsutton@icloud.com>
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
  * @param[in,out] v      The vector to modify.
  * @param[in]     item   The item to add into the vector.
  */
void vector_add(vector *v, void *item) {
  if (v->capacity == v->total) {
    vector_resize(v, v->capacity * 2);
  }
  v->items[v->total++] = item;
}

/**
  * @brief Set a value in the vector at a specified index.
 */
void vector_set(vector *v, int index, void *data) {
  if (index >= 0 && index < v->total) {
    v->items[index] = data;
  }
}

void *vector_get(vector *v, int index) {
  if (index >= 0 && index < v->total) {
    return v->items[index];
  }
  return NULL;
}

void vector_delete(vector *v, int index) {
  int i;
  if (index < 0 || index >= v->total)
    return;

  v->items[index] = NULL;

  for (i = index; i < v->total - 1; i++) {
    v->items[i] = v->items[i + 1];
    v->items[i + 1] = NULL;
  }

  v->total--;

  if (v->total > 0 && v->total == v->capacity / 4) {
    vector_resize(v, v->capacity / 2);
  }
}

void vector_free(vector *v) {
  free(v->items);
}
