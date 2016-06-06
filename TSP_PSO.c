//This program is based on idea proposed in the paper PARTICLE SWARM OPTIMIZATION FOR TRAVELING SALESMAN PROBLEM, Wang et al. ,2003
//Problem set used here is also taken from the paper above for better evaluation since its optimal solution is known.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int particle_number;
double cognition_coefficient;
double social_coefficient;
int number_iterations;
typedef struct{
    double x_coordinate;
    double y_coordinate;
}city;

struct swap_operator{
    int swap_a;
    int swap_b;
    struct swap_operator * next;
};

city cities[14];
struct swap_operator ** velocities;
int ** particles;
int ** best_route_each_particle;
double * best_value_each_particle;
int * global_best_route;
double global_best_value;
int * result;
int * standard;
/*void generate_problem(){
 int i;
 FILE * fp = fopen("tsp_problem.txt","w");
 for(i = 0; i < 15; i++){
 cities[i].x_coordinate = rand() * 20.0 / RAND_MAX;
 cities[i].y_coordinate = rand() * 20.0 / RAND_MAX;
 fprintf(fp, "%f,",cities[i].x_coordinate);
 fprintf(fp, "%f\n",cities[i].y_coordinate);
 }
 fclose(fp);
 }*/
void generate_problem(){
    cities[0].x_coordinate = 16.47;
    cities[0].y_coordinate = 96.10;
    cities[1].x_coordinate = 16.47;
    cities[1].y_coordinate = 94.44;
    cities[2].x_coordinate = 20.09;
    cities[2].y_coordinate = 92.54;
    cities[3].x_coordinate = 22.39;
    cities[3].y_coordinate = 93.37;
    cities[4].x_coordinate = 25.23;
    cities[4].y_coordinate = 97.24;
    cities[5].x_coordinate = 22.00;
    cities[5].y_coordinate = 96.05;
    cities[6].x_coordinate = 20.47;
    cities[6].y_coordinate = 97.02;
    cities[7].x_coordinate = 17.20;
    cities[7].y_coordinate = 96.29;
    cities[8].x_coordinate = 16.30;
    cities[8].y_coordinate = 97.38;
    cities[9].x_coordinate = 14.05;
    cities[9].y_coordinate = 98.12;
    cities[10].x_coordinate = 16.53;
    cities[10].y_coordinate = 97.38;
    cities[11].x_coordinate = 21.52;
    cities[11].y_coordinate = 95.59;
    cities[12].x_coordinate = 19.41;
    cities[12].y_coordinate = 97.13;
    cities[13].x_coordinate = 20.09;
    cities[13].y_coordinate = 94.55;
}

void shuffle(int index){
    int i;
    for(i = 0; i < 50; i++){
        int a = rand() % 14;
        int b = rand() % 14;
        int buffer = particles[index][a];
        particles[index][a] = particles[index][b];
        particles[index][b] = buffer;
    }
}
double calculate_length(int index){
    int i;
    double x_1, x_2, y_1, y_2;
    double length = 0;
    for(i = 0; i < 13; i++){
        x_1 = cities[particles[index][i] - 1].x_coordinate;
        y_1 = cities[particles[index][i] - 1].y_coordinate;
        x_2 = cities[particles[index][i + 1] - 1].x_coordinate;
        y_2 = cities[particles[index][i + 1] - 1].y_coordinate;
        length += sqrt((x_2 - x_1) * (x_2 - x_1) + (y_2 - y_1) * (y_2 - y_1));
    }
    x_1 = cities[particles[index][13] - 1].x_coordinate;
    y_1 = cities[particles[index][13] - 1].y_coordinate;
    x_2 = cities[particles[index][0] - 1].x_coordinate;
    y_2 = cities[particles[index][0] - 1].y_coordinate;
    length += sqrt((x_2 - x_1) * (x_2 - x_1) + (y_2 - y_1) * (y_2 - y_1));
    return length;
}
void generate_first_iteration(){
    //Generate Initial Velocities
    int i,j;
    velocities = malloc(sizeof(struct swap_operator *) * particle_number);
    for(i = 0; i < particle_number; i++){
        velocities[i] = malloc(sizeof(struct swap_operator));
        velocities[i] -> swap_a = rand() % 13 + 1;
        velocities[i] -> swap_b = rand() % 13 + 1;
        velocities[i] -> next = NULL;
    }
    //Randomly Genrate Solution Candidates
    particles = malloc(sizeof(int *) * particle_number);
    for(i = 0; i < particle_number; i++){
        particles[i] = malloc(sizeof(int) * 14);
        for(j = 0; j < 14; j++){
            particles[i][j] = j + 1;
        }
        shuffle(i);
    }
    //Preserve Solution Candidates and Their Values
    best_route_each_particle = malloc(sizeof(int *) * particle_number);
    global_best_route = malloc(sizeof(int) * 14);
    global_best_value = 10000000;
    for(i = 0; i < particle_number; i++){
        best_route_each_particle[i] = malloc(sizeof(int) * 14);
    }
    best_value_each_particle = malloc(sizeof(double) * particle_number);
    for(i = 0; i < particle_number; i++){
        for(j = 0; j < 14; j++){
            best_route_each_particle[i][j] = particles[i][j];
        }
        best_value_each_particle[i] = calculate_length(i);
        if(best_value_each_particle[i] < global_best_value){
            global_best_value = best_value_each_particle[i];
            for(j = 0; j < 14; j++){
                global_best_route[j] = best_route_each_particle[i][j];
            }
        }
    }
}
//Mode 0: compare with best solution of particle; Mode 1: compare with global best solution; Mode 2: compare with "result"(used in simplify())
struct swap_operator * calculate_distance(int particle_index, int mode){
    int particle_solution[14];
    int best_solution[14];
    int i,j;
    struct swap_operator * element;
    struct swap_operator * begin = NULL;

