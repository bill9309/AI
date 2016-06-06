//A heuristic solution to the 0/1 Knapsack Problem using Ant Colony Optimization
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#define CAPACITY 165
#define OBJECT_NUM 10
int weights[OBJECT_NUM] = {23, 31, 29, 44, 53, 38, 63, 85, 89, 82};
int profits[OBJECT_NUM] = {92, 57, 49, 68, 60, 43, 67, 84, 87, 72};
//int weights[OBJECT_NUM] = {70, 73, 77, 80, 82, 87, 90, 94, 98, 106, 110, 113, 115, 118, 120};
//int profits[OBJECT_NUM] = {135, 139, 149, 150, 156, 163, 173, 184, 192, 201, 210, 214, 221, 229, 240};
//int weights[OBJECT_NUM] = {25, 35, 45, 5, 25, 3, 2, 2};
//int profits[OBJECT_NUM] = {350, 400, 450, 20, 70, 8, 5, 5};
int number_ants;
int number_loops;
double alpha;
double beta;
int ** solution_by_each_ant;
int * best_solution;
int best_value;
double pheromones[OBJECT_NUM];
double * fitness;
double evaporization_rate;
int randomly_choose(int * can_be_chosen){
  int i;
  int number_of_items_choosable = 0;
  for(i = 0; i < OBJECT_NUM; i++){
    if(can_be_chosen[i] == 1){
      number_of_items_choosable ++;
    }
  }
  double step = 1.0 / number_of_items_choosable;
  double random = 1.0 * rand() / RAND_MAX;
  int choice = (int)(random / step);
  for(i = 0; i < OBJECT_NUM; i++){
    if(can_be_chosen[i] == 1){
      if(choice == 0){
        return i;
      }
      choice --;
    }
  }
  return -1;
}

int pheromone_choose(int * can_be_chosen){
  int i;
  int numbers_choosable = 0;
  for(i = 0; i < OBJECT_NUM; i++){
    if(can_be_chosen[i] == 1){
      numbers_choosable ++;
    }
  }
  assert(numbers_choosable != 0);
  double * prob = malloc(sizeof(double) * numbers_choosable);
  int * items_choosable = malloc(sizeof(int) * numbers_choosable);
  double * accu_prob = calloc(numbers_choosable, sizeof(double));
  double prob_sum = 0;
  int index = 0;
  for(i = 0; i < OBJECT_NUM; i++){
    if(can_be_chosen[i] == 1){
      items_choosable[index] = i;
      index ++;
    }
  }
  if(numbers_choosable == 1){
    return items_choosable[0];
  }
  for(i = 0; i < numbers_choosable; i++){
     prob[i] = pow(pheromones[i],alpha) * pow(1.0 * weights[i] / profits[i], beta);
     prob_sum += prob[i];
  }
  for(i = 0; i < numbers_choosable; i++){
    prob[i] = prob[i] / prob_sum;
  }
  accu_prob[0] = prob[0];
  for(i = 1; i < numbers_choosable; i++){
    accu_prob[i] = accu_prob[i - 1] + prob[i];
  }
  double random = 1.0 * rand() / RAND_MAX;

  if(random < accu_prob[0]){
    return items_choosable[0];
  }
  for(i = 1; i < numbers_choosable; i++){
    if(random < accu_prob[i]){
      if(random > accu_prob[i - 1]){
        return items_choosable[i];
      }
    }
  }
  return -1;
}

