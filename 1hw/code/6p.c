#include <stdio.h>

int AddVal(int x, int y){
  return x+y;
}

void AddRef(int *x, int *y){
  //changes x to be the sum of x and y
  *x += *y;
}

int main(){
  int a=1;
  int b=2;
printf("%d\n",AddVal(a, b));
AddRef(&a, &b);
printf("%d\n",a);

  return 0;
}
