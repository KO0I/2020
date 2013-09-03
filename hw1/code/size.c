#include <limits.h>
#include <stdio.h>

int main(){
//int max_int = INT_MAX;
int max_int;
max_int = sizeof(int);

printf("%d bytes\n",max_int);
return 0;
}
