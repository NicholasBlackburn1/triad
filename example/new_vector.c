#include <stdio.h>
#include "triad.h"

int main() {
  VECTOR_INIT(v);
  vector_add(&v, "bonjour");
  vector_add(&v, "hello");
  vector_add(&v, "hola");
  vector_add(&v, "ciao");
  printf("vector has %i item(s)\n", VECTOR_TOTAL(v));
  printf("french: %s\n", (char*)vector_get(&v, 0));
  printf("spanish: %s\n", (char*)vector_get(&v, 2));
  vector_delete(&v, 2);
  printf("italian: %s\n", (char*)vector_get(&v, 2));
  vector_add(&v, "hallo");
  vector_add(&v, "Здравствуйте");
  printf("russian: %s\n", (char*)vector_get(&v, v.total - 1));
  vector_delete(&v, 0);
  vector_delete(&v, 1);
  vector_delete(&v, 2);
}
