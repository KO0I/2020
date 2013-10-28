#include <stdio.h>

int main(){
  int *ptr;
  int vals[10];
  ptr=vals;
  int i;
  for(i=10; i--; *ptr++=i+1){
  }
  for(i=0; i<=9; i++){
    printf("val at %d is %d\n",i+1,vals[i]);
  }

  return 0;
}