    if(mode == 0){
        for(i = 0; i < 14; i++){
            best_solution[i] = best_route_each_particle[particle_index][i];
        }
    }
    else if (mode == 1){
        for(i = 0; i < 14; i++){
            best_solution[i] = global_best_route[i];
        }
    }
    else{
        for(i = 0; i < 14; i++){
            best_solution[i] = result[i];
        }
    }
    if(mode == 2){
        for(i = 0; i < 14; i++){
            particle_solution[i] = standard[i];
        }
    }
    else{
        for(i = 0; i < 14; i++){
            particle_solution[i] = particles[particle_index][i];
        }
    }
    while(1){
        int finished = 1;
        for(i = 0; i < 14; i++){
            if(particle_solution[i] != best_solution[i]){
                finished = 0;
                break;
            }
        }
        if(finished == 1){
            return begin;
        }
        finished = 1;
        for(j = 0; j < 14; j++){
            if(best_solution[j] == particle_solution[i]){
                if(begin == NULL){
                    begin = malloc(sizeof(struct swap_operator));
                    begin -> swap_a = j;
                    begin -> swap_b = i;
                    begin -> next = NULL;
                    element = begin;
                }
                else{
                    element -> next = malloc(sizeof(struct swap_operator));
                    element -> next -> swap_a = j;
                    element -> next -> swap_b = i;
                    element -> next -> next = NULL;
                    element = element -> next;
                }
                int buffer = particle_solution[j];
                particle_solution[j] = particle_solution[i];
                particle_solution[i] = buffer;
                break;
            }
        }

    }
}

void simplify(int index){
    standard = malloc(sizeof(int) * 14);
    result = malloc(sizeof(int) * 14);
    int i;
    for(i = 0; i < 14; i++){
        standard[i] = i + 1;
    }
    for(i = 0; i < 14; i++){
        result[i] = standard[i];
    }
    struct swap_operator * search = velocities[index];
    while(search != NULL){
        int a = search -> swap_a;
        int b = search -> swap_b;
        int buffer = result[a];
        result[a] = result[b];
        result[b] = buffer;
        search = search -> next;
    }

    free(velocities[index]);
    velocities[index] = calculate_distance(0,2);
}

void pso(){
    int i, j;
    for(i = 0; i < number_iterations; i++){
        for(j = 0; j < particle_number; j++){
            struct swap_operator * cognition_velocity = calculate_distance(j, 0);
            struct swap_operator * social_velocity = calculate_distance(j, 1);
            double rand_alpha = 1.0 * rand() / RAND_MAX;
            double rand_beta = 1.0 * rand() / RAND_MAX;
            struct swap_operator * search;
            if(rand_alpha < cognition_coefficient && cognition_velocity != NULL){
                search = velocities[j];
                if(search != NULL){
                while(search -> next != NULL){
                    search = search -> next;
                }
                search -> next = cognition_velocity;
              }
            }
            if(rand_beta < social_coefficient && cognition_velocity != NULL){
                search = velocities[j];
                if(search != NULL){
                while(search -> next != NULL){
                    search = search -> next;
                }
                search -> next = social_velocity;
              }
            }
            simplify(j);
            free(cognition_velocity);
            free(social_velocity);
            //Move to new position
            search = velocities[j];
            while(search != NULL){
                int a = search -> swap_a;
                int b = search -> swap_b;
                int buffer = particles[j][a];
                particles[j][a] = particles[j][b];
                particles[j][b] = buffer;
                search = search -> next;
            }
            double length = calculate_length(j);
            if(best_value_each_particle[j] > length){
                best_value_each_particle[j] = length;
                int iteration;
                for(iteration = 0; iteration < 14; iteration++){
                    best_route_each_particle[j][iteration] = particles[j][iteration];
                }
                if(global_best_value > length){
                    global_best_value = length;
                }
                for(iteration = 0; iteration < 14; iteration++){
                    global_best_route[iteration] = particles[j][iteration];
                }
            }
        }
    }
}

int main(int argc, char ** argv){
    if(argc < 5){
        printf("Usage: %s particle_number cognition_coefficient social_coefficient number_iterations\n", argv[0]);
        exit(1);
    }
    //srand((unsigned)time(NULL));
    particle_number = atoi(argv[1]);
    cognition_coefficient = atof(argv[2]);
    social_coefficient = atof(argv[3]);
    number_iterations = atoi(argv[4]);
    generate_problem();
    generate_first_iteration();
    pso();
    printf("Best Value:%f\n",global_best_value);
    int i;
    printf("Best Solution:\n");
    for(i = 0; i < 14; i++){
        printf("%d ",global_best_route[i]);
    }
    printf("\n");
    return 0;
}
