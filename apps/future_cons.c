#include <prodcons.h>

uint future_cons(future *fut) {
//  printf("inside coons");
  int i = &fut->value;
  int status;
  status = future_get(fut, &i);
//  printf("Status %d", status);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("it produced %d\n", i);
//  future_free(fut);
  return OK;
}
