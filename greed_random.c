#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct result_list{
 int number;
 struct result_list * next;
}result_list;

void shuffle(int *);
int main(){
  int i,j;
  double smallest_difference = 100;
  double sum_a,sum_b;
  result_list * a_first = malloc(sizeof(result_list));
  result_list * b_first = malloc(sizeof(result_list));
  result_list * a_last = a_first;
  result_list * b_last = b_first;
  int * numbers = malloc(sizeof(int) * 50);
  for(i = 0; i < 50; i++){
    numbers[i] = 50 - i;
  }
  for(j = 0; j < 200; j++){
  shuffle(numbers);
  result_list * a_first = malloc(sizeof(result_list));
  result_list * b_first = malloc(sizeof(result_list));
  result_list * a_last = a_first;
  result_list * b_last = b_first;
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
  if(smallest_difference > fabs(sum_a - sum_b)){
    smallest_difference = fabs(sum_a - sum_b);
  }
  free(a_first);
  free(b_first);
  free(a_last);
  free(b_last);
}
printf("Smallest Difference: %f\n",smallest_difference);
  return 0;
}
void shuffle(int * numbers){
  int i;
  int buffer;
  for(i = 0; i < 50; i++){
    int random = rand() % 50;
     buffer = numbers[i];
     numbers[i] = numbers[random];
     numbers[random] = buffer;
  }
}
