#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
double fitness;
int data_index;
} fitness_data;

void calculate_fitness(int **, int, fitness_data *);
void normalize(fitness_data *);
void mutate();
int compare(const void *, const void *);
int size_population;
int max_generation;
double p_cross;
double p_mutation;
int ** population;
int ** next_gen;
fitness_data * fitness;
int gen_count;
int first_parent;
int second_parent;
int count_offspring;
int next_gen_size;
double best_fitness;
int * best_answer;
fitness_data * next_gen_fitness;

int main(int argc, char ** argv){
  best_fitness = 0;
  best_answer = malloc(sizeof(int) * 50);
  if(argc < 5){
    fprintf(stderr, "Usage: %s size_population max_generation p_cross p_mutation\n",argv[0]);
    exit(1);
  }
  gen_count = 0;
  size_population = atoi(argv[1]);
  max_generation = atoi(argv[2]);
  p_cross = atof(argv[3]);
  p_mutation = atof(argv[4]);
  population = malloc(sizeof(int *) * size_population);
  next_gen = malloc(sizeof(int *) * size_population + (int)(size_population * p_cross));
  fitness = malloc(sizeof(fitness_data) * size_population);
  next_gen_fitness = malloc(sizeof(fitness_data) * (size_population + (int)(size_population * p_cross)));
  next_gen_size = size_population + (int)(size_population*(1+p_cross));
  int i,j;

  //Randomly generate individuals
  for(i = 0; i < size_population; i++){
    population[i] = malloc(sizeof(int) * 50);
    for(j = 0; j < 50; j++){
      double random = 1.0 * rand()/RAND_MAX;
      if(random < 0.5){
        population[i][j] = 0;
        }
        else{
          population[i][j] = 1;
        }
    }
  }

  calculate_fitness(population, size_population, fitness);
  normalize(fitness);
  for(i = 0; i < next_gen_size; i++){
    next_gen[i] = malloc(sizeof(int) * 50);
  }
  while(gen_count < max_generation){
    gen_count++;
    count_offspring = 0;
    for(i = 0; i < next_gen_size; i++){
      next_gen[i] = malloc(sizeof(int) * 50);
    }
    while(count_offspring < (int)(size_population * p_cross)){
     double roulette = 1.0 * rand()/RAND_MAX;
     for(i = 0; i < size_population; i++){
       if(fitness[i].fitness <= roulette){
         if(fitness[i + 1].fitness >= roulette){
           first_parent = i;
           break;
         }
       }
     }
     roulette = 1.0 * rand()/RAND_MAX;
     for(i = 0; i < size_population; i++){
       if(fitness[i].fitness <= roulette){
         if(fitness[i + 1].fitness >= roulette){
           second_parent = i;
           break;
         }
       }
     }

     for(i = 0; i < 35; i++){
       next_gen[count_offspring][i] = population[first_parent][i];
     }
     for(i = 35; i < 50; i++){
       next_gen[count_offspring][i] = population[second_parent][i];
     }
     count_offspring++;
     if(count_offspring == (int)size_population * p_cross){
       break;
     }

     for(i = 0; i < 35; i++){
       next_gen[count_offspring][i] = population[second_parent][i];
     }
     for(i = 35; i < 50; i++){
       next_gen[count_offspring][i] = population[first_parent][i];
     }
     count_offspring++;

}
    for(i = count_offspring; i < size_population + (int)(size_population * p_cross); i++ ){
      for(j = 0; j < 50; j++){
        next_gen[i][j] = population[i - count_offspring][j];
      }

    }
  calculate_fitness(next_gen, size_population + size_population * p_cross, next_gen_fitness);
  qsort((void *)next_gen_fitness, (int)(size_population *(1 + p_cross)), sizeof(fitness_data), compare);
  for(i = 0; i < size_population; i++){
    for(j = 0; j < 50; j++){
      population[i][j]= next_gen[next_gen_fitness[i].data_index][j];
    }
  }
  for(i = 0; i < next_gen_size; i++){
    free(next_gen[i]);
  }
  mutate();
  calculate_fitness(population, size_population, fitness);
  double max_fitness = 0;
  double fitness_average = 0;
  for(i = 0; i < size_population; i++){
   fitness_average += fitness[i].fitness / size_population;
   if(max_fitness < fitness[i].fitness){
     max_fitness = fitness[i].fitness;
     if(best_fitness < max_fitness){
       best_fitness = max_fitness;
       for(j = 0; j < 50; j++){
         best_answer[j] = population[i][j];
       }
     }
   }
  }

  printf("Generation %d, Average Fitness %f, Best Fitness %f Smallest Difference %f\n", gen_count, fitness_average, max_fitness, 1.0/max_fitness);

  normalize(fitness);


}
printf("Smallest Difference: %f\n",1.0/best_fitness);
printf("Best Answer\n");
for(i = 0; i < 5; i ++){
  for(j = 0; j < 10; j++){
    printf("%d ",best_answer[i * 10 + j]);
  }
  printf("\n");
}
  return 0;

}
void calculate_fitness(int ** population, int size_population, fitness_data * fitness){
  int i, j;
  double sum_a, sum_b;
  for(i = 0; i < size_population; i++){
    sum_a = 0;
    sum_b = 0;
    for(j = 0; j < 50; j++){
      if(population[i][j] == 0){
       sum_a +=sqrt(j+1);
      }
      else{
        sum_b +=sqrt(j+1);
      }
    }
    //double fit = exp(fabs(sum_a-sum_b) * (-0.1));
    double fit = 1.0/fabs(sum_a-sum_b);
    fitness[i].fitness = fit;
    fitness[i].data_index = i;
  }

}

void mutate(){
 int i,j;
 for(i = 0; i < size_population; i++){
   for(j = 0; j < 50; j ++){
     double random = 1.0 * rand()/RAND_MAX;
     if(random <= p_mutation){
       if(population[i][j] == 0){
         population[i][j] = 1;
       }
       else{
         population[i][j] = 0;
       }
     }
   }
 }
}

void normalize(fitness_data * fitness){
  double sum = 0;
  int i;
  for(i = 0; i < size_population; i++){
    sum += fitness[i].fitness;
    fitness[i].fitness = sum;
  }
  for(i = 0; i < size_population; i++){
    fitness[i].fitness = fitness[i].fitness /sum;
  }
}

int compare(const void * a, const void * b){
  if((*(fitness_data *)a).fitness < (*(fitness_data *)b).fitness ){
    return 1;
  }
  else if((*(fitness_data *)a).fitness == (*(fitness_data *)b).fitness ){
    return 0;
  }
  else{
    return -1;
  }
}
