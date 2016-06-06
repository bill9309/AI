#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct result_list{
 int number;
 struct result_list * next;
}result_list;
int main(){
  int i,j;
  double sum_a,sum_b;
  result_list * a_first = malloc(sizeof(result_list));
  result_list * b_first = malloc(sizeof(result_list));
  result_list * a_last = a_first;
  result_list * b_last = b_first;
  int * numbers = malloc(sizeof(int) * 50);
  for(i = 0; i < 50; i++){
    numbers[i] = 50 - i;
  }
  sum_a = 0;
  sum_b = 0;
  sum_a += sqrt(numbers[0]);
  sum_b += sqrt(numbers[1]);
  a_first -> number = numbers[0];
  a_first -> next  = NULL;
  a_last = a_first;
  b_first -> number = numbers[1];
  b_first -> next = NULL;
  b_last = b_first;
  for(i = 2; i < 50; i++){
    if(sum_a >= sum_b){
     sum_b += numbers[i];
     b_last -> next = malloc(sizeof(result_list));
     b_last = b_last -> next;
     b_last -> number = numbers[i];
     b_last -> next  = NULL;
    }
    else{
      sum_a += numbers[i];
      a_last -> next = malloc(sizeof(result_list));
      a_last = a_last -> next;
      a_last -> number = numbers[i];
      a_last -> next  = NULL;
    }
  }
  printf("Difference: %f\n", fabs(sum_a - sum_b));
  printf("A:\t");
  while(a_first != NULL){
    printf("%d ",a_first -> number);
    a_first = a_first -> next;
  }
  printf("\n");
  printf("B:\t");
  while(b_first != NULL){
    printf("%d ",b_first -> number);
    b_first = b_first ->next;
  }
  printf("\n");
  return 0;
}
