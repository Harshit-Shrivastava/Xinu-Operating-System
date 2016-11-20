#include <prodcons.h>

uint future_cons(future *fut) {
  int i = &fut->value;
  int status;
  status = future_get(fut, &i);
  if (status < 1) {
   // printf("future_get failed\n");
    return -1;
  }
  printf("it consumed %d\n", i);
  return OK;
}