//MODE 0: choose path randomly; MODE 1: choose path based on probability determined by pheromones
void generate_loop(int mode){
  int i,j;
  int weight_until_now;
  int value_until_now;
  int * can_be_chosen = malloc(sizeof(int) * OBJECT_NUM);
  int finished;
  int * one_solution = calloc(OBJECT_NUM, sizeof(int));
  int chosen;
  for(i = 0; i < number_ants; i++){
    finished = 1;
    for(j = 0; j < OBJECT_NUM; j++){
      can_be_chosen[j] = 1;
      one_solution[j] = 0;
    }
    weight_until_now = 0;
    value_until_now = 0;
    while(weight_until_now < CAPACITY){
      for(j = 0; j < OBJECT_NUM; j++){
        if(weight_until_now + weights[j] > CAPACITY){
          can_be_chosen[j] = 0;
        }
      }
      for(j = 0; j < OBJECT_NUM; j++){
        if(can_be_chosen[j] == 1){
          finished = 0;
          break;
        }
      }
      if(finished){
        break;
      }
      if(mode == 0){
        chosen = randomly_choose(can_be_chosen);
      }
      else{
        chosen = pheromone_choose(can_be_chosen);
      }
      //assert(chosen != -1);
      //assert(can_be_chosen[chosen] == 1);
      //assert(one_solution[chosen] == 0);
      can_be_chosen[chosen] = 0;
      one_solution[chosen] = 1;
      weight_until_now += weights[chosen];
      value_until_now += profits[chosen];
      finished = 1;
    }
    for(j = 0; j < OBJECT_NUM; j++){
      solution_by_each_ant[i][j] = one_solution[j];
    }
    fitness[i] = value_until_now;
    if(best_value < value_until_now){
      best_value = value_until_now;
      for(j = 0; j < OBJECT_NUM; j++){
        best_solution[j] = one_solution[j];
      }
    }
  }
  for(j = 0; j < number_ants; j++){
    fitness[j] = 1.0 * fitness[j] / best_value;
  }
}

void place_pheromone(){
  int i,j;
  for(i = 0; i < OBJECT_NUM; i++){
    for(j = 0; j < number_ants; j++){
      if(solution_by_each_ant[j][i] == 1){
        pheromones[i] += fitness[j];
      }
    }
  }
}

void aco_explore(){
  int i,j;
  int weight;
  int value;
  for(i = 0; i < number_loops; i++){
    generate_loop(1);
    for(j = 0; j < OBJECT_NUM; j++){
      pheromones[j] *= evaporization_rate;
    }
    place_pheromone();
  }

}
int main(int argc, char ** argv){
  if(argc < 6){
    printf("usage: %s number_of_ants number_loops alpha beta evaporization_rate\n",argv[0]);
    exit(1);
  }
  number_ants = atoi(argv[1]);
  number_loops = atoi(argv[2]);
  alpha = atof(argv[3]);
  beta = atof(argv[4]);
  evaporization_rate = atof(argv[5]);
  srand((unsigned)time(NULL));
  int i,j;
  solution_by_each_ant = malloc(sizeof(int *) * number_ants);
  for(i = 0; i < number_ants; i++){
    solution_by_each_ant[i] = calloc(OBJECT_NUM, sizeof(int));
  }
  for(i = 0; i < OBJECT_NUM; i++){
    pheromones[i] = 0;
  }
  best_solution = malloc(sizeof(int) * OBJECT_NUM);
  best_value = 0;
  fitness = malloc(sizeof(double) * number_ants);
  for(i = 0; i < number_ants; i++){
    fitness[i] = 0;
  }
  generate_loop(0);
  place_pheromone();
  for(i = 0; i < number_ants; i++){
    fitness[i] = 0;
    for(j = 0; j < OBJECT_NUM; j++){
      solution_by_each_ant[i][j] = 0;
    }
  }
  int times_success = 0;
  for(i = 0; i < 1000; i++){
    aco_explore();
    if(best_value == 309){
      times_success++;
    }
    best_value = 0;
  }
  //printf("Best value: %d\n",best_value);
  /*for(i = 0; i < OBJECT_NUM; i++){
    printf("%d ",best_solution[i]);
  }
  printf("\n");*/
  printf("Success Rate for Finding the best answer: %f\n", 1.0 * times_success/1000);
  return 0;
}
