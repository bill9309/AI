#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char ** argv){
  int i;
  double sum_a,sum_b;
  char * buffer = malloc(sizeof(char) * 50);
  sum_a = 0;
  sum_b = 0;
  for(i = 0; i < 50; i++){
    buffer[i] = argv[1][i];
  }
  for(i = 0; i < 50; i++){
    if(buffer[i] == '0'){
      sum_a += sqrt(i + 1);
      printf("Group A: %d\n", i+1);
    }
    else{
      sum_b += sqrt(i + 1);
      printf("Group B: %d\n", i+1);
    }
  }
  printf("Difference: %f\n", fabs(sum_a - sum_b));
  return 0;
}
