#include <stdio.h>
#include <stdlib.h>
int main(){
  int* x;
  int* y;
  x = malloc(sizeof(int));
  y = malloc(sizeof(int));
  *x = 1;
  *y = 2;
  printf("%d, %d\n", x, y);
  x=y;
  printf("%d, %d\n", x, y);
  return 0;
  free(x);
  free(y);
}
