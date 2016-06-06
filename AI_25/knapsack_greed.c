#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CAPACITY 750
#define OBJECT_NUM 15
int weights[OBJECT_NUM] = {70, 73, 77, 80, 82, 87, 90, 94, 98, 106, 110, 113, 115, 118, 120};
int profits[OBJECT_NUM] = {135, 139, 149, 150, 156, 163, 173, 184, 192, 201, 210, 214, 221, 229, 240};
typedef struct {
 int weight;
 int profit;
 double fitness;
} OBJECT;

int comp(const void *c1, const void *c2){
  OBJECT object1 = *(OBJECT *)c1;
  OBJECT object2 = *(OBJECT *)c2;
  double fitness1 = object1.fitness;
  double fitness2 = object2.fitness;
  if(fitness1 > fitness2){
    return 1;
  }
  else{
    return -1;
  }
}

int main(){
  int i;
  int weight = 0;
  int value = 0;
  OBJECT objects[OBJECT_NUM];
  int * choice = calloc(OBJECT_NUM, sizeof(int));
  for(i = 0; i < OBJECT_NUM; i++){
    objects[i].weight = weights[i];
    objects[i].profit = profits[i];
    objects[i].fitness = 1.0 * objects[i].profit / objects[i].weight;
  }
  qsort(objects, OBJECT_NUM, sizeof(OBJECT), comp);
  i = 0;
  while(i < OBJECT_NUM){
    if(weight + objects[i].weight <= CAPACITY){
      weight += objects[i].weight;
      value += objects[i].profit;
      choice[i] = 1;
    }
    else {
      i++;
    }
  }
  printf("Best Value: %d\n",value);
}
