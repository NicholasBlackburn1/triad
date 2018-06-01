#include <stdio.h>
#include "triad.h"

int main() {
  VECTOR_INIT(v);
  vector_add(&v, "bonjour");
  printf("vector has %i item(s)\n", VECTOR_TOTAL(v));

}
